#ifndef __TraceSimulator_H
#define __TraceSimulator_H

#include <PhysicsFunctions.h>


namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}

class DetectorSetup;
class Event;


class TraceSimulator
{
 public:
  TraceSimulator();
  TraceSimulator(const DetectorSetup*);

  // simulate signal trace
  void Run(Event&);


 private:
  utl::LDFType fLDFmodel;
  unsigned int fThinning;
  double fMWyield;
  double fTau;
  unsigned int fNsample;
  const DetectorSetup* fDetectorSetup;
  ROOT::Math::GSLRngMT* fRandom;
};

#endif
