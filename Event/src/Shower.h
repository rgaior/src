#ifndef __Shower_H
#define __Shower_H

#include <TObject.h>
#include <TVector3.h>
#include <iostream>
#include <vector>
//
//  Shower data definition
//

class Shower : public TObject {

public:
  Shower();
  virtual ~Shower() { }

  /// total energy [eV]
  Double_t GetEnergy() const { return fEnergy; }
  Double_t GetRelativeEnergyError() const { return fEnergyError; }
  void SetEnergy(const double e) { fEnergy = e; }
  bool HasEnergy() const { return fEnergy > 0.; }

  /// Gaisser Hillas parameters
  Double_t GetXmax() const { return fXmax; }
  void SetXmax(const double xmax) { fXmax = xmax; }
  Double_t GetNmax() const { return fNmax; }
  void SetNmax(const double nmax) { fNmax = nmax; }
  Double_t GetLambda() const { return fLambda; }
  void SetLambda(const double lambda) { fLambda = lambda; }
  Double_t GetX0() const { return fX0; }
  void SetX0(const double x0) { fX0 = x0; }
  Double_t GetX1() const { return fX1; }
  void SetX1(const double x1) { fX1 = x1; }

  /// LDF parameters
  Double_t GetAge() const { return fAge; }
  void SetAge(double val) { fAge = val; }
  Double_t GetElectronSize() const { return fElectronSize; }
  void SetElectronSize(double val) { fElectronSize = val; }
  Double_t GetMuonSize() const { return fMuonSize; }
  void SetMuonSize(double val) { fMuonSize = val; }

  /// shower core [m]
  const TVector3& GetCore() const { return fCore; }
  void SetCore(const TVector3& core) { fCore = core; }
  Double_t GetCoreError() const { return fCoreError; }
  void SetCoreError(double val) { fCoreError = val; }

  /// shower axis
  const TVector3& GetAxis() const { return fAxis; }
  void SetAxis(const TVector3& axis) { fAxis = axis; }
  Double_t GetAxisError() const { return fAxisError; }
  void SetAxisError(double val) { fAxisError = val; }

  /// core GPS time
  UInt_t GetCoreTimeSecond() const { return fCoreTimeSecond; }
  UInt_t GetCoreTimeNanoSecond() const { return fCoreTimeNanoSecond; }
  void SetCoreTime(const int sec, const int nsec)
       { fCoreTimeSecond = sec; fCoreTimeNanoSecond = nsec; }

  /// longitudinal profile
  std::vector<Double_t>& GetEnergyDeposit() { return fdEdX; }
  std::vector<Double_t>& GetElectrons() { return fElectrons; }
  std::vector<Double_t>& GetDepth()  { return fDepth; }
/*   void SetHeight(); */
/*   std::vector<Double_t>& GetHeight() ; */
  // { return fHeight; }
  const std::vector<Double_t>& GetEnergyDeposit() const { return fdEdX; }
  const std::vector<Double_t>& GetElectrons() const { return fElectrons; }
  const std::vector<Double_t>& GetDepth() const { return fDepth; }
  //  const std::vector<Double_t>& GetHeight() const { return fHeight; }

  /// Fluctuate Shower acoording to Grande uncertainties
  void Fluctuate();

  /// zenith angle  [rad]
  Double_t GetZenith() const;
  /// cos(zenith angle)
  Double_t GetCosZenith() const;
  /// azimuth angle
  Double_t GetAzimuth() const;

  void DumpASCII(std::ostream& o = std::cout) const;
  void Dump(std::ostream& o = std::cout) const;

  void AddToPositionTrace(TVector3 pos){fPosition.push_back(pos);}
private:
  Double_t fEnergy;
  Double_t fEnergyError;
  Double_t fXmax;
  Double_t fNmax;
  Double_t fX0;
  Double_t fX1;
  Double_t fLambda;

  Double_t fAge;
  Double_t fElectronSize;
  Double_t fMuonSize;

  UInt_t fCoreTimeSecond;
  UInt_t fCoreTimeNanoSecond;
  TVector3 fCore;
  Double_t fCoreError;
  TVector3 fAxis;
  Double_t fAxisError;

  std::vector<Double_t> fDepth;
  std::vector<Double_t> fElectrons;
  std::vector<Double_t> fdEdX;
  std::vector<TVector3> fPosition;
  //  std::vector<Double_t> fHeight;

  ClassDef(Shower, 2);
};

#endif
