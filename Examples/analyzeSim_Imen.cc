#include <Event.h>
#include <EventFile.h>
#include <DetectorSetup.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TFile.h>
#include <KGBits.h>
#include <BitTools.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <TraceSimulator.h>
#include <GHShowerParameterization.h>
#include <ChannelData.h>
#include <iostream>
#include <numeric>
#include <TApplication.h>
#include <TRint.h>
using namespace std;
using namespace utl;

int counter;
 
enum ESelFlag {
  eInit,
  eRun,
  eFinish
};


bool doPlots(Event& event, DetectorSetup& detector, ESelFlag flag) {

  static TH1D* hDist;
  static TH1D* hNorm;
  static vector<TH1D*> antennasGraph;  
  static vector<TH1D*> antennasGraphSens;
  if(flag == eInit ) {
    hDist =  new TH1D("hDist","maximum of signal vs distance",300,0,15); //30 bins from 0 to 15 km
    //hDist->Sumw2();
    hNorm =  new TH1D("hNorm","maximum of signal vs distance- Normalization",300,0,15); //30 bins from 0 to 15 km
    //hNorm->Sumw2();
    return true;
  }
  else if(flag == eRun) {

    //const Shower& shower = arrayData.GetShower();
    Shower& shower = event.GetSimData().GetShower();
    counter = -1;
    
    //loop an antennas
    for ( Event::ConstAntennaDataIterator iter = event.AntennasBegin(); iter != event.AntennasEnd(); ++iter ) {
          
      const int antennaId = iter->GetId();
      //cout << "Antenna ID : "<< antennaId;
      //(*detector)->GetAntenna(antennaId);
      const TVector3& showerCore = shower.GetCore();	
      const TVector3& antennaPos = detector.GetAntenna(antennaId).GetPosition() - showerCore;
      const double chDist = antennaPos.Mag()/km;
      const double x = detector.GetAntenna(antennaId).GetPosition().X(); const double y = detector.GetAntenna(antennaId).GetPosition().Y(); 
      //cout << " d [km]:  "<< chDist << " Shower position [m] :  "<<shower.GetCore().Mag()/km<<" antenna position [km] :  "<<detector.GetAntenna(antennaId).GetPosition().Mag()/km <<endl;
     
      counter++;
      //loop on channels
  
   // if (chDist> 0.5) {       

      for ( AntennaData::ConstChannelDataIterator chanIter = iter->ChannelsBegin(); chanIter != iter->ChannelsEnd(); ++chanIter ) {
	
	int nChannel = 0;
	
        const ChannelData& channelData = *chanIter;
	const std::vector<Double_t>& simtrace = channelData.GetSimulatedFluxTrace(); // signal trace
        const std::vector<Double_t>& minfluxtrace = channelData.GetMinimumFluxTrace(); // minimum flux trace
        const int channelId = chanIter->GetId();
      
        //cout << "Channel ID : "<< channelId <<endl;//<<"Channel minimum flux : "<<s/(watt/m2/Hz)<< endl;
	
        if ( simtrace.empty() ) { cout << "The trace is empty" << endl; continue;}

	const unsigned int simtraceLength = simtrace.size();
	const double t1sim = channelData.GetSimulationStartTime();
	const double binWidth = detector.GetAntenna(antennaId).GetChannel(channelId).GetTraceBinWidth();
	char graphName[20]; char graphNameSens[20];
	sprintf(graphName,"simTrace_ev%i_%i",event.GetEventId(), antennaId);
	sprintf(graphNameSens,"simTracesens_ev%i_%i",event.GetEventId(), antennaId);

	TH1D* temp = new TH1D(graphName, graphName, simtraceLength, t1sim/microsecond, (t1sim+simtraceLength*binWidth)/microsecond );
        TH1D* tempsens = new TH1D(graphNameSens, graphNameSens, simtraceLength, t1sim/microsecond, (t1sim+simtraceLength*binWidth)/microsecond );	
        
        //antennasGraph[counter] = new TH1D(graphName, graphName, simtraceLength, t1sim/microsecond, (t1sim+simtraceLength*binWidth)/microsecond );

	double maxSign = 0; 
        double minflux = 0.;
        double sigma = 0.;
	// loop on trace bins
        for (unsigned int j=0; j<simtraceLength; j++) {
	  temp->SetBinContent(j+1, simtrace[j]/(watt/m2/Hz));
          tempsens->SetBinContent(j+1, minfluxtrace[j]/(watt/m2/Hz));
	  if( simtrace[j] > maxSign ) {
	    maxSign = simtrace[j]; //the maximum of the trace 
	    //peakPos = t1sim + ((double)j+0.5)*binWidth; // time of the maximum
	  }
         if (minfluxtrace[j] !=0) {minflux = minfluxtrace[j];} // eliminate 0 content 
	}
        antennasGraph.push_back(temp);
	antennasGraphSens.push_back(tempsens);
        
        nChannel++;
	
	if(maxSign==0) continue;	
	hDist->Fill(chDist, maxSign/(watt/m2/Hz)); //fill maximum distribution as a function of antenna distance
        hNorm->Fill(chDist); 
	//output antenna position and sigma value for mapping 
        cout << x/m <<" "<<y/m <<" "<<(100*maxSign-minflux)/minflux<<endl;
      } 
     // }// End condition on distance 
      // End loop on channels
    }
    // End loop on antennas
    
    return true;
  }
  else if(flag == eFinish) {
    
    hDist->Divide(hNorm);
    hDist->Write();
    cout << "******************************* Event counter : " << counter << endl;
    
    for(int i=0; i<antennasGraph.size(); i++) {
      antennasGraph[i]->Write();
      antennasGraphSens[i]->Write();
    }

    return true;

  }
  else return false;

}


int main(int argc, char** argv) {

TApplication *app = new TRint("app",0,0,0);

  if ( argc < 2 ) {
    cerr << " usage: analyzeSim <simFiles> " << endl;
    return 1;
  }
  vector<string> fileNames; vector<string> fileNames1;
  for (int i = 1; i < argc; i++) {
    fileNames.push_back(string(argv[i]));
  }
 fileNames1.push_back("testroot.root"); 
  
  Event* event = new Event();
  cout << "OK1"<<endl;
  EventFile inputFile(fileNames); EventFile inputFile1(fileNames1);cout << "OK2"<<endl;
  inputFile.SetEventBuffer(event);cout << "OK3"<<endl;
  DetectorSetup detector;cout << "OK4"<<endl;
  inputFile1.ReadDetectorSetup(detector);cout << "OK5"<<endl;
  TFile fPlots("myPlots.root","RECREATE");cout << "OK6"<<endl;


  int nEvents = 0;
  int nEventsStacked = 0;
  //const int nev = 61*inputFile.GetNumberOfEvents();
  
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    if (nEvents == 0) doPlots(*event, detector, eInit);
    if( !event->HasSimData() ) {
      cerr << " No Sim Data in the events impossible to do the plots ... please check!" << endl;
      return 2;
    }
    doPlots(*event, detector, eRun);
    nEvents++;
  }

  doPlots(*event, detector, eFinish);
  fPlots.Close();
  //app->Run();
}


