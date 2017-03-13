#ifndef __Event_H
#define __Event_H

#include <TObject.h>

#include <AntennaData.h>
#include <ArrayData.h>
#include <SimData.h>

#include <list>
#include <vector>
#include <utility>

class Shower;
class Channel;

struct SensitivityTrace {
  std::pair< std::vector<double>, std::vector<double> > trace;
  std::pair< std::vector<double>, std::vector<double> > trace_min;
  std::pair< std::vector<double>, std::vector<double> > trace_max;
};


class Event : public TObject {

public:

  Event();

  /// IDs
  long long GetEventId() const { return fEventId; }
  void SetEventId(long long id) { fEventId = id; }
  UInt_t GetRunId() const { return fRunId; }
  void SetRunId(UInt_t id) { fRunId = id; }

  /// timing
  UInt_t GetGPSSecond() const { return fGPSSecond; }
  void SetGPSSecond(UInt_t val) { fGPSSecond=val; }
  UInt_t GetGPSNanoSecond() const { return fGPSNanoSecond; }
  void SetGPSNanoSecond(UInt_t val) { fGPSNanoSecond=val; }
  Double_t GetTriggerTimeDelay() const { return fTriggerTimeDelay; }
  void SetTriggerTimeDelay(Double_t val) { fTriggerTimeDelay=val; }

  /// get array data
  const ArrayData& GetArrayData() const { return fArrayData; }
  ArrayData& GetArrayData() { return fArrayData; }

  /// get simulated data
  const SimData& GetSimData() const { return fSimData; }
  SimData& GetSimData() { return fSimData; }

  /// antenna data
  typedef std::list<AntennaData>::iterator AntennaDataIterator;
  typedef std::list<AntennaData>::const_iterator ConstAntennaDataIterator;

  double GetNumberOfAntennas() { return fAntennas.size(); }
  AntennaDataIterator AntennasBegin() { return fAntennas.begin(); }
  AntennaDataIterator AntennasEnd() { return fAntennas.end(); }
  ConstAntennaDataIterator AntennasBegin() const { return fAntennas.begin(); }
  ConstAntennaDataIterator AntennasEnd() const { return fAntennas.end(); }

  bool HasAntennaData(unsigned int antennaId) const;
  void MakeAntennaData(unsigned int antennaId);
  void AddAntennaData(const AntennaData&);
  const AntennaData& GetAntennaData(unsigned int antennaId) const;
  AntennaData& GetAntennaData(unsigned int antennaId);
  bool HasSimData() const;

  /// get vector containing the start and end time of the expected signal for
  /// a given antenna id, channel id and acceptance angle
  /// Should be moved to ChannelData once the channel knows its event.
  std::vector<double> GetExpectedSignalWindow(unsigned int antennaId,
                                              unsigned int channelId,
                                              double coneAngle,
                                              bool includeErrors = true)
    const throw(TriggerTimeNotFoundError);

  SensitivityTrace GetSensitivityTrace(unsigned int antennaId,
                                       unsigned int channelId,
                                       double coneAngle,
                                       double stepSize,
                                       bool includeErrors = true)
    const;
    
    SensitivityTrace GetSensitivityTraceWithN(unsigned int antennaId,
                                         unsigned int channelId,
                                         double coneAngle,
                                         double stepSize,
                                         bool includeErrors = true)
    const;
    

    void Dump(std::ostream& o = std::cout) const;

private:

  long long fEventId;
  UInt_t fRunId;
  UInt_t fGPSSecond;
  UInt_t fGPSNanoSecond;
  Double_t fTriggerTimeDelay;

  SimData fSimData;
  ArrayData fArrayData;
  std::list<AntennaData> fAntennas;

  std::vector<double> CalculateExpectedWindow(Shower shower,
                                              Channel channel,
                                              double coneAngle,
                                              double groundImpactTime)
    const;

  std::pair< std::vector<double>, std::vector<double> >
    CalculateSensitivityTrace(Shower shower,
                              Channel channel,
                              double coneAngle,
                              double groundImpactTime,
                              double stepSize)
    const;
    
    std::pair< std::vector<double>, std::vector<double> >
    CalculateSensitivityTraceWithN(Shower shower,
                              Channel channel,
                              double coneAngle,
                              double groundImpactTime,
                              double stepSize)
    const;
    

  ClassDef(Event, 1);
};

#endif
