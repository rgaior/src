#include "ElectronicsSimulator.h"
#include <Math/GSLRndmEngines.h>
#include <RandomGenerator.h>
#include <Shower.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <MathConstants.h>
#include <Event.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <tinyxml.h>
#include <tinyxmlCromeUtilities.h>
#include <Configuration.h>
#include <fftw3.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include "BackgroundSimulator.h"

#include <Channel.h>

using namespace std;
using namespace utl;



ElectronicsSimulator::ElectronicsSimulator():
  TimedClass("ElectronicsSimulator"),
  fNoise(-2.37),
  fSigNoise(0.06)
{


  fVerbosity = Configuration::GetInstance().GetVerbosity();
  fRandom = &(RandomGenerator::GetInstance().GetShowerRandom());

  if ( fVerbosity > 0 )
    cout << "\n  ==[ElectronicsSimulator::ElectronicsSimulator()]==>" << endl;

  const string configId("ElectronicsSimulator");
  const string& fileName =
    Configuration::GetInstance().GetConfigurationFileName(configId);

  TiXmlDocument doc(fileName);
  if ( doc.LoadFile() ) {

    const TiXmlElement* topBranch = doc.FirstChildElement(configId);
    if (!topBranch ) {
      throw std::runtime_error(string("ElectronicsSimulator(): ")+
                               string("could not find config in ")+
                               fileName);
    }

    fBeta = GetDoubleValue(topBranch->FirstChildElement("frequencyIndex"));
    //if ( fVerbosity > 0 )
      //cout << "   beta = " << fBeta << endl;
  }

}


void ElectronicsSimulator::plot(int channelID ,int dim, vector<double> &timebins,vector<double> &signalTrace) {
TCanvas* c = new TCanvas("c","c",800,600);
TGraph *g = new TGraph(dim, &timebins[0], &signalTrace[0]);
g->SetTitle("whitenoise");
g->GetXaxis()->SetTitle("time [microsecond]");
g->GetYaxis()->SetTitle("Power [Watt]");
g->SetLineColor(2);
g->Draw("AL"); c->SaveAs("simuMBR319Reducednoise.root");

}



void
ElectronicsSimulator::Run(Event& event)
{
  StartTimer();
  const Detector& detector = Detector::GetInstance();
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
              chanData.MakeDataTrace(chanData.GetSimulationStartTime(), chanData.GetSimulationBinWidth(), chanData.GetSimulationBinCount());
            
            const double skyTemp = chanData.GetTsky();
            const double traceBinWidth = channel.GetTraceBinWidth();
	    // modified on 18/10/2015
            // const double Noise = kBoltzmann * channel.GetBandWidth() * (skyTemp + channel.GetNoise());  
            // Consider only additive electronic noise 
            double Noise = kBoltzmann * channel.GetBandWidth() * channel.GetNoise(); 
            //cout <<"Electronic Noise Power [w]: " <<channel.GetNoise()/watt<<endl;
            std::vector<double>& signalTrace = chanData.GetDataTrace(); //mean sky
            std::vector<double> timebins;
            const int traceLength = signalTrace.size();
             
	    std::vector<double>& simTrace = chanData.GetSimulatedFluxTrace(); // GetSimulatedPowerTrace(); //signal trace
	    const unsigned int NoiseBins = signalTrace.size();
            SimElecChain(simTrace,chanData.GetSimulationStartTime(), traceBinWidth);
	    const double nyquistFrequency = 1./(2.*traceBinWidth);
	    // --- init FFTW3 plans
            double* in = (double *) fftw_malloc(sizeof(double) * NoiseBins);
	    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NoiseBins/2+1));
	    fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(NoiseBins, out, in, FFTW_ESTIMATE);
	    
            for (unsigned int i = 0; i < (NoiseBins / 2) + 1; i++) {
	      out[i][0] = fRandom->Gaussian(1./pow(2.*kPi,2))*sqrt(0.5*Noise*Noise/((channel.GetBandWidth())*(chanData.GetSimulationBinWidth()))); // real
              out[i][1] = fRandom->Gaussian(1./pow(2.*kPi,2))*sqrt(0.5*Noise*Noise/((channel.GetBandWidth())*(chanData.GetSimulationBinWidth()))); // imaginary
	    }
	    fftw_execute(c2rPlan);
            //Added on 18/08/2015
            for(unsigned int bin = 0; bin < NoiseBins; bin++) {signalTrace[bin] +=simTrace[bin]/watt;} // added signal to sky noise
            //SimElecChain(signalTrace,chanData.GetSimulationStartTime(), traceBinWidth); 
            for(unsigned int i=0; i<NoiseBins; i++ ) {signalTrace[i] += Noise/watt ;} // Added electronic noise to (sky noise+ signal)
	    //for(unsigned int i=0; i<NoiseBins; i++ ) {signalTrace[i] = powerToDBm(signalTrace[i]);} /**/
            //SimElecChain(signalTrace,chanData.GetSimulationStartTime(), traceBinWidth);
            for(unsigned int i=0; i<NoiseBins; i++ ) {signalTrace[i] += in[i]/watt;} // added 1/f noise 
	    //SimElecChain(signalTrace,chanData.GetSimulationStartTime(), traceBinWidth); 
            //for(unsigned int i=0; i<NoiseBins; i++ ) {signalTrace[i] = powerToDBm(signalTrace[i]);}
            
            const double RC = 5.*ns;
            LowPassFilter(signalTrace, RC, traceBinWidth); 
            for(unsigned int i=0; i<NoiseBins; i++ ) {
              signalTrace[i] = signalTrace[i]; 
            }
            // --- clean up FFTW3 
  	    fftw_destroy_plan(c2rPlan);
            fftw_free(in);
 	    fftw_free(out);
          }
        }
      }
    }
  }
  StopTimer();
}



void
ElectronicsSimulator::LowPassFilter(std::vector<double>& noise,double RC, double dt) const {

  double alpha = dt / (RC + dt); 
  for(unsigned int i=1; i<noise.size(); i++) {
    noise[i] = alpha * noise[i] + (1.-alpha) * noise[i-1] ;}

}



  //electronic chain response
  //  __      __       __      __
  //    |    |           \   |
  //    |    |     ==>    \  |
  //    |    |             +-+
  //    +----+

double
ElectronicsSimulator::PulseResponse(double time, double start,
				    double stop, double electau) const {

  Double_t response;
  if (time < start) return 0;
  else if( time >= start && time < stop)
    response = 1. - exp(-(time - start)/electau);
  else {
    const Double_t SignLast = 1. - exp(-(stop - start)/electau);
    response = SignLast * exp(-(time - stop)/electau);
  }

  return response;

}


void
ElectronicsSimulator::SimElecChain(std::vector<double>& signal, double TraceStartTime, double TraceBinWidth) {

  const double electau = 0.1*ns;
  double start = -99999.*microsecond;
  double stop = -99999.*microsecond;

  double threshold = 0;
  for(unsigned int i=0; i<signal.size(); i++) {
    if(signal[i] > threshold)
      threshold = signal[i];
  }
  threshold /= kE;

  for(unsigned int i=0; i<signal.size(); i++) {
    if(signal[i] > threshold) {
      start = TraceStartTime + TraceBinWidth*((double)i+0.5);
      break;
    }
  }
  for(int i=signal.size()-1 ; i>0; i--) {
    if(signal[i] > threshold) {
      stop = TraceStartTime + TraceBinWidth*((double)i+0.5);
      break;
    }
  }
  //cout << "start[mus] = " << start/microsecond << " stop[mus] = " << stop/microsecond << endl;

  for(unsigned int i =0; i<signal.size(); i++) {
    double currtime = TraceStartTime + TraceBinWidth*((double)i+0.5);
    signal[i] = signal[i] * PulseResponse(currtime, start, stop, electau);
  }


}
