#include "RandomGenerator.h"

#include <iostream>

#include <Math/GSLRndmEngines.h>

#include "Configuration.h"


using namespace std;


void RandomGenerator::Init(unsigned int showerSeed,
                           unsigned int detectorSeed) {

  if (Configuration::GetInstance().GetVerbosity() > 0) {
    cout << "\n  ==[RandomGenerator::Init()]==> \n"
         << "   shower seed: " << showerSeed << "\n"
         << "   detector seed: " << detectorSeed << endl;
  }

  fShowerRandom = new ROOT::Math::GSLRngMT();
  fShowerRandom->Initialize();
  fShowerRandom->SetSeed(showerSeed);

  fDetectorRandom = new ROOT::Math::GSLRngMT();
  fDetectorRandom->Initialize();
  fDetectorRandom->SetSeed(detectorSeed);

}


ROOT::Math::GSLRngMT& RandomGenerator::GetShowerRandom() const
{

  if (!fShowerRandom) {
    if (Configuration::GetInstance().GetVerbosity() > 0) {
      cerr << " RandomGenerator::GetShowerRandom() - Warning: "
           << " initializing TRandom with default seeds... " << endl;
    }

    fShowerRandom = new ROOT::Math::GSLRngMT();
    fShowerRandom->Initialize();
    fShowerRandom->SetSeed(0);
  }

  return *fShowerRandom;

}


ROOT::Math::GSLRngMT& RandomGenerator::GetDetectorRandom() const
{

  if (!fDetectorRandom) {
    if (Configuration::GetInstance().GetVerbosity() > 0) {
      cerr << " RandomGenerator::GetDetectorRandom() - Warning: "
           << " initializing TRandom with default seeds... " << endl;
    }

    fDetectorRandom = new ROOT::Math::GSLRngMT();
    fDetectorRandom->Initialize();
    fDetectorRandom->SetSeed(0);
  }

  return *fDetectorRandom;

}
