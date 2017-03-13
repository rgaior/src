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

//------------------------------------------------
void SimulateEvents(Event& event, DetectorSetup& detector) {

  // copy array shower to sim shower
  event.GetSimData().GetShower() = event.GetArrayData().GetShower();
  Shower& shower = event.GetSimData().GetShower();
  gGHParameters->FillShower(shower,
                            event.GetArrayData().GetShower().GetEnergy(),
                            1.*m,
                            event.GetArrayData().GetShower().GetCosZenith(),
                            true,
                            detector);
  gTraceSimulator->Run(event);

}


//------------------------------------------------
int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: simulateEvents <cromeFiles> " << endl;
    return 1;
  }

  vector<string> filenames;
  for ( int i = 1; i < argc; ++i ) {
    filenames.push_back(string(argv[i]));
  }

  // Open the input and output files
  EventFile inputFile(filenames);
  EventFile outputFile("simulated.root", EventFile::eWrite);

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

  // Loop over all events
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {

    SimulateEvents(*event, detector);

    outputFile.WriteEvent();

  }

  // Finalise the output file
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();

  cout << " selected " << outputFile.GetNumberOfEvents()
       << " event" << endl;
  outputFile.Close();

}
