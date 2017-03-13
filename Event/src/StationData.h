#ifndef __STATIONDATA_H__
#define __STATIONDATA_H__

#include <stdexcept>
#include <vector>

#include <TObject.h>


// Exception is thrown if StationData::GetSignalTriggerTime fails
class TriggerTimeNotFoundError : public std::exception {
  virtual const char* what() const throw() {
    return "Could not derive trigger time from station signal.";
  }
};


class StationData : public TObject
{
 public:
  StationData(unsigned int stationId = 0);

  // fId
  unsigned int GetId(void) const;
  void SetId(unsigned int id);

  // fHasTriggerTime / fTriggerTime
  bool HasTriggerTime(void) const;
  double GetTriggerTime(void) const;
  void SetTriggerTime(double t);

  // fHasCorrectedTriggerTime / fCorrectedTriggerTime
  bool HasCorrectedTriggerTime(void) const;
  double GetCorrectedTriggerTime(void) const;
  void SetCorrectedTriggerTime(double t);

  // fHasEnergyDeposit / fEnergyDeposit
  bool HasEnergyDeposit(void) const;
  double GetEnergyDeposit(void) const;
  void SetEnergyDeposit(double e);

  // fSignal
  bool HasSignal(void) const;
  const std::vector<double>& GetSignal(void) const;
  void SetSignal(std::vector<double> signal);

  // fSignalStartTime
  double GetSignalStartTime(void) const;
  void SetSignalStartTime(double t0);

  // fSignalBinWidth
  double GetSignalBinWidth(void) const;
  void SetSignalBinWidth(double dt);

  // fSignalTriggerTime
  double GetSignalTriggerTime(void) const throw(TriggerTimeNotFoundError,
                                                std::runtime_error);

  // fSimulatedSignalTriggerTime
  double GetSimulatedSignalTriggerTime(void) const;
  void SetSimulatedSignalTriggerTime(double t);


 private:
  // Finds the trigger time of the signal trace
  double FindSignalTriggerTime(void) const throw(TriggerTimeNotFoundError,
                                                 std::runtime_error);

  unsigned int fId;  // Station id, zero when not initialized

  bool fHasTriggerTime;  // Indicator for available trigger time
  double fTriggerTime;  // Trigger time from reconstruction
                        // (GRAT1 for Grande stations)

  bool fHasCorrectedTriggerTime;  // Indicator for available corrected trigger time
  double fCorrectedTriggerTime;  // Corrected trigger time to remove outliers

  bool fHasEnergyDeposit;  // Indicator for available energy deposit

  double fEnergyDeposit;  // Energy deposit from reconstruction
                          // (GRED for Grande stations)

  std::vector<double> fSignal;  // Digitized voltage trace

  double fSignalStartTime;  // Start time of the signal trace

  double fSignalBinWidth;  // Bin width (dt) of the signal trace

  mutable bool fHasCachedSignalTriggerTime;  //! Internal state indicating a
                                             //  cached trigger time

  mutable double fSignalTriggerTime;  //! Trigger time estimated from signal
                                      //  trace

  double fSimulatedSignalTriggerTime;  // Signal trigger time estimated from
                                       // simulation

  ClassDef(StationData, 2);  // Container for array station data

};

#endif
