#include <iostream>
#include <stdlib.h>

#include <Detector.h>
#include <Event.h>
#include <EventFile.h>
#include <DetectorSetup.h>
#include <TraceSimulator.h>
#include <GHShowerParameterization.h>



using namespace std;
using namespace utl;


int main(int argc, char** argv) {

  if (argc != 4) {
    cerr << "Usage: " << argv[0];
    cerr << " id input output" << endl;
    return 1;
  }

  const unsigned int targetEventId = (unsigned int) atoi(argv[1]);
  const string inputFilename = string(argv[2]);
  const string outputFilename = string(argv[3]);

  // Open the input file
  EventFile inputFile(inputFilename);

  // Initialize the detector
  Detector::GetInstance().InitFromFile(inputFile);

  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);

  // Create an event buffer and assign it to the input and output files
  Event* event = new Event();
  inputFile.SetEventBuffer(event);

  // Loop over all events
  bool foundEvent = false;
  while (inputFile.ReadNextEvent() == EventFile::eSuccess) {
    if (event->GetEventId() == targetEventId) {
      foundEvent = true;
      break;
    }
  }

  if (!foundEvent) {
    cerr << "Error: Couldn't find event with id " << targetEventId << "."
         << endl;
    return 1;
  }

  // Create the simulation chain
  TraceSimulator traceSimulator(&detector);
  GHShowerParameterization ghParameters(5600);

  // Open the output file
  EventFile outputFile(outputFilename, EventFile::eWrite);
  outputFile.SetEventBuffer(event);

  // Copy array shower to sim shower and init G.-H. parameters
  event->GetSimData().GetShower() = event->GetArrayData().GetShower();
  Shower& shower = event->GetSimData().GetShower();
  ghParameters.FillShower(shower,
                          shower.GetEnergy(),
                          1.*m,
                          shower.GetCosZenith(),
                          true,
                          detector);

  // Run the simulation
  cout << "Running simulation... " << flush;
  traceSimulator.Run(*event);
  cout << "done." << endl;

  // Finalise the output file
  outputFile.WriteEvent();
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();
  outputFile.Close();
}
