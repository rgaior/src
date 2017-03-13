#include <TApplication.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom.h>
#include <TMath.h>
#include <TF1.h>

#include <cmath>
#include <iostream>
#include <fftw3.h>

using namespace std;

int main(int argc, char** argv) {

  // --- ROOT stuff
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
 
  TApplication theApp("App",&argc,argv);
  TCanvas* canvas = new TCanvas("a","",19,19,1200,500);
  canvas->Divide(3,1);

  //  --- binning
  const int nBins=1500;
  const double dT=1;
  const double t1=-nBins/2.*dT;
  const double t2= nBins/2.*dT;
  double nyquistFrequency = 1./(2.*dT);

  //  --- background parameters
  const int nSin = 4;
  const double periods[nSin] = {250., 50., 20., 10.};
  const double phases[nSin] = {10., 30., 50., 20.};
  const double ampl[nSin] = {.5,0.5, 0.25, 0.25};
  const double randomAmpl = 0.2;

  // --- signal parameters
  const double signalPosition = 0.;
  const double signalHeight = -1.5;
  const double signalWidth = 10*dT;

  // histogram x/y-axis range
  double xScale = 40*signalWidth;
  double yScale = 0.;
  for ( int i=0; i<nSin; i++ ) 
    yScale += ampl[i];
  yScale+=fabs(signalHeight);


  // --- init FFTW3 plans
  double* in = (double *) fftw_malloc(sizeof(double) * nBins);
  fftw_complex* out = 
    (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (nBins/2+1));

  fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(nBins, in, out,
                                        FFTW_ESTIMATE);
  fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(nBins, out, in, 
                                           FFTW_ESTIMATE);

  // --- generate data
  TH1D* hData = new TH1D("data","original trace;time;",nBins,t1,t2);
  hData->GetXaxis()->SetRangeUser(-xScale, xScale);
  hData->GetYaxis()->SetRangeUser(-yScale, yScale);
  hData->GetXaxis()->SetNdivisions(505);
  for ( int i=0; i<nBins; ++i ) {
    const double t = (0.5+i+t1)*dT;
    double background = 0.;
    for ( int j=0; j<nSin; j++ ) 
      background += ampl[j]*sin(2*M_PI*(t-phases[j])/periods[j]);    
    background += randomAmpl*gRandom->Gaus();

    double signal = signalHeight*TMath::Gaus(t,signalPosition, signalWidth);

    hData->SetBinContent(i+1,signal+background);
  }
  canvas->cd(1);
  hData->Draw();


  // --- transform ...
  for ( int i=0; i<nBins; i++ ) 
    in[i] = hData->GetBinContent(i+1);
  fftw_execute(r2cPlan);


  // --- ... and draw 
  TH1D* hFreq = new TH1D("req","FFT;frequency",nBins/2.+1,0.,nyquistFrequency);
  hFreq->GetXaxis()->SetNdivisions(505);
  for (int i = 0; i < (nBins / 2) + 1; i++) {
    double absval = sqrt(out[i][0] * out[i][0] + out[i][1]*out[i][1]);
    hFreq->SetBinContent(i+1,absval);
  }
  canvas->cd(2);
  hFreq->Draw();


  // --- filter  ...
  for ( unsigned int i=0; i<nBins/2.+1; i++ ) {
     const double thisFreqLow = nyquistFrequency/(nBins/2.+1)*i;
     const double thisFreqUp = nyquistFrequency/(nBins/2.+1)*(i+1);
     for ( int j=0; j<nSin; j++ ) {
       if ( thisFreqLow < 1./periods[j] && thisFreqUp > 1./periods[j] ) {
         out[i][0] = 0.;
         out[i][1] = 0.;
       }
     }
  }
  fftw_execute(c2rPlan);

  // --- ... and draw
  TH1D* hFiltered = new TH1D("filtered","filtered;time",nBins,t1,t2);
  hFiltered->GetXaxis()->SetNdivisions(505);
  hFiltered->GetXaxis()->SetRangeUser(-xScale, xScale);
  hFiltered->GetYaxis()->SetRangeUser(-yScale, yScale);
  for (int i = 0; i < nBins; i++) 
    hFiltered->SetBinContent(i+1,in[i]/nBins);
  canvas->cd(3);
  hFiltered->Draw();
  TF1* signalFunc = new TF1("signal","[0]*TMath::Gaus(x,[1],[2])",t1,t2);
  signalFunc->SetParameters(signalHeight,signalPosition,signalWidth);
  signalFunc->SetLineColor(kRed);
  signalFunc->SetLineWidth(1);
  signalFunc->SetNpx(1000);
  signalFunc->Draw("SAME");

  // --- clean up FFTW3 
  fftw_destroy_plan(r2cPlan);
  fftw_destroy_plan(c2rPlan);
  fftw_free(in);
  fftw_free(out);
  

  // ----
  cout << " ---- " << endl;
  cout << " signal at t=" 
       << signalPosition << " height= " << signalHeight << endl;
  cout << " ---- " << endl;
  theApp.Run();
  return 0;

}
