#include <Event.h>
#include <EventFile.h>
#include <DetectorSetup.h>
#include <TH1D.h>
#include <KGBits.h>
#include <BitTools.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <TraceSimulator.h>
#include <GHShowerParameterization.h>

#include <iostream>
#include <numeric>
using namespace std;
using namespace utl;


enum ESelFlag {
  eInit,
  eRun,
  eFinish
};


bool Stack(Event& event, DetectorSetup& detector, ESelFlag flag) {
  static TH1D* hStacked;

  if(flag == eInit ) {

    for ( Event::ConstAntennaDataIterator iter = event.AntennasBegin();
          iter != event.AntennasEnd(); ++iter ) {

      const int antennaId = iter->GetId();
      AntennaData::ConstChannelDataIterator chanIter = iter->ChannelsBegin();
      const ChannelData& channelData = *chanIter;
      const std::vector<Double_t>& signalTrace = channelData.GetDataTrace();
      const unsigned int signaltraceLength = signalTrace.size();
      // if ( signalTrace.empty() )
      //   continue;
      const unsigned int channelId = chanIter->GetId();
      const double binWidth =
        detector.GetAntenna(antennaId).GetChannel(channelId).GetTraceBinWidth();
      //cout << "signaltraceLength = " << signaltraceLength << endl;
      const double t1 = channelData.GetDataStartTime()/microsecond;
      const double t2 = t1 + signaltraceLength*binWidth/microsecond;
      hStacked =
        new TH1D("hStacked","stacked traces", signaltraceLength, t1, t2);

    }
    return true;
  }
  else if(flag == eRun) {
     cout << "stack here "<<endl;
    for ( Event::ConstAntennaDataIterator iter = event.AntennasBegin();
        iter != event.AntennasEnd(); ++iter ) {


      for ( AntennaData::ConstChannelDataIterator chanIter =
	      iter->ChannelsBegin();
	    chanIter != iter->ChannelsEnd(); ++chanIter ) {

	const int antennaId = iter->GetId();

	int nChannel = 0;
	const ChannelData& channelData = *chanIter;
	const std::vector<Double_t>& simtrace = channelData.GetSimulatedFluxTrace();
	if ( simtrace.empty() )
	  continue;
	const std::vector<Double_t>& signalTrace = channelData.GetRealDataTrace();
	if ( signalTrace.empty() )
	  continue;
	
	const unsigned int signaltraceLength = signalTrace.size();
	const unsigned int simtraceLength = simtrace.size();
	const double t1sim = channelData.GetSimulationStartTime();
	const double t1sig = channelData.GetRealDataStartTime();
	//cout << "simtraceLength = " << simtraceLength << endl;
	const unsigned int channelId = chanIter->GetId();
	const double binWidth = detector.GetAntenna(antennaId).GetChannel(channelId).GetTraceBinWidth();
	
	double maxsign = 0;
	double peakPos = 0;
	for (unsigned int j=0; j<simtraceLength; j++) {
	  if( simtrace[j] > maxsign ) {
	    maxsign = simtrace[j];
	    peakPos = t1sim + ((double)j+0.5)*binWidth; 
	  }
	}
	nChannel++;
	
	if(peakPos==0) continue;	

	for (unsigned int j=0; j<signaltraceLength; j++) {
	  double signalcontent = signalTrace[j]/(watt/m2/hertz);
	  
	  double signaltime = t1sig + ((double)j + 0.5)*binWidth - peakPos;
	  cout << "signalcontent = " << signalcontent << " signaltime = " << signaltime/microsecond << endl;
	  //hStacked->GetBinContent(1);
	  hStacked->Fill(signaltime/microsecond, signalcontent);
	  //cout << "2" << endl;
	}

      }
     
    }
    return true;
  }
  else {
    hStacked->Write();
    return true;
  }


  return false;
}

bool GoodTrace(int nEvent){

  int bad[7] = {38, 39, 40, 41, 42, 43, 44};
  for (int i=0; i<7; i++) {
    if(nEvent == bad[i]) return false; 
  }
  return true;
}



int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: StackEvents <simFiles> " << endl;
    return 1;
  }
  vector<string> fileNames;
  for (int i = 1; i < argc; i++) {
    fileNames.push_back(string(argv[i]));
  }

  Event* event = new Event();
  EventFile inputFile(fileNames);
  inputFile.SetEventBuffer(event);
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  EventFile outputFile("stackedEvents.root", EventFile::eWrite);
  outputFile.SetEventBuffer(event);

  int nEvents = 0;
  int nEventsStacked = 0;
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    
    if (nEvents == 0) Stack(*event, detector, eInit);
    
    if( !event->HasSimData() ) {
      cerr << " No Sim Data in the events impossible to stack events ... please check!" << endl;
      return 2;
    }
    
    
    bool good = GoodTrace(nEvents);
    if(good) { 
      bool stacked = Stack(*event, detector, eRun);
      if (stacked) { cout << "STACK"<<endl;
	nEventsStacked++;
	cout << "Event n." << nEvents << " stacked!" << endl;
	outputFile.WriteEvent();
      }
    }
    nEvents++;
  }

  
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();
  Stack(*event, detector, eFinish);
  outputFile.Close();

}


