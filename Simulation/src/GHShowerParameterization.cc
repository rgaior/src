#include "GHShowerParameterization.h"
#include <AugerUnits.h>
#include <PhysicsFunctions.h>
#include <PhysicalConstants.h>
#include <Shower.h>
#include <RandomGenerator.h>
#include <TGraph.h>
#include <Math/GSLRndmEngines.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <Atmosphere.h>
#include <Detector.h>
#include <DetectorSetup.h>

using namespace std;
using namespace utl;

GHShowerParameterization::GHShowerParameterization(int primary):
  fX1Graph(NULL),
  fX0Graph(NULL),
  fDX0Graph(NULL),
  flogNGraph(NULL),
  fDlogNGraph(NULL),
  fXmaxGraph(NULL), 
  fDXmaxGraph(NULL), 
  fLambdaGraph(NULL),
  fDlambdaGraph(NULL),
  fPrimary(primary)
{

  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());
  InitTGraphs();

}

GHShowerParameterization::~GHShowerParameterization() {

  delete fX1Graph;
  delete fX0Graph;
  delete fDX0Graph;
  delete flogNGraph;
  delete fDlogNGraph;
  delete fXmaxGraph; 
  delete fDXmaxGraph; 
  delete fLambdaGraph;
  delete fDlambdaGraph;

}

void 
GHShowerParameterization::FillShower(Shower& shower, double energy,
                                     double deltaDepth, double costh,
                                     bool fluctuateParameters,
                                     const DetectorSetup& det)
{
  
  fEnergy = energy;
  shower.SetEnergy(fEnergy);  


  InitGHParameters(fluctuateParameters);

  shower.SetX1(fX1);
  shower.SetX0(fX0);
  shower.SetNmax(fNmax);
  shower.SetXmax(fXmax);
  shower.SetLambda(fLambda);

  cout << "GH Shower Parameters: " <<endl;
  cout << "Energy[eV] =  " << fEnergy/eV << endl;
  cout << "X0[g/cm2] =  " << fX0/(g/cm2) << endl;
  cout << "X1[g/cm2] =  " << fX1/(g/cm2) << endl;
  cout << "lambda [g/cm2]=  " << fLambda/(g/cm2) << endl;
  cout << "Nmax =  " << fNmax << endl;
  cout << "Xmax[g/cm2] =  " << fXmax/(g/cm2) << endl;



  const Atmosphere& atmo = Atmosphere::GetInstance();
  

  //const double startH = atmo.Height(fX1, costh) - deltaDepth/2.;  //50000.*m;
  const double stopH = det.GetArray().GetHeight();
 
  //double H = startH;// - deltaH/2.;

  const double stopD = atmo.Depth(stopH, costh);
  //const double startD = fX1 - deltaDepth/2.; //2.5 *g/cm2; //atmo.Depth(startH, costh) - deltaH/2.; // for this case the deltaH is a depth

  double depth = fX1;// - deltaDepth/2.;;
   
  while(depth < stopD ) {
  //while(H > stopH ) {
  
  
    //double depth = atmo.Depth(H,costh);
    shower.GetDepth().push_back(depth);
    shower.GetElectrons().push_back(Particles(depth));
    shower.GetEnergyDeposit().push_back(Particles(depth)*
                                        EnergyDeposit(ShowerAge(depth,fXmax),
                                                      0.1*MeV));
    //H -= deltaDepth; 
    depth += deltaDepth;
  }
 
}



void 
GHShowerParameterization::FillShower(Shower& shower, double energy,
                                     double deltaDepth, double costh,
                                     bool fluctuateParameters)  {
  FillShower(shower, energy, deltaDepth, costh, fluctuateParameters,
             Detector::GetInstance().GetSetup());

}


void GHShowerParameterization::InitTGraphs() {

  const unsigned int nPar = 8;
  const double glogE[nPar]  = {17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0};
  
  //PROTON
  const double gX1_p[nPar]      = 
    {49.70, 50.09, 46.64, 47.80, 43.78, 41.70, 40.33, 39.40};
  const double gX0_p[nPar]      = 
    {-165.72, -180.74, -202.48, -213.48, -219.73, -221.58, -219.71, -211.23};  
  const double gDX0_p[nPar]     = 
    {74.84, 64.83, 65.82, 63.10, 61.77, 53.59, 53.54, 51.32};
  const double glogN_p[nPar]    = 
    {8.317, 8.815, 9.312, 9.806, 10.300, 10.791, 11.282, 11.770};   
  const double gDlogN_p[nPar]   = 
    {0.021, 0.018, 0.013, 0.013, 0.013, 0.017, 0.020, 0.029};
  const double gXmax_p[nPar]    = 
    {643.68, 671.34, 698.36, 727.94, 752.81, 775.35, 802.12, 823.17};
  const double gDXmax_p[nPar]   = 
    { 49.15, 42.42, 39.74, 42.07, 44.50, 40.82, 40.37, 36.10};
  const double glambda_p[nPar]  = 
    {58.47, 57.18, 56.58, 57.30, 58.47, 60.92, 62.84, 64.00};
  const double gDlambda_p[nPar] = 
    {12.26, 6.43, 6.27, 5.82, 6.03, 7.30, 7.31, 6.76};
  
  // IRON
  const double gX1_fe[nPar]      = 
    {11.49, 11.74, 11.58, 11.83, 10.89, 11.38, 10.37, 10.47};
  const double gX0_fe[nPar]      = 
    {-98.29, -117.57, -133.69, -152.39, -171.40, -188.07, -203.03, -212.37};
  const double gDX0_fe[nPar]     = 
    {39.53, 43.11, 41.77, 39.98, 36.58, 32.51, 28.73, 25.10};
  const double glogN_fe[nPar]    = 
    {8.288, 8.793, 9.295, 9.796, 10.294, 10.792, 11.289, 11.786};
  const double gDlogN_fe[nPar]   = 
    {0.010, 0.010, 0.009, 0.009, 0.009, 0.009, 0.008, 0.007};
  const double gXmax_fe[nPar]    = 
    {586.59, 619.28, 648.27, 679.84, 709.61, 739.85, 770.11, 797.70};
  const double gDXmax_fe[nPar]   = 
    {21.54, 20.69, 17.41, 17.67, 17.30, 16.97, 18.67, 18.98};
  const double glambda_fe[nPar]  = 
    {65.92, 63.03, 61.05, 59.36, 58.57, 57.89, 58.01, 58.63};
  const double gDlambda_fe[nPar] = 
    {4.09, 3.55, 2.99, 2.43, 2.44, 2.02, 2.25, 2.44};
  

  if ( fPrimary == 100) {
    fX1Graph = new TGraph(nPar, glogE, gX1_p);
    fX0Graph = new TGraph(nPar, glogE, gX0_p);
    fDX0Graph = new TGraph(nPar, glogE, gDX0_p);
    flogNGraph = new TGraph(nPar, glogE, glogN_p);
    fDlogNGraph = new TGraph(nPar, glogE, gDlogN_p);
    fXmaxGraph = new TGraph(nPar, glogE, gXmax_p);
    fDXmaxGraph = new TGraph(nPar, glogE, gDXmax_p);
    fLambdaGraph = new TGraph(nPar, glogE, glambda_p);
    fDlambdaGraph = new TGraph(nPar, glogE, gDlambda_p);
  }
  else if( fPrimary == 5600) {
    fX1Graph = new TGraph(nPar, glogE, gX1_fe);
    fX0Graph = new TGraph(nPar, glogE, gX0_fe);
    fDX0Graph = new TGraph(nPar, glogE, gDX0_fe);
    flogNGraph = new TGraph(nPar, glogE, glogN_fe);
    fDlogNGraph = new TGraph(nPar, glogE, gDlogN_fe);
    fXmaxGraph = new TGraph(nPar, glogE, gXmax_fe);
    fDXmaxGraph = new TGraph(nPar, glogE, gDXmax_fe);
    fLambdaGraph = new TGraph(nPar, glogE, glambda_fe);
    fDlambdaGraph = new TGraph(nPar, glogE, gDlambda_fe);
  }
  else {
    ostringstream err;
    err << " unknown primary " << fPrimary;
    throw std::runtime_error(err.str());
  }
    
}



double 
GHShowerParameterization::Particles(double depth) const {

  const double N = fNmax
    *pow((depth-fX0)/(fXmax-fX0), 
	(fXmax-fX0)/fLambda)
    *exp((fXmax-depth)/fLambda);
  
  if(N>=1.) 
    return N;
  else 
    return 0.; 

}



void
GHShowerParameterization::InitGHParameters(bool fluctuateParameters)
{

  const double lgE = log10(fEnergy);

  if (fluctuateParameters) {
    fX1 = fRandom->Exponential(fX1Graph->Eval(lgE)) * g / cm2;
    fX0 = ((fX0Graph->Eval(lgE) +
            fRandom->Gaussian(fDX0Graph->Eval(lgE))) * g / cm2 + fX1);
    fNmax = pow(10, (flogNGraph->Eval(lgE) +
                     fRandom->Gaussian(fDlogNGraph->Eval(lgE))));
    fXmax = ((fXmaxGraph->Eval(lgE) +
              fRandom->Gaussian(fDXmaxGraph->Eval(lgE))) * g / cm2 + fX1);
    fLambda = ((fLambdaGraph->Eval(lgE) +
                fRandom->Gaussian(fDlambdaGraph->Eval(lgE))) * g / cm2);
  } else {
    fX1 = fX1Graph->Eval(lgE) * g / cm2;
    fX0 = fX0Graph->Eval(lgE) * g / cm2 + fX1;
    fNmax = pow(10, flogNGraph->Eval(lgE));
    fXmax = fXmaxGraph->Eval(lgE) * g / cm2 + fX1;
    fLambda = fLambdaGraph->Eval(lgE) * g / cm2;
  }

}
