#include <TRandom.h>
#include <TBits.h>
#include <Event.h>
#include <EventFile.h>
#include <Configuration.h>
#include <AugerUnits.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
using namespace utl;

void fillEvent(int i, Event*);

int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: testEventFile <configXML> " << endl;
    return 1;
  }

  Configuration& config = Configuration::GetInstance();
  config.Init(string(argv[1]));

  cout << "Configuration file ok"<<endl;

  Detector::GetInstance().Init();

  cout << "intialize detector ok ..."<<endl;
  Event* testEvent = new Event();
  EventFile testFile("test.root", EventFile::eWrite);
  testFile.SetEventBuffer(testEvent);

  cout << "created test.root & event buffer ..."<<endl;

  const unsigned int nEvents = 1000;
  for ( unsigned int i=0; i<nEvents; i++ ) {

    fillEvent(i,testEvent);
    testFile.WriteEvent();

  }

  testFile.WriteDetectorSetup(Detector::GetInstance().GetSetup());
  testFile.Close();
  cout << "detector setup ok ..."<<endl;
}

void fillEvent(int iEvent, Event* theEvent) {

  const Detector& detector = Detector::GetInstance();
  const unsigned int nAntennas = detector.GetSetup().GetAntennas().size(); cout << "N antennas "<<nAntennas<<endl;
  if ( nAntennas <= 0 ) {
    cerr << " error: no antennas in detector " << endl;
    return;
  }
  const unsigned int nChannels = 
    detector.GetSetup().GetAntennas().front().GetNumberOfChannels(); cout << "N channels "<<nChannels<<endl;
  const unsigned int nTraceBins = 500;  

  // start with a fresh event
  *theEvent = Event();
  theEvent->SetEventId(iEvent+1);
  theEvent->SetRunId(123);
  theEvent->SetGPSSecond(966737481+iEvent);
  theEvent->SetGPSNanoSecond(gRandom->Uniform()*1e9);
  cout<< "Event Id, run, gps ... ok"<<endl;
  Shower& theShower = theEvent->GetArrayData().GetShower();
  const double lgE = gRandom->Uniform(16,19);
  theShower.SetEnergy(pow(10.,lgE));
  
  TVector3 theCore(gRandom->Uniform(detector.GetSetup().GetMinX(),
                                              detector.GetSetup().GetMaxX()),
                             gRandom->Uniform(detector.GetSetup().GetMinY(),
                                              detector.GetSetup().GetMaxY()),
		   0.);

  theShower.SetCore(theCore);


  TVector3 axis(0.,0.,1.);
  axis.SetTheta(gRandom->Uniform(0.,30.*degree));
  axis.SetPhi(gRandom->Uniform()*2*M_PI);
  theShower.SetAxis(axis);

  cout << "shower ok ..."<<endl;

  for ( unsigned int i=0; i<1/*nAntennas*/; i++ ) {
    AntennaData theAntenna;
    theAntenna.SetId(i+1);
    for ( unsigned int j=0; j<nChannels; j++ ) {
      ChannelData theChannel;
      theChannel.SetId(j+1);
      vector<Double_t>& dataTrace = theChannel.GetDataTrace(); cout <<" Get data trace ok... "<<endl;
      for ( unsigned int k=0; k<nTraceBins; k++ ) {
        const double noise = gRandom->Gaus();
        const double signalWidth = nTraceBins/100.;
        const double signalStart = nTraceBins/2 + j*signalWidth;
        const double signal = k > signalStart ?
          gRandom->Poisson(10.*exp(-(k-signalStart)/signalWidth)):
          0.;
        dataTrace.push_back(signal+noise);
      }
      theAntenna.AddChannelData(theChannel);
    }
      
    theEvent->AddAntennaData(theAntenna);
  }
  

  if ( iEvent && !(iEvent%250) ) {
    cout << "\n ===> event " << iEvent << endl;
    theShower.DumpASCII();
  }

}

