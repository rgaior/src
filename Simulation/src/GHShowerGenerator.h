#ifndef __GHShowerGenerator__
#define __GHShowerGenerator__

#include <TimedClass.h>
#include <TVector3.h>


namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}

class Event;
class GHShowerParameterization;


class GHShowerGenerator : public TimedClass
{

 public:
  GHShowerGenerator();
  ~GHShowerGenerator();
  void Run(Event&);


 private:
  double GenerateTheta(double , double , int);
  double GeneratePowerLaw(double , double , double);
  double func(double, double);


  int fVerbosity;
  double fEnergy;

  // parameters for spectrum
  double fMinE;
  double fMaxE;
  double fAlpha;

  // primary
  int fPrimary;

  // axis angles
  double fMinZen;
  double fMaxZen;
  double fMinAzi;
  double fMaxAzi;

  // core
  enum ECoreRandomType {
    eArray,
    eUser,
    eFixed,
    eHexagone
  };

  ECoreRandomType fCoreRandomType;
  TVector3 fCoreCenter;
  double fCoreDelta;

  // fluctuation
  bool fFluctuatePrimaryProperties;
  bool fFluctuateShowerParameterization;

  GHShowerParameterization* fGHParameterization;
  ROOT::Math::GSLRngMT* fRandom;
};

#endif
