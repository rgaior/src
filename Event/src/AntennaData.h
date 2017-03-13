#ifndef __AntennaData_H
#define __AntennaData_H

#include <list>
#include <vector>

#include <TObject.h>

#include "ChannelData.h"


class AntennaData : public TObject {

public:

  AntennaData();


  /// get antenna Id
  UInt_t GetId() const { return fAntennaId;}
  /// set antenna Id
  void SetId(UInt_t id);

  /// channel data
  typedef std::list<ChannelData>::iterator ChannelDataIterator;
  typedef std::list<ChannelData>::const_iterator ConstChannelDataIterator;

  ChannelDataIterator ChannelsBegin() { return fChannels.begin();}
  ChannelDataIterator ChannelsEnd() { return fChannels.end();}
  ConstChannelDataIterator ChannelsBegin() const { return fChannels.begin();}
  ConstChannelDataIterator ChannelsEnd() const { return fChannels.end();}

  bool HasChannelData(unsigned int channelId) const;
  void MakeChannelData(unsigned int channelId);
  void AddChannelData(const ChannelData&);
  const ChannelData& GetChannelData(unsigned int channelId) const;
  ChannelData& GetChannelData(unsigned int channelId);

  void ObserveEmission(const double energy,
                       const double t1,
                       const double t2,
                       const TVector3 r1,
                       const TVector3 r2);

  void ObserveEmissions(const double energy,
                        const double t1,
                        const TVector3 r1,
                        const double tau);
			//const std::vector<double> dts);

  void PrintAeff(const TVector3 r1);

private:

  UInt_t fAntennaId;
  std::list<ChannelData> fChannels;
  ClassDef(AntennaData, 1);
};

#endif
