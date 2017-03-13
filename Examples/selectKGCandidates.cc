#include <Event.h>
#include <EventFile.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <TH1D.h>
#include <KGBits.h>
#include <BitTools.h>
#include <AugerUnits.h>
#include <TraceSimulator.h>
#include <GHShowerParameterization.h>
//#include <Configuration.h>
//#include <RandomGenerator.h>

#include <iostream>
#include <numeric>
using namespace std;
using namespace utl;

TraceSimulator* gTraceSimulator = NULL;
GHShowerParameterization* gGHParameters = NULL;

enum ESelFlag {
  eInit,
  eSelect,
  eFinish
};

//------------------------------------------------
bool GoodEvent(Event& event, ESelFlag flag) {

  static TH1D* h_selHist;
  if ( flag == eInit ) {
    h_selHist = new TH1D("selEvents","",9,0.,9.);
    TAxis* xAxis = h_selHist->GetXaxis();
    xAxis->SetBinLabel(1,"all");
    xAxis->SetBinLabel(2,"empty");
    xAxis->SetBinLabel(3,"ANKA");
    xAxis->SetBinLabel(4,"7/7");
    xAxis->SetBinLabel(5,"zenith");
    xAxis->SetBinLabel(6,"E>0");
    xAxis->SetBinLabel(7,"area");
    xAxis->SetBinLabel(8,"age");
    xAxis->SetBinLabel(9,"energy");
    return true;
  }
  else if ( flag == eFinish ) {
    h_selHist->Write();
  }
  else {

    double thisCut = 0.5;

    h_selHist->Fill(thisCut);
    thisCut++;
    
    const ArrayData& arrayData = event.GetArrayData();
    
    if ( !arrayData.GetTriggerFlag4() ) 
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    //---- F.Cossavella PhD table 4.3 
    
    if ( arrayData.GetTriggerFlag2()>=4 ) 
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    if ( ! BitIsSet(eSevenOfSeven, arrayData.GetTriggerFlag4()) )
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    
    //---- F.Cossavella PhD table 4.4 
    Shower& shower = event.GetArrayData().GetShower();
    
    if ( shower.GetZenith()/degree > 25.0 ) 
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;

    if ( !shower.HasEnergy() ) 
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    const TVector3& core = shower.GetCore();
    // read from Fig. 4.4(a)
    if ( sqrt( (core.X()+210.)*(core.X()+210.) + (core.Y()+235.)*(core.Y()+235.) ) > 200. ) 
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    if ( shower.GetAge() < -0.385 || shower.GetAge() > 1.485 )
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;
    
    if ( shower.GetEnergy() < 1.0e16 )
      return false;
    h_selHist->Fill(thisCut);
    thisCut++;

    return true;

  }

  return false;

}
//------------------------------------------------
void SimulateEvent(Event& event, DetectorSetup& detector) {

  // copy array shower to sim shower
  event.GetSimData().GetShower() = event.GetArrayData().GetShower();
  Shower& shower = event.GetSimData().GetShower();
  gGHParameters->FillShower(shower,
                            event.GetArrayData().GetShower().GetEnergy(),
                            1.*m,
                            event.GetArrayData().GetShower().GetCosZenith(),
                            true,
                            detector);
  //cout << "Xmax = " << event.GetSimData().GetShower().GetXmax()/(g/cm2) << endl;
//  gTraceSimulator->Run(event);

  // set the baseline like data

//   for ( Event::AntennaDataIterator iter = event.AntennasBegin();
//         iter != event.AntennasEnd(); ++iter ) {
//     for ( AntennaData::ChannelDataIterator chanIter = 
//             iter->ChannelsBegin();
//           chanIter != iter->ChannelsEnd(); ++chanIter ) {
//       ChannelData& channelData = *chanIter;
//       vector<Double_t>& signalTrace =
//         channelData.GetSignalTrace();
//       vector<Double_t>& simTrace =
//         channelData.GetSimulatedTrace();
      
//       const unsigned int nBaseBins = 1000;
 
//       if ( signalTrace.size() > nBaseBins ) {
//         const double baseLine = 
//           std::accumulate(signalTrace.begin(), 
//                           signalTrace.begin()+nBaseBins,0.)/(double) nBaseBins;
//         for ( unsigned int i=0; i<simTrace.size(); i++ ) {
//           simTrace[i]+=baseLine;
//         }
//       }

//     }
//   } 

}


//------------------------------------------------
int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: selectKGCandidates <cromeFiles> " << endl;
    return 1;
  }

  vector<string> filenames;
  for ( int i = 1; i < argc; ++i ) {
    filenames.push_back(string(argv[i]));
  }

  // Open the input and output files
  EventFile inputFile(filenames);
  EventFile outputFile("selected.root", EventFile::eWrite);

  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  Detector::GetInstance().InitFromFile(inputFile);

  // Create an event buffer and assign it to the input and output files
  Event* event = new Event();
  inputFile.SetEventBuffer(event);
  outputFile.SetEventBuffer(event);

  // Create the simulation chain
  gTraceSimulator = new TraceSimulator(&detector);
  gGHParameters = new GHShowerParameterization(5600);

  // Initialise the selection routine
  GoodEvent(*event, eInit);

  // Loop over all events
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    // Simulate and write selected events
    if ( GoodEvent(*event, eSelect) ) {
      if ( !event->HasSimData() ) {
        SimulateEvent(*event, detector);
      }

      outputFile.WriteEvent();
    }
  }

  // Finalise the output file
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();
  GoodEvent(*event, eFinish);

  cout << " selected " << outputFile.GetNumberOfEvents()
       << " event" << endl;
  outputFile.Close();

}
