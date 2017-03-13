#include <TApplication.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TAxis.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRandom.h>
#include <TMath.h>
#include <TF1.h>
#include <TFile.h>
#include <TKey.h>
#include <cmath>
#include <iostream>
#include <TClass.h>
#include <fftw3.h>
#include <fstream>
#include <string>

using namespace std;

void PrintHeader();



int main(int argc, char** argv) {
  
  if (argc != 2) {
    cerr << "Usage: " << argv[0];
    cerr << " NREAL" << endl;
    return 1;
  }

  const int NREAL = atoi(argv[1]);
  const int nBins = 768;
  const int NBins_fft = 768;
  // --- ROOT stuff
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TApplication theApp("App",&argc,argv);  
  //Create Random
  //TRandom rGen (0);
  ofstream myfile ("traces_white.dat");
  TH1F *h = new TH1F("h","h",768,0,767);
  for (int i =0 ; i< NREAL ; i++) {
   TRandom rGen (0);
    for (int j = 0; j < nBins ; j++) {
      //TRandom rGen (0);
      int val = rGen.Gaus(0,1); 
      myfile<<i<<" "<<j<<" "<<val <<endl;
      if (i ==0){h->SetBinContent(j,val);}
    }
  }
 


/* double* in_r = (double *) fftw_malloc(sizeof(double) * NBins_fft);
 fftw_complex* out_r = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));

 fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(NBins_fft, out_r, in_r, FFTW_MEASURE);
 for (int i =0 ; i< NREAL ; i++) {
   TRandom rGen (0);
    for (int s = 0; s < (NBins_fft/2+1); s++) {
      //int val = rGen.Gaus(0,100); 
      //idouble absval = sqrt(out[s][0]*out[s][0]+out[s][1]*out[s][1]);
      out_r[s][0]= rGen.Gaus(0, 1./(4*M_PI*M_PI));///(s+1);//*sqrt(0.5*absval);
      out_r[s][1]= rGen.Gaus(0, 1./(4*M_PI*M_PI));///(s+1);//*sqrt(0.5*absval);
    }
   fftw_execute(c2rPlan);
   for(int j = 0; j < nBins-1; j++) {
     myfile<<i<<" "<<j<<" "<<in_r[j] <<endl; 
     if (i == 0)h->SetBinContent(j,in_r[j]); 
   }
  }
*/

 
  TCanvas *c = new TCanvas();
  c->cd();
  h->Draw();
 
  myfile.close();

  theApp.Run();
  return 0;

}

