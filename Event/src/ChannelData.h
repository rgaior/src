#ifndef __CHANNELDATA_H__
#define __CHANNELDATA_H__

#include <TObject.h>
#include <TVector3.h>
#include <iostream>
#include <vector>
class ChannelData : public TObject {

public:
  ChannelData(size_t antennaId = 0);




  // Channel id
  size_t GetId() const {return fChannelId;}
  void SetId(size_t id);

  //Real Data trace (D)
  bool HasRealDataTrace() const {return !fRealDataTrace.empty();}
  void MakeRealDataTrace(double startTime, double binWidth, size_t nBins);
  std::vector<Double_t>& GetRealDataTrace() {return fRealDataTrace;}
  const std::vector<Double_t>& GetRealDataTrace() const {return fRealDataTrace;}
  double GetRealDataBinWidth() const {return fRealDataBinWidth;}
  size_t GetRealDataBinCount() const {return fRealDataTrace.size();}
  double GetRealDataStartTime() const {return fRealDataStartTime;}
  void SetRealDataStartTime(double RealstartTime) {fRealDataStartTime = RealstartTime;}



  // Data trace (S+N)
  bool HasDataTrace() const {return !fDataTrace.empty();}
  void MakeDataTrace(double startTime, double binWidth, size_t nBins);
  std::vector<Double_t>& GetDataTrace() {return fDataTrace;}
  const std::vector<Double_t>& GetDataTrace() const {return fDataTrace;}
  double GetDataBinWidth() const {return fDataBinWidth;}
  size_t GetDataBinCount() const {return fDataTrace.size();}
  double GetDataStartTime() const {return fDataStartTime;}
  void SetDataStartTime(double startTime) {fDataStartTime = startTime;}

  // Simulation traces (S)
  bool HasSimulationTraces() const {return !fSimulatedFluxTrace.empty();}
  void MakeSimulationTraces(double startTime, double binWidth, size_t nBins);
  std::vector<Double_t>& GetSimulatedFluxTrace() {return fSimulatedFluxTrace;}
  std::vector<Double_t>& GetSimulatedPowerTrace() {return fSimulatedPowerTrace;}
  std::vector<Double_t>& GetMinimumFluxTrace() {return fMinimumFluxTrace;}
  const std::vector<Double_t>&
    GetSimulatedFluxTrace() const {return fSimulatedFluxTrace;}
  const std::vector<Double_t>&
    GetSimulatedPowerTrace() const {return fSimulatedPowerTrace;}
  const std::vector<Double_t>&
    GetMinimumFluxTrace() const {return fMinimumFluxTrace;}

  double GetSimulationBinWidth() const {return fSimulationBinWidth;}
  size_t GetSimulationBinCount() const {return fSimulatedFluxTrace.size();}
  double GetSimulationStartTime() const {return fSimulationStartTime;}

  void ObserveEmission(double energy,
                       double t1,
                       double t2,
                       const TVector3 r1,
                       const TVector3 r2);

  void ObserveEmission(double energy,
                       double t1,
                       const TVector3 r1,
		       double tau);

  void PrintAeff(const TVector3 r1);

  //double GetMinFlux() const ;//const {return fmin_flux;}

  double T_integred;
  //double min_flux;
  double fSurf;
  double GetTsky() const;
  double GetEffectiveSurf() const; 
  double CalculateSensitivity() const;
  
  double GetEffectiveArea(TVector3 r); 
  double GetTime(TVector3 r, double emissiontime); 
  void Dump(std::ostream& o = std::cout, bool power=true) const;
  void AddToAeffTrace(TVector3 r);
  void AddToTimeTrace(TVector3 r, double emissiontime);
 private:

  // Antenna and channel ids
  UInt_t fAntennaId;
  UInt_t fChannelId;

  // Real Data trace
  std::vector<Double_t> fRealDataTrace;  // digitised voltage
  Double_t fRealDataStartTime;
  Double_t fRealDataBinWidth;
 

  // Data trace
  std::vector<Double_t> fDataTrace;  // #FADC
  Double_t fDataStartTime;
  Double_t fDataBinWidth;

  // Simulation traces
  std::vector<Double_t> fSimulatedFluxTrace; // spectral irradiance at receiver
  std::vector<Double_t> fSimulatedPowerTrace;  // received power
  Double_t fSimulationStartTime;
  Double_t fSimulationBinWidth;
  std::vector<Double_t> fMinimumFluxTrace;   
  //Double_t fmin_flux;
  
  void AddToSimulatedFluxTrace(int bin, double flux);
  void AddToSimulatedPowerTrace(int bin, double power);
  void AddToMinimumFluxTrace(int bin, double fmin_flux);

  std::vector<double> fAeffTrace;
  std::vector<double> fTimeTrace;
  ClassDef(ChannelData, 2);

};

#endif
