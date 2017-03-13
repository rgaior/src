#ifndef _phy_PhysicsFuctions_h_
#define _phy_PhysicsFuctions_h_

/**
   \file
   Physical functions for common use in the framework
   
   \author Luis Prado Jr
   \version $Id: PhysicsFunctions.h 967 2012-07-17 15:59:41Z fwerner $
   \date 19 May 2004
*/

namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}

class TVector3;

#include <vector>

double Integrate(std::vector<double>& v , double a, double b);

namespace utl {
  
  enum LDFType { 
    eNone,
    eNKG,
    eGora
  };

  double ElectronsAboveCut(double);
  double EnergyDeposit(const double age, const double enCut);
  double GoraAParameter(const double);
  double GoraBParameter(const double);
  //  void CalculateLDFPoint(TVector3& position, const double, const double, 
  //                       const double, const LDFType, ROOT::Math::GSLRngMT&);
  //double GoraFunction(double, double, ROOT::Math::GSLRngMT&);
  //double NKGFunction(double, const double, ROOT::Math::GSLRngMT&);
  void CalculateLDFPoint(TVector3& position, const double, const double, 
                         const double, const LDFType);
  double GoraFunction(double, double);
  double NKGFunction(double, const double);
  double IncompleteBeta(const double, const double, const double);
  double BetaCF(const double, const double, const double);
  double LogGamma(const double);
  double ShowerAge(double, double);
 
}

#endif // _phy_PhysicsFunctions_h_ 
