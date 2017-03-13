// $Id: PhysicalConstants.h 150 2010-09-14 19:59:06Z munger $

//
// Define physical constants XS
//

#ifndef _utl_PhysicalConstants_h_
#define _utl_PhysicalConstants_h_

#include "AugerUnits.h"
#include "MathConstants.h"


namespace utl {

  /**
     \file PhysicalConstants.h

     \brief physical constants

     \ingroup units
  */

  const double kgEarth = 9.81 * m/(s*s);
  const double kRadiationLength = 37 * g/cm2;
  //const double kIonizationPotential = 35.8 * electronvolt; // valeur utilisee pour l'arXiv v1
  const double kIonizationPotential = 15.6 * electronvolt;
  const double kMeanKineticEnergy = 40 * electronvolt;

  // grammage at sea level
  const double kOverburdenSeaLevel = 1033 * g/cm2;
  // refraction index sea level
  const double kRefractiveIndexSeaLevel = 1.000292;

  // A few gas and liquid properties

  const double kMolarGasConstant =  8.3145 * joule/(mole*kelvin); // R: NIST
  const double kAvogadro = 6.022142e23 / mole;                    // Na: NIST
  const double kBoltzmann = kMolarGasConstant / kAvogadro;        // kB = R/Na

  const double kDryAirMolarMass = 28.97 * gram/mole;  // M. Note: R_spec = R/M
  const double kN2MolarMass = 28.0134 * gram/mole;
  const double kO2MolarMass = 31.9989 * gram/mole;
  const double kArMolarMass = 39.9481 * gram/mole;
  const double kCO2MolarMass = 44.0096 * gram/mole;
  const double kH2OMolarMass = 18.0153 * gram/mole;

  const double kN2AirFraction = 780840 * perMillion;  // Dry air vol. fractions;
  const double kO2AirFraction = 209460 * perMillion;  // NASA Earth Fact Sheet.
  const double kArAirFraction =   9340 * perMillion;  // H2O vapor @ surface is
  const double kCO2AirFraction =   380 * perMillion;  // ~10 000 ppm.

  const double kH2OFreezingPoint = 273.15 * kelvin;

  // All taken from PDG data tables (2002)

  // Gaisser-Hillas parameter for the electron mean free path in air
  const double kLambdaGH = 70 * g/cm2;

  // Constants

  const double kSpeedOfLightSI = 299792458;
  const double kSpeedOfLight = kSpeedOfLightSI * m/s;
  const double kSpeedOfLight2 = kSpeedOfLight * kSpeedOfLight;
  const double kPlanckSI = 6.62606876e-34;
  const double kPlanckReducedSI = kPlanckSI / (2*kPi);
  const double kPlanck = kPlanckSI * joule*s;
  const double kPlanckReduced = kPlanckReducedSI * joule*s;
  const double kMuZeroSI = 4*kPi * 1e-7;
  const double kMuZero = kMuZeroSI*newton/(ampere*ampere); 

  // Particle masses

  const double kMassConversionSI = eSI / (kSpeedOfLightSI*kSpeedOfLightSI);

  const double kElectronMass = 0.510998902 * MeV;
  const double kElectronMassSI = kElectronMass * kMassConversionSI;
  const double kMuonMass = 105.658357 * MeV;
  const double kMuonMassSI = kMuonMass * kMassConversionSI;
  const double kTauMass = 1776.99 * MeV;
  const double kTauMassSI = kTauMass * kMassConversionSI;

  const double kProtonMass = 938.271998 * MeV;
  const double kProtonMassSI = kProtonMass * kMassConversionSI;
  const double kNeutronMass = 939.56533 * MeV;
  const double kNeutronMassSI = kNeutronMass * kMassConversionSI;
  const double kDeuteronMass = 1875.612762 * MeV;
  const double kDeuteronMassSI = kDeuteronMass*kMassConversionSI;

  const double kLambdaMass = 1115.683 * MeV;
  const double kLambdaMassSI = kLambdaMass * kMassConversionSI;
  const double kLambdacMass = 2286.46 * MeV;
  const double kLambdacMassSI = kLambdacMass * kMassConversionSI;
  const double kSigmaZeroMass = 1192.642 * MeV;
  const double kSigmaZeroMassSI = kSigmaZeroMass * kMassConversionSI;
  const double kSigmaPlusMass = 1189.37 * MeV;
  const double kSigmaPlusMassSI = kSigmaPlusMass * kMassConversionSI;
  const double kSigmaMinusMass = 1197.449 * MeV;
  const double kSigmaMinusMassSI = kSigmaMinusMass * kMassConversionSI;
  const double kXiZeroMass = 1314.83 * MeV;
  const double kXiZeroMassSI = kXiZeroMass * kMassConversionSI;
  const double kXiMinusMass = 1321.31 * MeV;
  const double kXiMinusMassSI = kXiMinusMass * kMassConversionSI;
  const double kOmegaMinusMass = 1672.45 * MeV;
  const double kOmegaMinusMassSI = kOmegaMinusMass * kMassConversionSI;

  const double kPiZeroMass = 134.9766 * MeV;
  const double kPiZeroMassSI = kPiZeroMass * kMassConversionSI;
  const double kPiChargedMass = 139.57018 * MeV;
  const double kPiChargedMassSI = kPiChargedMass * kMassConversionSI;
  const double kKaonZeroMass = 497.672 * MeV; // | K^0 _s - K^0 _l | ~ 10^-12 MeV
  const double kKaonZeroMassSI = kKaonZeroMass * kMassConversionSI;
  const double kKaonChargedMass = 493.677 * MeV;
  const double kKaonChargedMassSI = kKaonChargedMass * kMassConversionSI;
  const double kEtaMass = 547.30 * MeV;
  const double kEtaMassSI = kEtaMass * kMassConversionSI;

  // Particle lifetimes

  const double kMuonLifetime = 2.19703e-6 * s;

  const double kNeutronLifetime = 885.7 * s;

  const double kLambdaLifetime = 2.632e-10 * s;
  const double kSigmaZeroLifetime = 7.4e-20 * s;
  const double kSigmaPlusLifetime = 0.8018e-10 * s;
  const double kSigmaMinusLifetime = 1.479e-10 * s;
  const double kXiZeroLifetime = 2.9e-10 * s;
  const double kXiMinusLifetime = 1.639e-10 * s;
  const double kOmegaMinusLifetime = 0.821e-10 * s;

  const double kPiZeroLifetime = 8.4e-17 * s;
  const double kPiChargedLifetime = 2.6033e-8 * s;
  const double kKaonZeroShortLifetime = 0.8934e-10 * s;
  const double kKaonZeroLongLifetime = 5.17e-8 * s;
  const double kKaonChargedLifetime = 1.2384e-8 * s;

  // Derived constants

  const double kEpsilonZeroSI = 1 / (kMuZeroSI * kSpeedOfLightSI);
  const double kAlpha = (eSI*eSI) /
    (4*kPi * kEpsilonZeroSI * kPlanckReducedSI * kSpeedOfLightSI);
  const double kElectronRadiusSI = (eSI*eSI) /
    (4*kPi * kEpsilonZeroSI * kElectronMassSI * kSpeedOfLightSI * kSpeedOfLightSI);
  const double kThomsonCrossSectionSI =
    8*kPi * kElectronRadiusSI * kElectronRadiusSI / 3;

//  const double kFreeFreeCR = 5e-30 * m2; // valeur utilisee pour l'arXiv v1
//  const double kFreeFreeCR = 3.5e-30 * m2; //valeur utilisee pour Astrop.Phys.
    const double kFreeFreeCR = 1.7e-28 * m2; 
}

#endif
