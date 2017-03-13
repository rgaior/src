#include "StationData.h"

#include <AugerUnits.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <Station.h>

using namespace std;
using namespace utl;


ClassImp(StationData)


StationData::StationData(unsigned int stationId) :
  fId(stationId),
  fHasTriggerTime(false),
  fTriggerTime(0.0),
  fHasCorrectedTriggerTime(false),
  fCorrectedTriggerTime(0.0),
  fHasEnergyDeposit(false),
  fEnergyDeposit(0.0),
  fSignalStartTime(0.0),
  fSignalBinWidth(0.0),
  fHasCachedSignalTriggerTime(false),
  fSignalTriggerTime(0.0),
  fSimulatedSignalTriggerTime(0.0)
{

}


unsigned int StationData::GetId(void) const
{
  return fId;
}


void StationData::SetId(unsigned int id)
{
  fId = id;
}


bool StationData::HasTriggerTime(void) const
{
  return fHasTriggerTime;
}


double StationData::GetTriggerTime(void) const
{
  if (!HasTriggerTime()) {
    throw runtime_error("StationData::GetTriggerTime(): "
                        "Trigger time is not available.");
  }

  return fTriggerTime;
}


void StationData::SetTriggerTime(double t)
{
  fTriggerTime = t;
  fHasTriggerTime = true;
}


bool StationData::HasCorrectedTriggerTime(void) const
{
  return fHasCorrectedTriggerTime;
}


double StationData::GetCorrectedTriggerTime(void) const
{
  if (!HasCorrectedTriggerTime()) {
    throw runtime_error("StationData::GetCorrectedTriggerTime(): "
                        "Corrected trigger time is not available.");
  }

  return fCorrectedTriggerTime;
}


void StationData::SetCorrectedTriggerTime(double t)
{
  fCorrectedTriggerTime = t;
  fHasCorrectedTriggerTime = true;
}


bool StationData::HasEnergyDeposit(void) const
{
  return fHasEnergyDeposit;
}


double StationData::GetEnergyDeposit(void) const
{
  if (!HasEnergyDeposit()) {
    throw runtime_error("StationData::GetEnergyDeposit(): "
                        "Energy deposit is not available.");
  }

  return fEnergyDeposit;
}


void StationData::SetEnergyDeposit(double e)
{
  fEnergyDeposit = e;
  fHasEnergyDeposit = true;
}


bool StationData::HasSignal(void) const
{
  return !fSignal.empty();
}


const vector<double>& StationData::GetSignal(void) const
{
  return fSignal;
}


void StationData::SetSignal(vector<double> signal)
{
  fSignal = signal;
}


double StationData::GetSignalStartTime(void) const
{
  return fSignalStartTime;
}


void StationData::SetSignalStartTime(double t0)
{
  fSignalStartTime = t0;
}


double StationData::GetSignalBinWidth(void) const
{
  return fSignalBinWidth;
}


void StationData::SetSignalBinWidth(double dt)
{
  fSignalBinWidth = dt;
}


double StationData::GetSignalTriggerTime(void) const
  throw(TriggerTimeNotFoundError, runtime_error)
{
  if (fHasCachedSignalTriggerTime)
    return fSignalTriggerTime;

  // Calculate and set trigger time
  fSignalTriggerTime = FindSignalTriggerTime();
  fHasCachedSignalTriggerTime = true;

  return fSignalTriggerTime;
}


double StationData::GetSimulatedSignalTriggerTime(void) const
{
  return fSimulatedSignalTriggerTime;
}


void StationData::SetSimulatedSignalTriggerTime(double t)
{
  fSimulatedSignalTriggerTime = t;
}


double StationData::FindSignalTriggerTime(void) const
  throw(TriggerTimeNotFoundError, runtime_error)
{
  if (fSignal.empty()) {
    throw runtime_error("StationData::FindSignalTriggerTime(): "
                        "No signal data available.");
  }

  // Get a reasonable time interval to search for the trigger signal
  const Station& station =
    Detector::GetInstance().GetSetup().GetArray().GetStation(fId);
  const double minTime = station.GetTriggerTimeRange().first;
  const double maxTime = station.GetTriggerTimeRange().second;
  const double triggerTimeOffset = station.GetTriggerTimeOffset();

  // Define a voltage interval as trigger condition
  const double satVoltage = -49.5 * milli * volt;

  const size_t nSamples = fSignal.size();
  double time;
  for (size_t i = 0; i < nSamples; ++i) {
    time = fSignalStartTime + i * fSignalBinWidth;
    if (time < minTime)
      continue;
    if (time > maxTime)
      break;

    // Check whether
    //  * this voltage sample is below the threshold,
    //  * the following voltage sample is below the threshold, and
    //  * the previous sample is higher than the current one.
    if ( (fSignal[i] < satVoltage) && (fSignal[i+1] < satVoltage) &&
         (fSignal[i] < fSignal[i-1]) ) {
      // Adjust for the slope of the signal depending on the electronics
      return (time - triggerTimeOffset);
    }
  }

  // Could not find the trigger signal
  throw TriggerTimeNotFoundError();
}

