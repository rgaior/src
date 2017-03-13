#include <string>
#include <iostream>

#include <Event.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <EventFile.h>
#include <StationData.h>
#include <ArrayData.h>

using namespace std;

int main(int argc, char** argv) {

  if ( argc != 2 ) {
    cerr << " usage: addCorrectedTriggerTime <File> " << endl;
    return 1;
  }

  string fileName;
  if ( argv[1] != NULL )
    fileName = string(argv[1]);
  else
    return 0;

  Event* event = new Event();
  EventFile inputFile(fileName);
  inputFile.SetEventBuffer(event);
  Detector::GetInstance().InitFromFile(inputFile);
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  string outFile = "mod_" + fileName;
  EventFile outputFile(outFile, EventFile::eWrite);
  outputFile.SetEventBuffer(event);

  if(inputFile.ReadNextEvent() == EventFile::eSuccess) {
    const DetectorSetup detSetup = Detector::GetInstance().GetSetup();
    ArrayData &arrayData = event->GetArrayData();
    arrayData.CalculateCorrectedTriggerTimes();
  }

  outputFile.cd();
  outputFile.WriteEvent();
  outputFile.WriteDetectorSetup(detector);
  outputFile.Close();

  return 0;

}

