#include "TraceSimulator.h"
#include <TRandom.h>
#include <Shower.h>
#include <Event.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <RandomGenerator.h>
#include <Atmosphere.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <tinyxml.h>
#include <tinyxmlCromeUtilities.h>
#include <Configuration.h>
#include <Math/GSLRndmEngines.h>


using namespace std;
using namespace utl;


TraceSimulator::TraceSimulator():
  fLDFmodel(eNone),
  fThinning(1),
  fMWyield(0),
  fTau(0),
  fNsample(1)
{
  const string configId("TraceSimulator");
  const string& fileName = Configuration::GetInstance().GetConfigurationFileName(configId);

  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());

  TiXmlDocument doc(fileName);
  if ( doc.LoadFile() ) {

    const TiXmlElement* topBranch = doc.FirstChildElement(configId);
    if (!topBranch ) {
      throw std::runtime_error(string("TraceSimulator::Init(): ")+
                               string("could not find TraceSimulation in ")+
                               fileName);
    }

    const TiXmlElement* showerBranch = topBranch->FirstChildElement("Shower");
    if (!showerBranch )
      throw std::runtime_error(string("TraceSimulator::Init(): ")+
                               string("could not find Shower branch in ")+
                               fileName);

    fLDFmodel = (LDFType)GetDoubleValue(showerBranch->FirstChildElement("LDF"));
    fThinning = GetDoubleValue(showerBranch->FirstChildElement("thinning"));
    //if(fLDFmodel==eNone) fThinning = 1;
    

    const TiXmlElement* MWmodelBranch = topBranch->FirstChildElement("MWmodel");
    if (!MWmodelBranch )
      throw std::runtime_error(string("TraceSimulator::Init(): ")+
                               string("could not find MWmodel branch in ")+
                               fileName);

    fMWyield =
      GetDoubleValue(MWmodelBranch->FirstChildElement("MWyield"))*(1./Hz);
    fTau = GetDoubleValue(MWmodelBranch->FirstChildElement("tau"))*s;
    fNsample = GetUnsignedValue(MWmodelBranch->FirstChildElement("Nsample"));

  }


  fDetectorSetup = &(Detector::GetInstance().GetSetup());

}


TraceSimulator::TraceSimulator(const DetectorSetup* detSetup):
  fLDFmodel(eGora),
  fThinning(1000), //300
  fMWyield(1.17253e-18*(1/Hz)),
  fTau(7e-9 * s),
  fNsample(20),
  fDetectorSetup(detSetup)
{

  cerr << " ---------------------------------------------------"  << endl;
  cerr << "|                                                   |" << endl;
  cerr << "|  WARNING: Using hardcoded simulation parameters!  |" << endl;
  cerr << "|                                                   |" << endl;
  cerr << " ---------------------------------------------------"  << endl;

  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());
  //fDetectorSetup = &(Detector::GetInstance().GetSetup());

}


void
TraceSimulator::Run(Event& event)
{

  //cout << "------------------------- fLDFmodel -> " <<  fLDFmodel << endl;
  Shower& shower = event.GetSimData().GetShower();
  const double arrayHeight = fDetectorSetup->GetArray().GetHeight();

  const vector<double>& depth = shower.GetDepth();
  const vector<double>& electrons = shower.GetElectrons();
  const vector<double>& energyDeposit = shower.GetEnergyDeposit();

  const double zenith = shower.GetZenith();
  const double azimuth = shower.GetAzimuth();
  const double cosZenith = cos(zenith);

  const TVector3& showerCore = shower.GetCore();
  const TVector3& showerAxis = shower.GetAxis();

  // prepare simulation traces
  for (Event::AntennaDataIterator antennaData = event.AntennasBegin();
       antennaData != event.AntennasEnd(); ++antennaData) {
    for (AntennaData::ChannelDataIterator channelData = antennaData->ChannelsBegin();
         channelData != antennaData->ChannelsEnd(); ++channelData) {
      if (!(*channelData).HasSimulationTraces()) {
        const size_t antennaId = antennaData->GetId();
        const Antenna& antenna = fDetectorSetup->GetAntenna(antennaId);
        const size_t channelId = channelData->GetId();
        const Channel& channel = antenna.GetChannel(channelId);
        channelData->MakeSimulationTraces(channel.GetTraceStartTime(),
                                          channel.GetTraceBinWidth(),
                                          channel.GetNTraceBins());
      }
    }
  }

  // holds decay times (see inner loop)
  vector<double> decayTimes(fNsample);

  // transformation from local to global CS
  TRotation localToGlobal;
  localToGlobal.RotateY(zenith);
  localToGlobal.RotateZ(azimuth);

  const Atmosphere& atmo = Atmosphere::GetInstance();
  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());
  TVector3 ldfPos;
  
  //cout << " depth [g/cm2]     altitude [m]    Ionization Electrons [m^-1]     Nm [m^-3]    dE/dX [g/m3]" << endl;
  for (int iDepth = 0; iDepth < (int) depth.size() - 1; iDepth++) { 
    const double depthStep = depth[iDepth + 1] - depth[iDepth]; 
    const double prevH = atmo.Height(depth[iDepth] - depthStep / 2., cosZenith);
    const double nextH = atmo.Height(depth[iDepth] + depthStep / 2., cosZenith);
    const double hstep = (prevH - nextH) * cosZenith;
    const double nLDFpoints = fmin(fThinning, electrons[iDepth]);
    const double ionizationEl =  energyDeposit[iDepth] * atmo.Density(atmo.Height(depth[iDepth], cosZenith)) / (kIonizationPotential+kMeanKineticEnergy); 

    const double averageDepth = depth[iDepth] + depthStep / 2.;
    const double rMoliereInBinCenter = atmo.MoliereRadius(averageDepth, cosZenith);
    //const double diskVolume = kPi * hstep * 2.*rMoliereInBinCenter * 2.*rMoliereInBinCenter;
    const double totIonizationEl = ionizationEl* hstep;// / diskVolume;
    
    if (iDepth%100==0) cout << " "<< iDepth << "/" << depth.size() << " --> depth = "
    			   << depth[iDepth] / (g/cm2) << " height = " << atmo.Height(depth[iDepth], cosZenith)/m 
    			   << " electrons = " << electrons[iDepth]  << endl;
    
    double totalRecovered = 0;
    
    
    const double currentDepth = depth[iDepth] + (((double) rand() / (RAND_MAX)) -0.5) * depthStep / 2.;
    const double currentHeight = atmo.Height(currentDepth, cosZenith);
    const double currentDistToCore = (currentHeight - arrayHeight) / cosZenith;
    const TVector3 currentPos = showerCore + currentDistToCore * showerAxis;
    
    shower.AddToPositionTrace(currentPos);
    const double EmTime = -currentDistToCore / kSpeedOfLight;
    for (Event::AntennaDataIterator antennaData = event.AntennasBegin(); antennaData != event.AntennasEnd(); ++antennaData) {
      for ( AntennaData::ChannelDataIterator chanIter = antennaData->ChannelsBegin(); chanIter != antennaData->ChannelsEnd(); ++chanIter )
	{
	  ChannelData& channelData = *chanIter;
	  channelData.AddToAeffTrace(currentPos);
	  channelData.AddToTimeTrace(currentPos,EmTime);
	  //	  channelData.AddToDistTrace(currentPos);
	} // for each channel (there is just one for easier)
    } // for each antenna
    int tempcount = 0;
    for (unsigned int l = 0; l < nLDFpoints; l++) {
      if(fLDFmodel==eNone) 
	    ldfPos = currentPos;
      else {
	    CalculateLDFPoint(ldfPos, rMoliereInBinCenter, currentDepth, shower.GetXmax(), fLDFmodel);
	    // transform to array coordinates
	    ldfPos.Transform(localToGlobal);
	    ldfPos += currentPos;
      }
    
      if(ldfPos.Z()>0 && ldfPos.Z()<10.*km) {
	//cout << "ldfpos = " << ldfPos.Z() << "currentDistToCore = " << currentDistToCore/m  << " showercore =  " << showerAxis.X() << " " << showerAxis.Y() <<" " << showerAxis.Z() << " " << endl;
	// signal at emitting point in J / Hz
	const double EmissionTime = -currentDistToCore / kSpeedOfLight;
 	// cout <<" emission time: "<< EmissionTime/microsecond <<", distance to core : "<<currentDistToCore<< "currentHeight "<<currentHeight<<endl;
	// test avec un yield
	const double mwPower = kPlanck * kSpeedOfLight * atmo.Density(currentHeight) * kAvogadro / kDryAirMolarMass *  totIonizationEl * kFreeFreeCR / 2. / (double)nLDFpoints;
	//double yield=2.14E-11;
	//double yield = 1.07E-10; latest value
	//const double mwPower = (atmo.Density(currentHeight)*energyDeposit[iDepth]) *(atmo.Density(currentHeight) / atmo.Density(0) ) * yield * hstep/(double)nLDFpoints;
        // loop over all active antennas
 	for (Event::AntennaDataIterator antennaData = event.AntennasBegin(); antennaData != event.AntennasEnd(); ++antennaData) {
	  double lifetime = atmo.GetLifeTime(currentHeight);
	  if (tempcount == 0){
	    //	    cout << "currnet pos " << ldfPos.X() << " " << ldfPos.Y() << " " <<ldfPos.Z() << endl;
	    //	    antennaData->PrintAeff(ldfPos);
	  }
	  tempcount++;
	  antennaData->ObserveEmissions(mwPower, EmissionTime, ldfPos, lifetime);
	} // for each antenna
      }
    } // for each ldf point
   //cout << "totIonizationEl = " <<  totIonizationEl/cm3 << " recovered by MC = " <<  totalRecovered/cm3 << endl;
  } // for each depth
}
