#include "GHShowerGenerator.h"
#include "GHShowerParameterization.h"
#include "KGSimulator.h"

#include <AugerUnits.h>
#include <PhysicsFunctions.h>
#include <PhysicalConstants.h>
#include <Atmosphere.h>
#include <Configuration.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <RandomGenerator.h>
#include <Array.h>
#include <Event.h>
#include <Shower.h>

#include <TGraph.h>
#include <TF1.h>
#include <Math/GSLRndmEngines.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <tinyxml.h>
#include <tinyxmlCromeUtilities.h>

using namespace std;
using namespace utl;

GHShowerGenerator::GHShowerGenerator():
  TimedClass("GHShowerGenerator"),
  fFluctuatePrimaryProperties(false),
  fFluctuateShowerParameterization(false)
{

  fVerbosity = Configuration::GetInstance().GetVerbosity();
  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());

  if ( fVerbosity > 0 )
    cout << endl << "  ==[GHShowerGenerator::GHShowerGenerator()]==>" << endl;

  const string configId("GHShowerSimulation");
  const string& fileName =
    Configuration::GetInstance().GetConfigurationFileName(configId);

  TiXmlDocument doc(fileName);
  if ( doc.LoadFile() ) {

    const TiXmlElement* topBranch = doc.FirstChildElement(configId);
    if (!topBranch ) {
      throw std::runtime_error(string("Simulation::Init(): ")+
                               string("could not find GHShowerSimulation in ")+
                               fileName);
    }

    const TiXmlElement* showerBranch = topBranch->FirstChildElement("Shower");
    if (!showerBranch )
      throw std::runtime_error(string("Simulation::Init(): ")+
                               string("could not find Shower branch in ")+
                               fileName);


    fMinE = GetDoubleValue(showerBranch->FirstChildElement("Emin"))*eV;
    fMaxE = GetDoubleValue(showerBranch->FirstChildElement("Emax"))*eV;
    fAlpha = GetDoubleValue(showerBranch->FirstChildElement("alpha"));

    fMinZen = GetDoubleValue(showerBranch->FirstChildElement("minZen"))*degree;
    fMaxZen = GetDoubleValue(showerBranch->FirstChildElement("maxZen"))*degree;
    fMinAzi = GetDoubleValue(showerBranch->FirstChildElement("minAzi"))*degree;
    fMaxAzi = GetDoubleValue(showerBranch->FirstChildElement("maxAzi"))*degree;

    fPrimary = GetUnsignedValue(showerBranch->FirstChildElement("primary"));

    fFluctuatePrimaryProperties = GetBoolValue(
      showerBranch->FirstChildElement("fluctuatePrimaryProperties"));

    fFluctuateShowerParameterization = GetBoolValue(
      showerBranch->FirstChildElement("fluctuateShowerParameterization"));

    const TiXmlElement* coreBranch =
      showerBranch->FirstChildElement("coreRandomization");
    string coreType;
    if ( coreBranch ) {
      coreType = string(coreBranch->Attribute("type"));
      if ( coreType == string("insideArray") )
        fCoreRandomType = eArray;
      else if ( coreType == string("user") ) {
        fCoreRandomType = eUser;
        fCoreDelta =
          GetDoubleValue(coreBranch->FirstChildElement("delta"))*m;
        fCoreCenter = GetPosition(coreBranch);
      }
      else if ( coreType == string("fixed") ) {
        fCoreRandomType = eFixed;
        fCoreDelta = 0;
        fCoreCenter = GetPosition(coreBranch);
      }
     else if ( coreType == string("hexagone") ) {
        fCoreRandomType = eHexagone;
        fCoreDelta = GetDoubleValue(coreBranch->FirstChildElement("delta"))*m;
        fCoreCenter = GetPosition(coreBranch);
      }

    }

    if ( coreType.empty() )
      throw std::runtime_error("Simulation::Init(): "
                               "could not initialize core type");

    fGHParameterization = new GHShowerParameterization(fPrimary);

    if ( fVerbosity > 0 ) {
      cout << "   E = [" << fMinE/eV << "," << fMaxE/eV << "] eV,"
           << " alpha=" << fAlpha << endl
           << "   theta = [" << fMinZen/degree << ","
           << fMaxZen/degree << "] deg." << endl
           << "   phi = [" << fMinAzi/degree << ","
           << fMaxAzi/degree << "] deg." << endl
           << "   core type: " << coreType;
      if ( fCoreRandomType == eUser ||  fCoreRandomType == eFixed)
        cout << ", delta=" << fCoreDelta/m << " m  center = ("
             << fCoreCenter.X() << "," << fCoreCenter.Y()
             << "," << fCoreCenter.Z() << ")" << endl;
      cout << "   primary = " << fPrimary << endl;
      cout << "   fluctuate primary properties = "
           << fFluctuatePrimaryProperties << endl;
      cout << "   fluctuate shower parameterization = "
           << fFluctuateShowerParameterization << endl;

    }
  }

}

GHShowerGenerator::~GHShowerGenerator() {

}

void GHShowerGenerator::Run(Event& event) {

  StartTimer();

  Shower& myShower = event.GetSimData().GetShower();
  const DetectorSetup& setup = Detector::GetInstance().GetSetup();

  // Energy
  if (fMinE == fMaxE) {
    fEnergy = fMinE;
  } else {
    fEnergy = GeneratePowerLaw(fMinE, fMaxE, fAlpha);
  }
  myShower.SetEnergy(fEnergy);

  // Core
  TVector3 core;
  if ( fCoreRandomType == eArray ) {
    const double x =
      setup.GetMinX() + (setup.GetMaxX()-setup.GetMinX()) * (*fRandom)();
    const double y =
      setup.GetMinY() + (setup.GetMaxY()-setup.GetMinY()) * (*fRandom)();
    const double z = 0.;
    core.SetXYZ(x,y,z);
  } else if ( fCoreRandomType == eUser ) {
    if ( fCoreDelta > 0 ) {
      const double x =
        fCoreCenter.X() + fCoreDelta * ( (*fRandom)() - 0.5 );
      const double y =
        fCoreCenter.Y() + fCoreDelta * ( (*fRandom)() - 0.5 );
      const double z = 0.;
      core.SetXYZ(x,y,z);
    } else {
      const double x = fCoreCenter.X();
      const double y = fCoreCenter.Y();
      const double z = 0.;
      core.SetXYZ(x,y,z);
    }
  } else if ( fCoreRandomType == eFixed ) {
    core.SetXYZ(fCoreCenter.X(), fCoreCenter.Y(), fCoreCenter.Z());
  }

   else if (fCoreRandomType == eHexagone ) {
     //first triangle
     double pz = 0 ;
     const double C_x = fCoreCenter.X()-fCoreDelta/2. ; const double B_x = fCoreCenter.X() + fCoreDelta ; 
     const double C_y = fCoreCenter.Y()+sqrt(3.)/2.*fCoreDelta ; const double B_y = fCoreCenter.Y() ;
     const double O_x = fCoreCenter.X()-fCoreDelta/2. ; const double O_y = fCoreCenter.Y() ;
     //second triangle
     const double J_x = fCoreCenter.X()+fCoreDelta/2. ; const double K_x = fCoreCenter.X()-fCoreDelta ;
     const double J_y = fCoreCenter.Y()+sqrt(3.)/2.*fCoreDelta ; const double K_y = fCoreCenter.Y() ;
     const double I_x = fCoreCenter.X()+fCoreDelta/2. ; const double I_y = fCoreCenter.Y() ; 
     
     double px, py, pb ;
     while (1) {
       px = O_x + (*fRandom)() * (I_x-O_x);
       py = O_y + (*fRandom)()* (C_y-O_y);

       pb = C_y +((B_y-C_y)/(B_x-C_x))* (px-O_x);
       if (py>pb) continue ;

       pb = J_y +((K_y-J_y)/(K_x-J_x))*(px-I_x);
       if (py<pb) break ;
     }   
    const double rd = (*fRandom)();
    if (rd <0.5) py = 2*fCoreCenter.Y()-py;
    core.SetXYZ(px,py,pz);
   }

  myShower.SetCore(core);

  // Axis
  const double zen = GenerateTheta(fMinZen, fMaxZen, 1);
  const double azi = fMinAzi + (*fRandom)() * (fMaxAzi - fMinAzi);
  TVector3 axis(sin(zen)*cos(azi) , sin(zen)*sin(azi), cos(zen));
  myShower.SetAxis(axis);

  // Fluctuate energy, core, and axis if requested
  if ( fFluctuatePrimaryProperties ) {
    KGSimulator kgSimulator;
    kgSimulator.FluctuateShower(myShower);
  }

  // Gaisser-Hillas parameters
  //const double deltaDepth = 10. * m;
  const double deltaDepth = 2.5 * g/cm2;

  const double cosz = myShower.GetAxis().CosTheta();
  fGHParameterization->FillShower(myShower, myShower.GetEnergy(), deltaDepth, cosz,
                                  fFluctuateShowerParameterization);

  // Time of impact
  const Atmosphere& atmo = Atmosphere::GetInstance();
  const double coreHeight =
    myShower.GetCore().Z() + setup.GetArray().GetHeight();
  const double hStart =
    fmin(atmo.Height(myShower.GetX1(), cosz), atmo.GetMaxHeight());
  const double tGround = (hStart - coreHeight) / kSpeedOfLight / cosz;
  myShower.SetCoreTime(0, (UInt_t) (tGround / nanosecond));

  StopTimer();
}

double
GHShowerGenerator::GenerateTheta(double fZenithMin, double fZenithMax,
                                 int fNcos) {

  if(fZenithMin==fZenithMax) {

    return fZenithMax;

  } else {

    double theta_1 = pow(cos(fZenithMax), fNcos+1);
    double theta_2 = pow(cos(fZenithMin), fNcos+1);

    double r = (*fRandom)();
    double tmp = (theta_2-theta_1) * r + theta_1;

    double theta;
    if (fNcos == 0)
      theta = acos (tmp);
      else if (fNcos == 1)
        theta = acos (sqrt (tmp));
      else
        theta = acos (exp( log(tmp)/(fNcos+1) ) );

    return theta;
  }
}


 double GHShowerGenerator::func(double e, double index) {
    double log10e12 = 19.63; 
    double log10wc = 0.15; 
    double spec = pow(e,-index)/(1+ exp((log10(e)-log10e12)/(log10wc)));
    return spec;
  }

double
GHShowerGenerator::GeneratePowerLaw(double min, double max, double index)
{
  //commentaire rom
  // rejection sampling algo
  // use 2 different random numbers
  double x = 0.; 
  double rd = 0.; 
  double p = 0.;  
  double randNo =0. ;
  double randNo2 =0. ;
  while(1) {
      randNo = (*fRandom)();
      randNo2 = (*fRandom)();
      x = min+(max-min)*randNo ; 
      rd = randNo2*func(min,index); 
      p =func(x, index); 
      
      if (p>=rd) return (x);
      
  }
  //OLD
  /*if (min>0.0 && max>0.0) {
    double randNo = (*fRandom)();
    if (index != -1) {
      x = pow(pow(min, index + 1.0) + randNo*(pow(max, index + 1.0)
          - pow(min, index + 1.0)), 1.0/(index + 1.0));
    }
  }
  return (x);*/
}
