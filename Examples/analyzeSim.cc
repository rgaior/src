#include <Event.h>
#include <EventFile.h>
#include <DetectorSetup.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TFile.h>
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

int counter;
 
enum ESelFlag {
  eInit,
  eRun,
  eFinish
};


bool doPlots(Event& event, DetectorSetup& detector, ESelFlag flag) {

  static TH1D* hDist;
  static TH1D* hNorm;
  //static TH1D* antennasGraph[NE];
  static vector<TH1D*> antennasGraph;
  static vector<TH1D*> antennasGraph_data;  
  static vector<TH1D*> antennasGraph_realdata;
  if(flag == eInit ) {
    hDist =  new TH1D("hDist","maximum of signal vs distance",50,0,15); //30 bins from 0 to 15 km
    //hDist->Sumw2();
    hNorm =  new TH1D("hNorm","maximum of signal vs distance- Normalization",50,0,15); //30 bins from 0 to 15 km
    //hNorm->Sumw2();
    
    return true;
  }
  else if(flag == eRun) {

    const ArrayData& arrayData = event.GetArrayData();
    const Shower& shower = arrayData.GetShower();

    counter = -1;
    for ( Event::ConstAntennaDataIterator iter = event.AntennasBegin();
	  iter != event.AntennasEnd(); ++iter ) {
         
      
      const int antennaId = iter->GetId();
      //(*detector)->GetAntenna(antennaId);
	
      const TVector3& antennaPos = detector.GetAntenna(antennaId).GetPosition() - shower.GetCore();
      const double chDist = antennaPos.Mag()/km; cout << antennaPos.Mag()/km<<endl;
      
      counter++;
      for ( AntennaData::ConstChannelDataIterator chanIter = iter->ChannelsBegin(); chanIter != iter->ChannelsEnd(); ++chanIter ) 
      {
	int nChannel = 0;
	const ChannelData& channelData = *chanIter;
	const std::vector<Double_t>& simtrace = channelData.GetSimulatedFluxTrace();
        const std::vector<Double_t>& datatrace = channelData.GetDataTrace();
	const std::vector<Double_t>& realdatatrace = channelData.GetRealDataTrace();
        const int channelId = chanIter->GetId();
	if ( simtrace.empty()||datatrace.empty() )
	  continue; 
	const unsigned int simtraceLength = simtrace.size();const unsigned int datatraceLength = datatrace.size();
	const double t1sim = channelData.GetSimulationStartTime(); const double t1data = channelData.GetDataStartTime();const double t1realdata = channelData.GetRealDataStartTime();
	const double binWidth = detector.GetAntenna(antennaId).GetChannel(channelId).GetTraceBinWidth();
	char graphName[20];char graphName1[20];char graphName2[20];	
	sprintf(graphName,"simTrace_ev%i_%i",event.GetEventId(), antennaId);
        sprintf(graphName1,"dataTrace_ev%i_%i",event.GetEventId(), antennaId);
        sprintf(graphName2,"realdataTrace_ev%i_%i",event.GetEventId(), antennaId);
	//cout << graphName << " " << channelId << " " << simtraceLength << " " << binWidth/microsecond << " " << t1sim/microsecond  << " " << (t1sim+simtraceLength*binWidth)/microsecond << endl;
	//antennasGraph[counter]->SetName(graphName);
	
        TH1D* temp = new TH1D(graphName, graphName, simtraceLength, t1sim/microsecond, (t1sim+simtraceLength*binWidth)/microsecond );
	TH1D* temp1 = new TH1D(graphName1, graphName1, datatraceLength, t1data/microsecond, (t1data+datatraceLength*binWidth)/microsecond );
	TH1D* temp2 = new TH1D(graphName2, graphName2, datatraceLength, t1realdata/microsecond, (t1realdata+datatraceLength*binWidth)/microsecond );
        //antennasGraph[counter] = new TH1D(graphName, graphName, simtraceLength, t1sim/microsecond, (t1sim+simtraceLength*binWidth)/microsecond );

	//const unsigned int channelId = chanIter->GetId();
	//const double binWidth = detector.GetAntenna(antennaId).GetChannel(channelId).GetTraceBinWidth();
	double maxSign = 0;
	//double peakPos = 0;
	for (unsigned int j=0; j<simtraceLength; j++) {
	  //const double x = (t1sim + j*binWidth);
	  //antennasGraph[counter]->SetBinContent(j+1, simtrace[j]/watt);
	  //temp->SetBinContent(j+1, simtrace[j]/(watt/(m*m)/hertz));
        temp->SetBinContent(j+1, simtrace[j]/watt);  
	if( simtrace[j] > maxSign ) {
	  maxSign = simtrace[j];
	   
	    //cout << x/microsecond << " " <<   simtrace[j]/watt << endl;
	    //peakPos = t1sim + ((double)j+0.5)*binWidth; 
	  }
	}

	 for (unsigned int j=0; j<datatraceLength; j++) {
          temp1->SetBinContent(j+1, datatrace[j]);
          temp2->SetBinContent(j+1,realdatatrace[j]);
        }




	antennasGraph.push_back(temp);
        antennasGraph_data.push_back(temp1);
        antennasGraph_realdata.push_back(temp2);
	nChannel++;
	
	if(maxSign==0) continue;	
	
	//cout << "antenna id: " << antennaId << " dist = " << chDist << " max = " << 1e12*maxSign/watt << endl;
	
	hDist->Fill(chDist, maxSign/watt);
	hNorm->Fill(chDist);
	
	//if(peakPos==0) continue;	
	
	//for (unsigned int j=0; j<signaltraceLength; j++) {
	//double signalcontent = signalTrace[j]/(watt/m2/hertz);
	  
	//double signaltime = t1sig + ((double)j + 0.5)*binWidth - peakPos;
	  //hStacked->GetBinContent(1);
	  //hStacked->Fill(signaltime/microsecond, signalcontent);
	  //cout << "2" << endl;
	//}
	
      }
      
    }
    return true;
  }
  else if(flag == eFinish) {
    hDist->Divide(hNorm);
    hDist->Write(); cout <<antennasGraph.size()<<endl;
    //cout << "******************************* HERE *" << counter << endl;
    for(int i=0; i<antennasGraph.size(); i++) {
      antennasGraph[i]->Write();
    }
    for(int i=0; i<antennasGraph_data.size(); i++) {
      antennasGraph_data[i]->Write();
    }
    for(int i=0; i<antennasGraph_realdata.size(); i++) {
      antennasGraph_realdata[i]->Write();
    }

    return true;

  }
  else return false;

}


int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: analyzeSim <simFiles> " << endl;
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
  TFile fPlots("myPlots.root","RECREATE");


  int nEvents = 0;
  int nEventsStacked = 0;
  //const int nev = 61*inputFile.GetNumberOfEvents();
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    //if(nEvents > 0) break;
      
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

}


