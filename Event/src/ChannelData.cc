#include "ChannelData.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <MathFunctions.h>
#include <MathConstants.h>
#include <Atmosphere.h>
#include <Channel.h>
#include <Detector.h>
#include <DetectorSetup.h>


using namespace std;
using namespace utl;

ClassImp(ChannelData)


ChannelData::ChannelData(size_t antennaId) :
  fAntennaId(antennaId),
  fChannelId(0),
  fDataStartTime(0.),
  fRealDataStartTime(0.),
  fDataBinWidth(0.),
  fRealDataBinWidth(0.),
  fSimulationStartTime(0.),
  fSimulationBinWidth(0.)
  //fmin_flux(0.)
{

}


void ChannelData::SetId(size_t id)
{
  const Antenna* antenna;
  try {
    antenna = &Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  } catch(...) {
    stringstream err_msg_ss;
    err_msg_ss << "ChannelData::SetId(): Unknown antenna id "
               << fAntennaId << ".";
    string err_msg = err_msg_ss.str();
    throw runtime_error(err_msg);
  }

  // Check whether the channel id is known by requesting it
  try {
    antenna->GetChannel(id);
  } catch(...) {
    stringstream err_msg_ss;
    err_msg_ss << "ChannelData::SetId(): Unknown channel id " << id << " ";
    err_msg_ss << "for antenna " << fAntennaId << "." << endl;
    string err_msg = err_msg_ss.str();
    throw runtime_error(err_msg);
  }

  fChannelId = id;
}


void ChannelData::MakeDataTrace(double startTime,
                                double binWidth,
                                size_t nBins)
{
  fDataStartTime = startTime;
  fDataBinWidth = binWidth;

  fDataTrace.clear();
  fDataTrace.resize(nBins, 0.);
}

void ChannelData::MakeRealDataTrace(double startTime,
                                double binWidth,
                                size_t nBins)
{
  fRealDataStartTime = startTime;
  fRealDataBinWidth = binWidth;

  fRealDataTrace.clear();
  fRealDataTrace.resize(nBins, 0.);
}




void ChannelData::MakeSimulationTraces(double startTime,
                                       double binWidth,
                                       size_t nBins)
{
  fSimulationStartTime = startTime;
  fSimulationBinWidth = binWidth;

  fSimulatedFluxTrace.clear();
  fSimulatedFluxTrace.resize(nBins, 0.);

  fSimulatedPowerTrace.clear();
  fSimulatedPowerTrace.resize(nBins, 0.);
  
  fMinimumFluxTrace.clear();
  fMinimumFluxTrace.resize(nBins, 0.);

}


void ChannelData::ObserveEmission(double energy,
                                  double t1,
                                  double t2,
                                  const TVector3 r1, /* array cs */
                                  const TVector3 r2) /* array cs */
{
  if (!HasSimulationTraces()) {
    throw std::runtime_error("ChannelData::ObserveFlux(): "
                             "Channel has no simulation traces.");
  }

  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();

  const TVector3 origin = 0.5 * (r1 + r2);
  const double effectiveArea = channel.GetEffectiveArea(origin);
  //if (effectiveArea <= 0.0) {
    //"The channel is not sensitive in the direction of the emitter"
  //  return;
  //}

  const TVector3& channelPos = channel.GetGlobalPosition();
  const double arrayHeight =
    Detector::GetInstance().GetSetup().GetArray().GetHeight();

  const double channelHeight = channelPos.Z() + arrayHeight;
  const double prevTimeToChannel =
    atmosphere.GetTimeOfFlight(r1.Z() + arrayHeight,
                               channelHeight,
                               (r1 - channelPos).CosTheta(),
                               channel.GetCenterFrequency());

  const double nextTimeToChannel = atmosphere.GetTimeOfFlight(r2.Z() + arrayHeight,
                               channelHeight,
                               (r2 - channelPos).CosTheta(),
                               channel.GetCenterFrequency());

  t1 += prevTimeToChannel;
  t2 += nextTimeToChannel;

  if (t2 < t1) {
    // Force t1 < t2
    swap(t1, t2);
  }



  // Calculate flux at the detector in [W / m² / Hz]
  const double distance2 = (origin - channelPos).Mag2();
  const double totalFlux = energy / ((t2 - t1) * 4. * kPi * distance2);

  //cout << " (t2 - t1) -> " << (t2 - t1)/ns << " ns" << endl; 
  // Convert flux to received power using the effective area and bandwidth
  const double bandwidth = channel.GetBandWidth();
  const double receivedPower = totalFlux * effectiveArea * bandwidth;
  const double fmin_flux = CalculateSensitivity(); cout << "fmin "<<fmin_flux<<endl;
  // Calculate indices of the first and last bins covered by the signal
    const int firstBin = (t1 - fSimulationStartTime) / fSimulationBinWidth;
  const int lastBin = (t2 - fSimulationStartTime) / fSimulationBinWidth;

  if (firstBin == lastBin) {
    // Signal falls into one bin only
    double fraction = (t2 - t1) / fSimulationBinWidth;
    AddToSimulatedFluxTrace(firstBin, fraction * totalFlux);
    AddToSimulatedPowerTrace(firstBin, fraction * receivedPower);
   // AddToMinimumFluxTrace(firstBin, fraction * fmin_flux);
  } else {
    // Signal falls into two or more bins
    // First bin
    const double tFirstBinUpperEdge =
      fSimulationStartTime + (firstBin + 1) * fSimulationBinWidth;
    double fraction = (tFirstBinUpperEdge - t1) / fSimulationBinWidth;
    AddToSimulatedFluxTrace(firstBin, fraction * totalFlux);
    AddToSimulatedPowerTrace(firstBin, fraction * receivedPower);
    AddToMinimumFluxTrace(firstBin, fraction * fmin_flux);
    // Middle bins (if any)
    for (int i = firstBin + 1; i < lastBin; i++) {
      AddToSimulatedFluxTrace(i, totalFlux);
      AddToSimulatedPowerTrace(i, receivedPower);
      AddToMinimumFluxTrace(i, fraction * fmin_flux);
    }

    // Last bin
    const double tLastBinLowerEdge = fSimulationStartTime + lastBin * fSimulationBinWidth;
    fraction = (t2 - tLastBinLowerEdge) / fSimulationBinWidth;
    AddToSimulatedFluxTrace(lastBin, fraction * totalFlux);
    AddToSimulatedPowerTrace(lastBin, fraction * receivedPower);
    AddToMinimumFluxTrace(lastBin, fraction * fmin_flux);
  }
}


double intgrd_ftime(double t) // parametrage pour t en ns
{
  double fact=0.08*pow(t,-0.3)/(1.0+pow(t,1.5));
  if(fact>1.0) return 1.0;
  if(isnan(fact) || isinf(fact)) return 0.0;
  return fact;
}


void ChannelData::ObserveEmission(double energy,
                                  double t1,
                                  const TVector3 r1,
				  double tau) /* array cs */
{
  if (!HasSimulationTraces()) {
    throw std::runtime_error("ChannelData::ObserveFlux(): "
                             "Channel has no simulation traces.");
  }
 
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();

  const double effectiveArea = channel.GetEffectiveArea(r1);
  if (effectiveArea <= 0.0) {
    return;
   }
  const TVector3& channelPos = channel.GetGlobalPosition();
  const double arrayHeight = Detector::GetInstance().GetSetup().GetArray().GetHeight();
  const double channelHeight = channelPos.Z() + arrayHeight;
  
  const double TimeToChannel = atmosphere.GetTimeOfFlight(r1.Z()+arrayHeight, channelHeight, (r1 - channelPos).CosTheta(), channel.GetCenterFrequency());
  const double TimeAtReceiver = t1 + TimeToChannel ;
  
  // Calculate flux at the detector in [W / m² / Hz]
  const double distance2 = (r1 - channelPos).Mag2();
  const double totalFlux = energy / (4. * kPi * distance2);
  const double fmin_flux = CalculateSensitivity(); 
  cout <<scientific;

  // Convert flux to received power using the effective area and bandwidth
  const double bandwidth = channel.GetBandWidth();
  const double receivedPower = totalFlux * effectiveArea * bandwidth;

  //total number of bins in the trace
  const int Nbins = channel.GetNTraceBins();
  
  //remove an offset time
  const double firstTimeAtReceiver = (TimeAtReceiver - fSimulationStartTime); 
  //  cout << " firstTimeAtReceiver = " << firstTimeAtReceiver << endl;
  const int firstBin = firstTimeAtReceiver / fSimulationBinWidth;
//   cout << "firstBin = " << firstBin << endl;
//   cout << " channelHeight = " << channelHeight << endl;
//   cout << "t1 = " << t1 << " r1.Z() = " << r1.Z() <<  endl;
//   static int count = 0;
//   count++;
//   if (count++>10) exit(1);

  // loop on the bins of the recorded trace
  // and we split the initial total signal into the time bins.
  // The duration of emission is the same as the duration of the received flux,
  // because the source is not moving so there is no time compression.
  // So the algorithm loops over the time at the reception by binwidth steps.
  if(firstBin<=Nbins) {
    const double precision = 0.999; 
    //the lastTime is defined in term of number of tau (tau: characteristic time of emission)
    //here a multiplication by log(10^-3) is used to say that we account for a few taus 
    //lastTime is relative
    //    double emissionDuration = -tau*log(1-precision);
    double emissionDuration = 100*ns;
    //   replaced double lastTime  with emissionDuration
    
    //const int lastBin = (t1 + lastTime - fSimulationStartTime) / fSimulationBinWidth;
    //lastBin is absolute
    const int lastBin = (firstTimeAtReceiver + emissionDuration) / fSimulationBinWidth;


    // for each time bin (of fSimulationBinWidth width) we compute the contribution of the total flux.
    // So we define a start time tS and a stop time tF of the time bin.
    // Small trick here: the first time will not arrive at the beginning of a bin, so the first bin is not complete.
    // so we need to subtract the firstTime to the end of the first bin.
    // There should be the same consideration for the last bin, but it might not be a big effect
    double tS = 0.;//2*ns; // plasma dispersion effects...
    double tF =  (firstBin)*fSimulationBinWidth + fSimulationBinWidth - firstTimeAtReceiver;
    
    for(int iBin=firstBin; iBin<=lastBin; iBin++){
      // for all other bins than the first one, we just add the binwidth
      if (iBin>firstBin) {
	tS += tF-tS ;
	tF = tS + fSimulationBinWidth ;
      }
      //double Integral = tau *( exp(-tS/tau) - exp(-tF/tau));      
      double Integral=LogIntegrate(&intgrd_ftime,(tS>0)?tS:1E-5*ns,tF,10);
      //fill the trace with the weighted power or flux
      AddToSimulatedFluxTrace(iBin, totalFlux*Integral / fSimulationBinWidth);
      AddToSimulatedPowerTrace(iBin, receivedPower*Integral / fSimulationBinWidth);
      //      if (count%10000 == 0) cout << "count = " << count << " flux = " <<  (receivedPower*Integral / fSimulationBinWidth)/(watt/m/m/hertz) << " ibin " << iBin <<" energy = " << energy << " totalFlux =  " << totalFlux << " Integral =  " << Integral <<  endl;
      AddToMinimumFluxTrace(iBin, fmin_flux/ fSimulationBinWidth);
    }
  }
}


void ChannelData::PrintAeff(const TVector3 r1  /* array cs */)
{
  if (!HasSimulationTraces()) {
    throw std::runtime_error("ChannelData::ObserveFlux(): "
                             "Channel has no simulation traces.");
  }
  
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();
  
  const double effectiveArea = channel.GetEffectiveArea(r1);
  cout << " effective area = " << effectiveArea << endl;
}

void ChannelData::AddToSimulatedFluxTrace(int bin, double flux)
{
  if (bin >= 0 && bin < (int) fSimulatedFluxTrace.size())
    fSimulatedFluxTrace[bin] += flux;
}


void ChannelData::AddToSimulatedPowerTrace(int bin, double power)
{
  if (bin >= 0 && bin < (int) fSimulatedPowerTrace.size())
    fSimulatedPowerTrace[bin] += power;
}



void ChannelData::AddToMinimumFluxTrace(int bin, double fmin_flux)
{ 
  if (bin >= 0 && bin < (int) fSimulatedFluxTrace.size())
    fMinimumFluxTrace[bin] += fmin_flux;
}


double ChannelData::GetTsky() const {
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();
  double fMaxTheta = 180.;
  TVector3 antennaPos = antenna.GetPosition();
  const unsigned int nPhiBins = 180;  // Limited to 180 bins by ROOT
  const vector<double> phiEdges = GenerateBinEdges(-180., 180., nPhiBins);
  const vector<double> phiCenters = GenerateBinCenters(-180., 180., nPhiBins);
  const unsigned int nThetaBins = fMaxTheta * 5;  // 0.2° steps
  const vector<double> thetaEdges = GenerateBinEdges(0., fMaxTheta, nThetaBins);
  const vector<double> thetaCenters = GenerateBinCenters(0., fMaxTheta, nThetaBins);
  vector<double> vphi, vphi_norm;
  for (unsigned int iPhi = 0; iPhi < nPhiBins; iPhi++) {
    const double phi = phiCenters[iPhi] * degree;
    vector<double> vtheta, vtheta_norm;
    for (unsigned int iTheta = 0; iTheta < nThetaBins; iTheta++) {
      const double theta = thetaCenters[iTheta] * degree;
      const double Ts = atmosphere.MWSkyNoise(theta / radian);
      TVector3 v(0.0, 0.0, 1.0);
      v.SetTheta(theta / radian);
      v.SetPhi(phi / radian);
      v += antennaPos;
      double gain = channel.GetResponse(v);
      if (gain < 1e-2) gain = 1e-2;
      vtheta.push_back(gain*Ts*sin(theta/radian));
      vtheta_norm.push_back(gain*sin(theta/radian));
    }
    double resth = Integrate(vtheta,0.0,kPi);
    double resth_norm = Integrate(vtheta_norm,0.0,kPi);
    vphi.push_back(resth);
    vphi_norm.push_back(resth_norm);
  }
  double res = Integrate(vphi,0,2*kPi);
  double res_norm = Integrate(vphi_norm,0,2*kPi);
  //T_integred = res/res_norm;
  //cout << "T sky for this antenna : "<<T_integred <<" K"<<endl ;
  return res/res_norm;

}


double ChannelData::GetEffectiveSurf() const 
{
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();
  double fMaxTheta = 180.;
  const double wavelength = channel.GetCenterWavelength();
  TVector3 antennaPos = antenna.GetPosition();
  const unsigned int nPhiBins = 180;  // Limited to 180 bins by ROOT
  const vector<double> phiEdges = GenerateBinEdges(-180., 180., nPhiBins);
  const vector<double> phiCenters = GenerateBinCenters(-180., 180., nPhiBins);
  const unsigned int nThetaBins = fMaxTheta * 5;  // 0.2° steps
  const vector<double> thetaEdges = GenerateBinEdges(0., fMaxTheta, nThetaBins);
  const vector<double> thetaCenters = GenerateBinCenters(0., fMaxTheta, nThetaBins);
  vector<double> vphi, vphi_norm;
  for (unsigned int iPhi = 0; iPhi < nPhiBins; iPhi++) {
    const double phi = phiCenters[iPhi] * degree;
    vector<double> vtheta, vtheta_norm;
    for (unsigned int iTheta = 0; iTheta < nThetaBins; iTheta++) {
      const double theta = thetaCenters[iTheta] * degree;
      TVector3 v(0.0, 0.0, 1.0);
      v.SetTheta(theta / radian);
      v.SetPhi(phi / radian);
      v += antennaPos;
      double gain = channel.GetResponse(v);
      if (gain < 1e-2) gain = 1e-2;

      vtheta.push_back(gain*sin(theta)*wavelength*wavelength/4./kPi);
      vtheta_norm.push_back(gain*wavelength*wavelength/4./kPi);
    }
    double resth = Integrate(vtheta,0.0,kPi);
    double resth_norm = Integrate(vtheta_norm,0.0,kPi);
    vphi.push_back(resth);
    vphi_norm.push_back(resth_norm);
  }
  double res = Integrate(vphi,0,2*kPi);
  double res_norm = Integrate(vphi_norm,0,2*kPi);
  //fSurf= res;
  return res;
}



double ChannelData::CalculateSensitivity() const 
{
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  //const Antenna& antenna = detector.GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  TVector3 r1(0.0, 0.0, 1.0);
  const double ch_elevation = kPi/2. - channel.GetGlobalPosition().Theta();
  const double ch_azimuth = kPi/2. - channel.GetGlobalPosition().Phi();
  r1.SetTheta(ch_elevation/radian);
  r1.SetPhi(ch_azimuth/radian);
  TVector3 antennaPos = antenna.GetPosition();
  r1 += antennaPos;
  const double Aeff = channel.GetEffectiveArea(r1);
  //cout << "Effective Area @"<<ch_elevation <<","<<ch_azimuth << " is "<<Aeff/m2<<" m2"<<endl;
  static double AntTemp = GetTsky();
  const double ElectTemp = channel.GetNoise();
  const double tau = 5 *ns;
  const double bandwidth = channel.GetBandWidth();
  double fmin_flux = kBoltzmann* (AntTemp+ElectTemp) / (Aeff*sqrt(tau * bandwidth));
  return fmin_flux;
}


// void ChannelData::Dump(std::ostream& o)
//   const
// {
//     o << "channel: antennaId starttime[s] binwidth[s] " << "\n"
//       << fAntennaId << " " << fSimulationStartTime/second << " " << fSimulationBinWidth/second
//       << endl;
//     o << "powertrace[W]: ";
//     //    int psize= fSimulatedPowerTrace.size();
//     int psize = fSimulatedFluxTrace.size();
//     double max = -1000;
//     for (int ip = 0; ip < psize; ++ip){
      
//       //       if (fSimulatedPowerTrace[ip]/watt > max){
//       // 	max = fSimulatedPowerTrace[ip]/watt;
//       //       }
      
//       if (fSimulatedFluxTrace[ip]/(watt/m2/Hz) > max){
// 	max = fSimulatedFluxTrace[ip]/(watt/m2/Hz);
//       }
//       //      o << fSimulatedPowerTrace[ip]/watt << " ";
//       o << fSimulatedFluxTrace[ip]/(watt/(m*m)/hertz) << " ";
//       //      o << fSimulatedFluxTrace[ip]/watt << " ";
//     }
//     o << endl;
//     o << "aefftrace[m2]: ";
//     int aeffsize = fAeffTrace.size();
//     for (int iaeff = 0; iaeff < aeffsize; ++iaeff){
//       o << fAeffTrace[iaeff]/m2 << " ";
//     }
//     o << endl;

//     o << "timetrace: ";
//     int tsize = fTimeTrace.size();
//     for (int it = 0; it < tsize; ++it){
//       o << fTimeTrace[it]/s << " ";
//     }
//     o << endl;
// }

void ChannelData::Dump(std::ostream& o,bool power)
  const
{
    o << "channel: antennaId starttime[s] binwidth[s] " << "\n"
      << fAntennaId << " " << fSimulationStartTime/second << " " << fSimulationBinWidth/second
      << endl;
    if (power == true){
      o << "powertrace[W]: ";
      int psize = fSimulatedPowerTrace.size();
      double max = -1000;
      for (int ip = 0; ip < psize; ++ip){
	o << fSimulatedPowerTrace[ip]/(watt) << " ";
      }
      o << endl;
    }
    else if(power == false){
      o << "powertrace[W]: ";
      int psize = fSimulatedFluxTrace.size();
      double max = -1000;
      for (int ip = 0; ip < psize; ++ip){
	o << fSimulatedFluxTrace[ip]/(watt/(m*m)/hertz) << " ";
      }
      o << endl;
    }
    o << "aefftrace[m2]: ";
    int aeffsize = fAeffTrace.size();
    for (int iaeff = 0; iaeff < aeffsize; ++iaeff){
      o << fAeffTrace[iaeff]/m2 << " ";
    }
    o << endl;

    o << "timetrace: ";
    int tsize = fTimeTrace.size();
    for (int it = 0; it < tsize; ++it){
      o << fTimeTrace[it]/s << " ";
    }
    o << endl;
}

double ChannelData::GetEffectiveArea(TVector3 r){
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  return channel.GetEffectiveArea(r);
}

void ChannelData::AddToAeffTrace(TVector3 r){
  double aeff = GetEffectiveArea(r);
  fAeffTrace.push_back(aeff);
}

double ChannelData::GetTime(const TVector3 r1, double t1) /* array cs */
{
  if (!HasSimulationTraces()) {
    throw std::runtime_error("ChannelData::ObserveFlux(): "
                             "Channel has no simulation traces.");
  }
  
  const Antenna& antenna = Detector::GetInstance().GetSetup().GetAntenna(fAntennaId);
  const Channel& channel = antenna.GetChannel(fChannelId);
  const Atmosphere& atmosphere = Atmosphere::GetInstance();
  const TVector3& channelPos = channel.GetGlobalPosition();
  const double arrayHeight = Detector::GetInstance().GetSetup().GetArray().GetHeight();
  const double channelHeight = channelPos.Z() + arrayHeight;  
  const double TimeToChannel = atmosphere.GetTimeOfFlight(r1.Z()+arrayHeight, channelHeight, (r1 - channelPos).CosTheta(), channel.GetCenterFrequency());
  //cout <<" "<<effectiveArea <<" "<< r1.Z()+arrayHeight <<" "<< channelHeight <<" "<<TimeToChannel/microsecond;
  //  cout << "emission time = " <<  t1 << endl;
  t1 += TimeToChannel ;
  //  cout << "time to channel  = " <<  TimeToChannel << endl;
  //OA  cout << "total time = " << t1 + fSimulationStartTime << endl;
  //cout <<" "<<t1/microsecond<<endl;
  return t1;
}

void ChannelData::AddToTimeTrace(TVector3 r, double emissiontime){
  double time = GetTime(r, emissiontime);
  fTimeTrace.push_back(time);
}
