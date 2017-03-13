#ifndef __GHShowerParameterization__
#define __GHShowerParameterization__

#include <DetectorSetup.h>


namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}

class TGraph;
class Shower;


class GHShowerParameterization {

public:
  GHShowerParameterization(int primary);
  ~GHShowerParameterization();
  void FillShower(Shower& shower, double energy,
                  double deltaH, double costh,
                  bool flutuateParameters,
                  const DetectorSetup&);
  void FillShower(Shower& shower, double energy,
                  double deltaH, double coszth,
                  bool flutctuateParameters);

 private:
  GHShowerParameterization();

  double Particles(double) const;

  void InitTGraphs();
  void InitGHParameters(bool fluctuateParameters);

private:

  double fEnergy;
  double fX1;
  double fX0;
  double fNmax;
  double fXmax;
  double fLambda;

  // for interpolation of GH parameters
  TGraph* fX1Graph;
  TGraph* fX0Graph;
  TGraph* fDX0Graph;
  TGraph* flogNGraph;
  TGraph* fDlogNGraph;
  TGraph* fXmaxGraph;
  TGraph* fDXmaxGraph;
  TGraph* fLambdaGraph;
  TGraph* fDlambdaGraph;
  int fPrimary;

  ROOT::Math::GSLRngMT* fRandom;

};

#endif
