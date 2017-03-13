#ifndef __KGSimulator__
#define __KGSimulator__

#include <Shower.h>
#include <TimedClass.h>


class Event;

namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}


class KGSimulator : public TimedClass
{
 public:
  KGSimulator();

  ~KGSimulator();

  void Run(Event& event);

  void FluctuateShower(Shower& shower) { shower.Fluctuate(); };


 private:
  ROOT::Math::GSLRngMT* fRandom;
};

#endif
