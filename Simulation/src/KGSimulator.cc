#include "KGSimulator.h"

#include <iostream>

#include <Array.h>
#include <AugerUnits.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <Event.h>
#include <KGBits.h>
#include <RandomGenerator.h>

#include <Math/GSLRndmEngines.h>


using namespace std;
using namespace utl;


KGSimulator::KGSimulator():
  TimedClass("KGSimulator")
{

  fRandom = &(RandomGenerator::GetInstance().GetDetectorRandom());

}


KGSimulator::~KGSimulator() {

}


void KGSimulator::Run(Event& event) {

  StartTimer();

  // Copy shower to array data and fluctuate its parameters
  ArrayData& arrayData = event.GetArrayData();
  arrayData.GetShower() = event.GetSimData().GetShower();
  //FluctuateShower(arrayData.GetShower()); // Not Fluctuate 

  // Configure array
  arrayData.SetType(ArrayData::eAuger);
  arrayData.SetTriggerFlag1(0xfff);
  arrayData.SetTriggerFlag2(0);
  arrayData.SetTriggerFlag3(0xfff);
  arrayData.SetTriggerFlag4(1 << eSevenOfSeven);
  arrayData.SetDaqFlag1(0xfff);
  // Add station data
  const Array& array = Detector::GetInstance().GetSetup().GetArray();
  for ( Array::ConstStationIterator iter = array.StationsBegin();
        iter != array.StationsEnd(); ++iter ) {
    StationData stationData;
    stationData.SetId(iter->first);
    arrayData.AddStationData(stationData);
  }

  StopTimer();
}

