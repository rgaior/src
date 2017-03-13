#include "BackgroundSimulator.h"
#include <Math/GSLRndmEngines.h>
#include <Detector.h>
#include <RandomGenerator.h>
#include <DetectorSetup.h>
#include <AugerUnits.h>
#include <Atmosphere.h>
#include <PhysicalConstants.h>
#include <Event.h>
#include <vector>
#include <iostream>
#include <cmath>


using namespace std;
using namespace utl;

BackgroundSimulator::BackgroundSimulator():
  TimedClass("BackgroundSimulator"),
  fNoiseFluctuation(0)
{

  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());

}

void 
BackgroundSimulator::Run(Event& event)
{
  StartTimer();
  const Detector& detector = Detector::GetInstance();
  const Atmosphere& atmo  = Atmosphere::GetInstance();
  const std::vector<Antenna> Antennas = detector.GetSetup().GetAntennas();
  
  for(unsigned int iAntenna=0; iAntenna<Antennas.size(); iAntenna++) {
    const Antenna& antenna = Antennas[iAntenna];
    const unsigned int antennaId = antenna.GetId();
    if ( event.HasAntennaData(antennaId) ) {
      AntennaData& antennaData = event.GetAntennaData(antennaId);
      const std::vector<Channel>& Channels = antenna.GetChannels();
      
      for(unsigned int iCh=0; iCh<Channels.size(); iCh++) {
        const Channel& channel = Channels[iCh];
        const unsigned int channelId = channel.GetId();
        if ( antennaData.HasChannelData(channelId) ) {
          ChannelData& chanData = antennaData.GetChannelData(channelId);
          if ( chanData.HasSimulationTraces() ) {
            if( !chanData.HasDataTrace() )
              chanData.MakeDataTrace(chanData.GetSimulationStartTime(),
                                     chanData.GetSimulationBinWidth(),
                                     chanData.GetSimulationBinCount());

	    
	    const double solidAngle = 2.*kPi*(1. - cos(channel.GetBeamWidth()/2.));       
	    const double wavelength = kSpeedOfLight/channel.GetCenterFrequency();
            const double dishEffArea = channel.GetBoresightEffectiveArea();
	    const double antennaFactor = dishEffArea * solidAngle/(4.*kPi*wavelength*wavelength);
	    const double ch_elevation = kPi/2. - channel.GetGlobalPosition().Theta();
	    
            const double skyTemp =chanData.GetTsky(); // For gorham, otherwise skyTemp = chanData.GetTsky();
	    // Modification on 18/08/2015
            //const double Noise = kBoltzmann * channel.GetBandWidth() * (skyTemp+channel.GetNoise()) ;//* antennaFactor / dishEffArea;
	    const double Noise = kBoltzmann * channel.GetBandWidth() * skyTemp; // At the entry level of the antenna, only sky noise. 
            //cout << "Channel "<<channelId <<": Sky Noise power [w] :" << Noise/watt << endl; //" antenna factor "<<antennaFactor<< endl;
            std::vector<double>& trace = chanData.GetDataTrace();
            for(unsigned int bin = 0; bin <  trace.size(); bin++) {
              trace[bin] = Noise/watt ; 
            }
            // get minimal flux 
            //const double S = chanData.CalculateSensitivity(); // UNCOMMENT for real setup
           
          }
        }
      }
    }	  
  }
  
  StopTimer();

}
  
 
