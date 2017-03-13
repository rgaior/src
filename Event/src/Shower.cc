#include <cmath>

#include "Shower.h"
#include <Atmosphere.h>
#include <AugerUnits.h>
#include <RandomGenerator.h>
#include <Math/GSLRndmEngines.h>


using namespace std;
using namespace utl;


ClassImp(Shower)


Shower::Shower() :
  fEnergy(-1.),
  fEnergyError(0.2),
  fXmax(0.),
  fNmax(0.),
  fX0(0.),
  fX1(0.),
  fLambda(0.),
  fAge(0.),
  fElectronSize(0),
  fMuonSize(0),
  fCoreTimeSecond(0),
  fCoreTimeNanoSecond(0),
  fCore(0.,0.,0.),
  fCoreError(6*m),
  fAxis(0.,0.,0.),
  fAxisError(0.8*degree)
{
  // Energy Resolution 20%;
  // Reference: PhD thesis of F.Cossavella, Fig. A.1

  // core uncertainty 6m for highest energy
  // axis uncertainty 0.8deg
  // Reference: doi:10.1016/j.nima.2010.03.147, Figs. 23, 24
}


Double_t
Shower::GetZenith()
  const
{
  return fAxis.Theta();
}


Double_t
Shower::GetCosZenith()
  const
{
  return fAxis.Z();
}


Double_t
Shower::GetAzimuth()
  const
{
  double azi = fAxis.Phi();
  if (azi < 0)
    azi = 2.*M_PI + azi;
  return azi;
}


void
Shower::DumpASCII(std::ostream& o)
  const
{
  o << "  energy [PeV]           " << fEnergy/1.e15 << '\n'
    << "  x/y/z [m]  " << fixed << fCore.X()
    << " / " << fCore.Y() << " / " << fCore.Z() << '\n'
    << "  theta/phi [deg] " << fAxis.Theta()*180./M_PI
    << " / " << fAxis.Phi()*180./M_PI
    << endl;
}

void
Shower::Dump(std::ostream& o)
  const
{
  o << "shower:energy [EeV]   x y z [m] theta[deg] phi[deg] " << "\n"
    << fEnergy/1.e18 << " " << fCore.X() << " " << fCore.Y() << " " << fCore.Z() 
    << " " << fAxis.Theta()*180./M_PI << " " << fAxis.Phi()*180./M_PI 
    << endl;
  o << "depth: ";
    int dsize= fDepth.size();
    for (int idep = 0; idep < dsize; ++idep){
      o << fDepth[idep] << " ";
    }
  o << endl;
  o << "electron: ";
    int esize= fElectrons.size();
    for (int ie = 0; ie < esize; ++ie){
      o << fElectrons[ie] << " ";
    }
  o << endl;
  o << "dedx: ";
    int dedxsize= fdEdX.size();
    cout << "dedxsize = " << dedxsize << endl;
    for (int idedx = 0; idedx < dedxsize; ++idedx){
      o << fdEdX[idedx] << " ";
    }
  o << endl;
  int psize = fPosition.size();
  o << "position X Y Z "; 
  for (int ip = 0; ip < psize; ++ip){
    TVector3 pos = fPosition[ip];
    o << pos.X()<< " " << pos.Y() << " " << pos.Z() <<" " ;
  }
  

//   o << "height: ";
//     int hsize= fHeight.size();
//     cout << "hsize = " << hsize << endl;
//     for (int ih = 0; ih < hsize; ++ih){
//       o << fHeight[ih] << " ";
//     }
  o << endl;

}


// void
// Shower::SetHeight()
// {
//   const Atmosphere& atmo = Atmosphere::GetInstance();
//   //  const double zenith = fZenith;
//   const double cosZenith = GetCosZenith();
//   int dsize = fDepth.size();
//   vector <double> h_v;
//   for (int iDepth = 0; iDepth < dsize ; iDepth++) { 
//     double h = atmo.Height(fDepth[iDepth], cosZenith);
//     h_v.push_back(h);
//   }
//   fHeight = h_v;
// }


void
Shower::Fluctuate()
{
  ROOT::Math::GSLRngMT* random = &(RandomGenerator::GetInstance().GetDetectorRandom());

  // Shower core variation
  //
  // Reference: doi:10.1016/j.nima.2010.03.147, Figs. 23, 24
  // sigmaDeltaR is shown in Fig. 23. It's the 68% quantile of the
  // distance to the real shower core, which is taken to be Rayleigh
  // distributed.  We'll smear the core X and Y with two
  // Gaussians. The standard deviations of the Gaussians are assumed to
  // be equal and are calculated as
  //   sigma_xy = sqrt( -sigmaDeltaR^2 / ( 2 * ln(1 - 0.68) ) )
  const double sigmaDeltaR = GetCoreError();
  const double coreSigma = sqrt(-1.0 * sigmaDeltaR * sigmaDeltaR /
                                (2.0 * log(1.0 - 0.68)));
  // Smear core using two gaussian distributions (resulting in a Rayleigh
  // distribution for the distance)
  TVector3 core = GetCore();
  core.SetX(core.X() + random->Gaussian(coreSigma));
  core.SetY(core.Y() + random->Gaussian(coreSigma));
  SetCore(core);

  // Shower axis variation
  //
  // sigmaDeltaOmega is again the 68% quantile of the angular distance
  // to the real shower axis. The standard deviations of the two
  // Gaussians used to smear the shower pointing are calculated like
  // above.
  // Note: In Fig. 24 the uncertainty seems to rise with energy. We
  // assume this is due to either statistical fluctuations or the
  // saturation of KASCADE (not Grande). Therefore a value of 0.8° is
  // believed to be a good upper limit for the highest energies.
  const double sigmaDeltaOmega = GetAxisError();
  const double angularSigma = sqrt(-1.0 * sigmaDeltaOmega * sigmaDeltaOmega /
                                   (2.0 * log(1.0 - 0.68)));
  // Smear azimuth and zenith by rotating the shower axis around two orthogonal
  // axes using two gaussian distributions
  TVector3 showerAxis = GetAxis();
  TVector3 axis1 = showerAxis.Orthogonal();
  TVector3 axis2 = showerAxis.Cross(axis1);
  // Rotate shower axis around the axes and set new pointing
  showerAxis.Rotate(random->Gaussian(angularSigma / rad), axis1);
  showerAxis.Rotate(random->Gaussian(angularSigma / rad), axis2);
  SetAxis(showerAxis);

  // Shower energy variation
  //
  // Smear energy using log-normal distribution
  const double energy = GetEnergy();
  const double energyResolution = GetRelativeEnergyError();
  // Calculate parameters for log-normal distribution
  double energyUncertainty = energyResolution * energy;
  double energyUncertainty2 = energyUncertainty * energyUncertainty;
  double energySigma = sqrt(log(1.0 + energyUncertainty2 / (energy * energy)));
  double energyMu = log(energy) - 0.5 * energySigma * energySigma;
  // Draw an energy and set it
  SetEnergy(random->LogNormal(energyMu, energySigma));
}

