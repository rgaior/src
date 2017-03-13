#ifndef __BackgroundSimulator_H
#define __BackgroundSimulator_H

#include <TimedClass.h>

class Event;

namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}


class BackgroundSimulator : public TimedClass
{
 public:
  BackgroundSimulator();
  double S;
  // simulate signal trace
  void Run(Event&);


 private:
  double fNoiseFluctuation;
  ROOT::Math::GSLRngMT* fRandom;
};

#endif
