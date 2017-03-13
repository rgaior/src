#ifndef __ElectronicsSimulator_H
#define __ElectronicsSimulator_H

#include <TimedClass.h>
#include <vector>

class Event;

namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}


class ElectronicsSimulator : public TimedClass
{
 public:
  ElectronicsSimulator();

  // simulate signal trace
  void Run(Event&);
  void SimElecChain(std::vector<double>&, double, double);
  void plot(int, int,std::vector<double>&,std::vector<double>&);

 private:
  void LowPassFilter(std::vector<double>&, double, double) const;
  void HighPassFilter(std::vector<double>&, double, double) const;
  double PulseResponse(double, double, double, double) const;
  double fNoise;
  double fSigNoise;
  double fBeta;
  int fVerbosity;
  ROOT::Math::GSLRngMT* fRandom;
};

#endif
