#include <TApplication.h>
#include <TH1D.h>
#include <TH1F.h>
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



//Function to declare histograms
void InitHist(TH1F* &h, const char* name, const int bins, const double bin_start, const double bin_stop)
{
    h = new TH1F(name,"",bins,bin_start,bin_stop);
}



int main(int argc, char** argv) {
  
  if (argc != 2) {
    cerr << "Usage: " << argv[0];
    cerr << " data.root" << endl;
    return 1;
  }
  
 const string Filename = string(argv[1]);
  const int nBins = 768;
  const double dT = 1.;
  double nyquistFrequency = 1./(2.*dT);

  ifstream myfile (argv[1]);

  vector <int> l;
  const int NBins_fft = 768;

  if (myfile.is_open()) {
    TH1F *hFreq = new TH1F("hFreq","hFreq",NBins_fft/2.,0,nyquistFrequency);
    TH1F *hdata = new TH1F("hdata","hdata",nBins,0.,767.);

    double valeurs[768];
    double* in = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(NBins_fft, in, out, FFTW_MEASURE);
    double moy = 0.;
    /*for(int i = 0; i < nBins; i++) {
      double t, v;
      myfile>>t>>v;

    }*/





  // --- clean up FFTW3
    fftw_destroy_plan(r2cPlan);
    fftw_free(in);
    fftw_free(out);
}






/*
  const string Filename = string(argv[1]);
  const int nBins = 769;
  const double dT = 1.;
  double nyquistFrequency = 1./(2.*dT);

  // --- ROOT stuff
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TApplication theApp("App",&argc,argv);  

  //begin comment
  //Read ROOT format data
  TFile *f = new TFile (Filename.c_str()); 
  TIter next(f->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1 *h = (TH1*)key->ReadObj();
    double* in = (double *) fftw_malloc(sizeof(double) * nBins);
    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (nBins/2+1));
    fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(nBins, in, out, FFTW_MEASURE);
    fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(nBins, out, in, FFTW_MEASURE);
    for ( int i=0; i<nBins; i++ ) in[i] = h->GetBinContent(i+1);
    fftw_execute(r2cPlan);
    //Plot FFT 
    TH1D* hFreq = new TH1D("req","FFT;frequency",nBins/2.,0.,nyquistFrequency);
    hFreq->GetXaxis()->SetNdivisions(1536);
    for (int i = 0; i < (nBins / 2) + 1; i++) {
      //double absval = out[i][1] * out[i][1]-out[i][0]*out[i][0];
      double absval = sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]);
      hFreq->SetBinContent(i+1,absval);
    }
 
  canvas->cd(1); h->Draw();
  canvas->cd(2); hFreq->Draw();
  
  // --- clean up FFTW3 
  fftw_destroy_plan(r2cPlan);
  fftw_destroy_plan(c2rPlan);
  fftw_free(in);
  fftw_free(out);

  }
  //endcomment
 
  //Read ascii format data
  TRandom rGen (0);
  string line;
  ifstream myfile0 ("traces_CHAPE_cornet.dat");
  ifstream myfile ("traces_CHAPE_cornet.dat");
  const int NREAL=62;
  TFile *f = new TFile("trace_file.root","RECREATE");
  vector <int> l; 
  const int NBins_fft = 768;

  if (myfile.is_open()) {

    TH1F *h;TH1F *hFreq;TH1F *hReverse;TH1F *hdata; 
    InitHist(h,"h",nBins-1,0.,768.);
    InitHist(hFreq,"hFreq",NBins_fft/2.,0.,nyquistFrequency);
    InitHist(hReverse,"hReverse",nBins-1,0.,768.);
    InitHist(hdata,"hdata", nBins-1,0.,768.);
    //Forward FFT
    double* in_test1 = (double *) fftw_malloc(sizeof(double) * NBins_fft);double valeurs[767];
    double* in_test2 = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    double* in = (double *) fftw_malloc(sizeof(double) * NBins_fft); 
    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    //Backward FFT
    double* in_r = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    fftw_complex* out_r = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    //Intialize plans
    fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(NBins_fft, in, out, FFTW_ESTIMATE);
    fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(NBins_fft, out_r, in_r, FFTW_ESTIMATE);
    
    //Read data traces
     
    for(int i = 0; i < NREAL; i++) {
      double moy =0.;
      //Get average and values
      for (int bl = 0; bl <nBins-1;bl++ ) {
        int dumbl,sbl,valbl;
        myfile0>>dumbl>>sbl>>valbl;
        moy += valbl/(nBins-1.);
        valeurs[bl] = valbl;
      }
      //Calculate autocorrelation
      for (int j = 0; j < nBins-2 ; j++) {
        int dum,s,val,s0;
        myfile>>dum>>s>>val;
        in_test1[j] +=((val-moy)*(valeurs[j+1]-moy))/NREAL;
        in_test2[j] += pow((val-moy),2)/NREAL;
        hdata->SetBinContent(j,val-moy); 
      }
    }
    
    for (int j = 0; j < nBins-1 ; j++) {
     in[j] = in_test1[j]/in_test2[j];
     h->SetBinContent(j,in[j]);
    }  
    
    h->Write();
    hdata->Write(); //this is just the last trace of NREAL
    
    fftw_execute(r2cPlan); 

    for (int s = 0; s < (NBins_fft/2+1); s++) {
      double absval = sqrt(out[s][0]*out[s][0]+out[s][1]*out[s][1]);
      hFreq->SetBinContent(s,absval);  // FFt of Auto-correlation function
      //Build input for inverse FFT
      out_r[s][0]= rGen.Gaus(0,1./(4*M_PI*M_PI))*sqrt(0.5*absval);
      out_r[s][1]= rGen.Gaus(0,1./(4*M_PI*M_PI))*sqrt(0.5*absval);
    }
    hFreq->Write();

    fftw_execute(c2rPlan);

    //Get Reversed FFT (time trace)
    for(int s = 0; s < nBins-1; s++) 
      hReverse->SetBinContent(s+1,in_r[s]); 
    hReverse->Write();
     
    // compare distributions of original and reversed FFT traces
    TH1F *h2; InitHist(h2,"h2", 30,-200, 200);
    TH1F *h2_data; InitHist(h2_data,"h2_data", 30,-200, 200);
    for(int s = 0; s < nBins-1; s++) {
      h2->Fill(hReverse->GetBinContent(s+1));
      h2_data->Fill(hdata->GetBinContent(s+1));
    }
    h2->Write(); 
    h2_data->Write();

    // --- clean up FFTW3 
    fftw_destroy_plan(r2cPlan);
    fftw_destroy_plan(c2rPlan);
    fftw_free(in);
    fftw_free(out);
    fftw_free(in_r);
    fftw_free(out_r);

    f->Write(); 
    myfile.close();
  }
  else cout << "Unable to open file"; 
  f->Close();
  
 

 // theApp.Run();
  return 0;
*/
}

