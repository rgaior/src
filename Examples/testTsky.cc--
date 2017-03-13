#include <Atmosphere.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <iostream>
#include <iomanip>
#include <EventFile.h>
#include <DetectorSetup.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TFile.h>
#include <KGBits.h>
#include <BitTools.h>
#include <Configuration.h>
#include <Detector.h>
#include <Channel.h>
#include <TCanvas.h>
#include <map>
#include <MathFunctions.h>
#include <TApplication.h>
#include <TRint.h>
#include <TStyle.h>

using namespace std;
using namespace utl;








int main(int argc, char** argv) {

  TApplication *app = new TRint("app",0,0,0);
  TStyle *gStyle = new TStyle();
  //gStyle->SetOptStat("");
  if (argc != 2) {
    cerr << "Usage: " << argv[0];
    cerr << " configuration" << endl;
    return 1;
  }
  Configuration& config = Configuration::GetInstance();
  const string configFilename = string(argv[1]);
  config.Init(configFilename);
  Detector::GetInstance().Init();
  const Detector& detector = Detector::GetInstance();
  const std::vector<Antenna> Antennas = detector.GetSetup().GetAntennas();
  const Atmosphere& atmo = Atmosphere::GetInstance();  
  TCanvas *fCanvas = new TCanvas("gain (dBi)","gain (dBi)",400,400);
  TCanvas *fCanvas_Ts = new TCanvas("Aeff(m2)","Aeff(m2)",400,400);
  /*fCanvas->SetTopMargin(0.0);
  fCanvas->SetBottomMargin(0.0);
  fCanvas->SetLeftMargin(0.0);
  fCanvas->SetRightMargin(0.0);*/
  DetectorSetup setup;
  //setup.FindBounds(); 

  
  cout << "min x "<< detector.GetSetup().GetMinX()<<" max x "<< detector.GetSetup().GetMaxX()<<" min y "<< detector.GetSetup().GetMinY()<<" max y "<< detector.GetSetup().GetMaxY()<<endl;
  // Set view for gain patterns (view from top with phi = 0 to the right)
  fCanvas->SetTheta(90);
  fCanvas->SetPhi(180);
  fCanvas_Ts->SetTheta(90);
  fCanvas_Ts->SetPhi(180);

  std::map<unsigned int, TH2D*> fGainHists;
  std::map<unsigned int, TH2D*> fSkyPatterns;

  for(unsigned int iAntenna=0; iAntenna< Antennas.size(); iAntenna++) {
    const Antenna& antenna = Antennas[iAntenna];
    const unsigned int antennaId = antenna.GetId();
    const double fMaxTheta = 180.;
    const std::vector<Channel>& Channels = antenna.GetChannels();  
    for(unsigned int iCh=0; iCh<Channels.size(); iCh++) {
        const Channel& channel = Channels[iCh];
        const unsigned int channelId = channel.GetId();
        //if ( antennaData.HasChannelData(channelId) ) {
          ostringstream gainHistTitle;
          ostringstream skyPatternTitle;
          gainHistTitle << "hGainPlot_" << antennaId << "_" << channelId;
          skyPatternTitle << "hSkyPattern_" << antennaId << "_" << channelId;
          const unsigned int nPhiBins = 180;  // Limited to 180 bins by ROOT
          const vector<double> phiEdges = GenerateBinEdges(-180., 180., nPhiBins);
          const vector<double> phiCenters = GenerateBinCenters(-180., 180., nPhiBins);
    
          const unsigned int nThetaBins = fMaxTheta * 5;  // 0.2° steps
          const vector<double> thetaEdges = GenerateBinEdges(0., fMaxTheta, nThetaBins);
    	  const vector<double> thetaCenters = GenerateBinCenters(0., fMaxTheta, nThetaBins);
          TH2D* gainHist = new TH2D(gainHistTitle.str().c_str(), "", nPhiBins, &phiEdges[0], nThetaBins, &thetaEdges[0]);
          TH2D* TskyPattern = new TH2D(skyPatternTitle.str().c_str(), "", nPhiBins, &phiEdges[0], nThetaBins, &thetaEdges[0]);
          TVector3 antennaPos = antenna.GetPosition();
          vector<double> vphi, vphi_norm;
          for (unsigned int iPhi = 0; iPhi < nPhiBins; iPhi++) {
            const double phi = phiCenters[iPhi] * degree;
            vector<double> vtheta, vtheta_norm;
            for (unsigned int iTheta = 0; iTheta < nThetaBins; iTheta++) {
              const double theta = thetaCenters[iTheta] * degree;
              const double Ts = atmo.MWSkyNoise(theta / radian);
              TVector3 v(0.0, 0.0, 1.0);
              v.SetTheta(theta / radian);
              v.SetPhi(phi / radian);
              v += antennaPos;
	      double gain = channel.GetResponse(v);
              double Aeff = channel.GetEffectiveArea(v); 
              if (gain < 1e-2) gain = 1e-2;
              // Fill histogram with gain in dBi
              gainHist->SetBinContent(iPhi + 1, iTheta + 1, gain); //modif ici ct 10log10 g
              TskyPattern->SetBinContent(iPhi + 1, iTheta + 1, Aeff/m2);
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
          double T_integred = res/res_norm;
          cout << "T sky for this antenna : "<<T_integred <<" K"<<endl ; 
        fGainHists[channelId] = gainHist;
        fSkyPatterns[channelId] = TskyPattern;
       // }
      fCanvas->cd();
      gStyle->SetOptStat("n");
      gStyle->SetPalette(1);
      fGainHists[channelId]->SetStats(0);
      fGainHists[channelId]->Draw("pol surf2");
      fCanvas->Update();
      fCanvas_Ts->cd();
      fSkyPatterns[channelId]->Draw("pol surf2");
      //fGainHists[channelId]->SaveAs("directivity_Horn77_2_polar.pdf");
      fCanvas_Ts->Update();
    }
  }  


  app->Run();
}       
