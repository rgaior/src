#include <boost/foreach.hpp>

#include <TRandom3.h>

#include <GHShowerParameterization.h>
#include <Configuration.h>
#include <GHShowerGenerator.h>
#include <TraceSimulator.h>
#include <BackgroundSimulator.h>
#include <ElectronicsSimulator.h>
#include <KGSimulator.h>
#include <Event.h>
#include <EventFile.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <TimedClass.h>
#include <AugerUnits.h>
#include <Atmosphere.h>
#include <PhysicalConstants.h>

using namespace std;
using namespace utl;


void PrintHeader();


// Adds channels which we want to simulate to the event
void addChannels(Event& event)
{

  BOOST_FOREACH(Antenna antenna, Detector::GetInstance().GetSetup().GetAntennas()) {
    event.MakeAntennaData(antenna.GetId());
    AntennaData& antennaData = event.GetAntennaData(antenna.GetId());
    BOOST_FOREACH(Channel channel, antenna.GetChannels()) {
      antennaData.MakeChannelData(channel.GetId());
    }
  }

}

int main(int argc, char** argv) {

  TimedClass* timer = new TimedClass("total");
  timer->StartTimer();

  if (argc != 4) {
    cerr << "Usage: " << argv[0];
    cerr << " configuration nShowers output" << endl;
    return 1;
  }

  const string configFilename = string(argv[1]);
  const unsigned int nShowers = (unsigned int) atoi(argv[2]);
  const string outputFilename = string(argv[3]);

  PrintHeader();
  cout <<  "detector initialized"<<endl;
  Configuration& config = Configuration::GetInstance();
  config.Init(configFilename);
  Detector::GetInstance().Init();
  EventFile theFile(outputFilename, EventFile::eWrite);
   
  GHShowerGenerator showerGenerator;
  TraceSimulator traceSimulator;
  //   BackgroundSimulator bgSimulator;
  //   ElectronicsSimulator elecSimulator;
  //   KGSimulator kgSimulator;
  
  for(unsigned int iEvent = 0; iEvent < nShowers; iEvent++) {
    Event event;
    theFile.SetEventBuffer(&event);
    
    event.SetEventId(iEvent+1);
    
    showerGenerator.Run(event);
    cout << "shower generator ok"  <<endl; 
    cout << "\n ===> event " << iEvent << endl;
    event.GetSimData().GetShower().DumpASCII();
    
    cout << " test pf tje sjower : " << event.GetSimData().GetShower().GetZenith() << endl;
    cout << " Sim data ok "<<endl;
    addChannels(event);
    cout << "Add channel ok "<<endl;
    traceSimulator.Run(event);

    //    cout << "trace simulator ok"<<endl;
    //     bgSimulator.Run(event);
    //     cout << "bg simulator ok "<<endl;
    //     elecSimulator.Run(event);
    //     kgSimulator.Run(event);
    theFile.WriteEvent();
  }

  theFile.WriteDetectorSetup(Detector::GetInstance().GetSetup());
  theFile.Close();
  cout << endl;
  delete timer;

  return 0;

}


void PrintHeader() {

  cout << "+----------------------------------+" << endl
       << "|                                  |" << endl
       << "|     EAS GHzMicrowave Simulation  |" << endl
       << "|                                  |" << endl
       << "|      version 2.0 - Sept 2013     |" << endl
       << "|                                  |" << endl
       << "|                                  |" << endl
       << "|                                  |" << endl
       << "|                                  |" << endl
       << "|                                  |" << endl
       << "+----------------------------------+" << endl;

}
