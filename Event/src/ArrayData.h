#ifndef _ArrayData_h_
#define _ArrayData_h_

#include <map>
#include <utility>

#include <TObject.h>
#include <Shower.h>

#include "StationData.h"


class ArrayData : public TObject {

public:

  enum EArrayType {
    eUndefined,
    eKASCADEGrande,
    eAuger
  };

  ArrayData();

  /// data type
  EArrayType GetType() const { return fType; }
  void SetType(EArrayType type) { fType = type; }

  /// event id of array trigger
  UInt_t GetEventId() const { return fEventId;}
  void SetEventId(UInt_t id) { fEventId = id;}

  /// run id of array trigger
  UInt_t GetRunId() const { return fRunId;}
  void SetRunId(UInt_t id) { fRunId = id;}

  /// event time of array trigger
  UInt_t GetGPSSecond() const { return fGPSSecond; }
  void SetGPSSecond(UInt_t val) { fGPSSecond=val; }
  UInt_t GetGPSNanoSecond() const { return fGPSNanoSecond; }
  void SetGPSNanoSecond(UInt_t val) { fGPSNanoSecond=val; }

  /// get shower
  const Shower& GetShower() const { return fShower; }
  Shower& GetShower() { return fShower; }
  /// set shower
  void SetShower(const Shower& shower) { fShower = shower; }

  /// trigger flag 1 (KG: Hit7)
  UInt_t GetTriggerFlag1() const { return fTriggerFlag1; }
  void SetTriggerFlag1(int val) { fTriggerFlag1=val; }
  /// trigger flag 2 (KG: Fanka)
  UInt_t GetTriggerFlag2() const { return fTriggerFlag2; }
  void SetTriggerFlag2(int val) { fTriggerFlag2=val; }
  /// trigger flag 3 (KG: Itrg)
  UInt_t GetTriggerFlag3() const { return fTriggerFlag3; }
  void SetTriggerFlag3(int val) { fTriggerFlag3=val; }
  /// trigger flag 4 (KG: Trgg)
  UInt_t GetTriggerFlag4() const { return fTriggerFlag4; }
  void SetTriggerFlag4(int val) { fTriggerFlag4=val; }
  /// DAQ flag 4 (KG: Iact)
  UInt_t GetDaqFlag1() const { return fDaqFlag1; }
  void SetDaqFlag1(int val) { fDaqFlag1=val; }

  /// fStationData
  bool HasStationData(int stationId) const;
  StationData& GetStationData(int stationId);
  const StationData& GetStationData(int stationId) const;
  void AddStationData(StationData stationData);

  /// ground impact time in Grande grat time reference [ns]
  Double_t GetGroundImpactTimeKG() const;
  std::pair<int,double> GetGroundImpactTimeKG(int hotStationIt, bool corrected = false) const;
  /// ground impact time in Crome time relative to trigger signal [ns]
  /// needs a reference station that is digitized by the Crome DAQ
  Double_t GetGroundImpactTimeCrome(int stationId) const;

  /// Calculate corrected trigger times for Grande stations with a distance to
  /// the shower axis of less than 300m. Ground impact times are calculated for
  /// all stations and times are corrected using a linear fit.
  void CalculateCorrectedTriggerTimes();
  void CalculateCorrectedTriggerTimes2();

private:

  EArrayType fType;

  UInt_t fEventId;
  UInt_t fRunId;
  UInt_t fGPSSecond;
  UInt_t fGPSNanoSecond;
  UInt_t fTriggerFlag1;
  UInt_t fTriggerFlag2;
  UInt_t fTriggerFlag3;
  UInt_t fTriggerFlag4;
  UInt_t fDaqFlag1;

  std::map<int, StationData> fStationData;
  typedef std::map<int, StationData>::iterator StationDataIterator;
  typedef std::map<int, StationData>::const_iterator ConstStationDataIterator;

  static bool comparePair(const std::pair<int, double>& i, const std::pair<int, double>& j);

  Shower fShower;

  ClassDef(ArrayData, 1);

};

#endif
