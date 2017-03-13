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



//Function to declare histograms
void InitHist(TH1F* &h, const char* name, const int bins, const double bin_start, const double bin_stop)
{
    h = new TH1F(name,"",bins,bin_start,bin_stop);
}



int main(int argc, char* argv[]) {
  
  if (argc != 4) {
    cerr << "Usage: " << argv[0];
    cerr << " file "<<" nBins "<<" dT" <<endl; 
    return 1;
  }

  TApplication theApp("App",&argc,argv);  

  /*const string Filename = string(argv[1])+".csv";
   
  cout << argv[1]<<endl;  
  const int nBins = atoi(argv[2]);
  const double dT = atof(argv[3]);
  double nyquistFrequency = 1./(2.*dT);

  ifstream myfile(Filename.c_str());
  vector <int> l;
  const int NBins_fft = nBins;
  TH1F *hFreq = new TH1F("hFreq","hFreq",NBins_fft/2.+1,0.,nyquistFrequency);
  TH1F *hdata = new TH1F("hdata","hdata",nBins,0.,nBins);
  TH1F *hReverse = new TH1F("hReverse","hReverse",nBins,0.,nBins);;  
  if (myfile.is_open()) {
    cout << "file is open "<<endl;
    double valeurs[nBins], time[nBins];
    double* in = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    double* in_r = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    fftw_complex* out_r = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(NBins_fft, in, out, FFTW_MEASURE);
    fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(NBins_fft, out_r, in_r, FFTW_MEASURE);
    double moy = 0.;
    //average on signal
    for(int i = 0; i < nBins; i++) {
      double t, v;
      myfile>>t>>v;
      moy += v/(nBins); 
      valeurs[i] = v;
      time[i] = t*dT;
    } 
    double v0m;
    for (int j = 0; j < nBins; j++) {
     in[j]=valeurs[j]-moy;
     //cout<< in[j]<<endl;
     hdata->SetBinContent(j,valeurs[j]-moy);
    }

    fftw_execute(r2cPlan); 

    for (int s = 0; s < (NBins_fft/2+1); s++) {
      double absval = sqrt(out[s][0] * out[s][0] + out[s][1]*out[s][1]); 
      hFreq->SetBinContent(s+1,absval);
      out_r[s][0]=out[s][0];out_r[s][1]=out[s][0];
    }
    


    fftw_execute(c2rPlan);
    //Get Reversed FFT (time trace)
    for(int s = 0; s < nBins-1; s++) 
    hReverse->SetBinContent(s+1,in_r[s]); 
    
    fftw_destroy_plan(r2cPlan);
    fftw_destroy_plan(c2rPlan);
    fftw_free(in);
    fftw_free(out);
    fftw_free(in_r);
    fftw_free(out_r);




  }
  myfile.close();
  TCanvas *c = new TCanvas();
  c->Divide(1,3);
  c->cd(1);
  hdata->GetXaxis()->SetTitle(Form("time bins(x%f)",dT));hdata->GetYaxis()->SetTitle("V-<V>");
  hdata->Draw();
  c->cd(2);
  hFreq->GetXaxis()->SetTitle("frequency bins");hFreq->GetYaxis()->SetTitle("FFT");
  hFreq->Draw(); 
  c->cd(3);
  hReverse->GetXaxis()->SetTitle("time bins");hReverse->GetYaxis()->SetTitle("ADC");
  hReverse->Draw();
  cout <<Form("FFT_%s_%f.pdf",argv[1],dT)<<endl;
  c->SaveAs(Form("FFT_%s_%f.pdf",argv[1],dT));
  */

//  const string Filename = string(argv[1]);
  const int nBins = 768;
  const double dT = 1.;
  double nyquistFrequency = 1./(2.*dT);

  // --- ROOT stuff
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

//  TApplication theApp("App",&argc,argv);  

  /*
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

  }end comment*/
 
  //Read ascii format data
  TRandom rGen (0);
  string line;

  //Helix
  ifstream myfile0 ("traces_SANTY_helix.dat");
  ifstream myfile ("traces_SANTY_helix.dat");

  //A-info
  //ifstream myfile0 ("traces_CHAPE_cornet.dat"); //40 traces
  //ifstream myfile ("traces_CHAPE_cornet.dat");
  
  //Pink
  //ifstream myfile0 ("traces_pink.dat");
  //ifstream myfile ("traces_pink.dat");
  
  //White
  //ifstream myfile0 ("traces_white100000.dat");
  //ifstream myfile ("traces_white100000.dat");

  //const int NREAL=73400;//Random
  const int NREAL=40;

  TFile *f = new TFile("trace_file_helix-testjacques.root","RECREATE");
  vector <int> l; 
  const int NBins_fft = 768;

  if (myfile.is_open()) {

    TH1F *h;TH1F *hFreq;TH1F *hReverse;TH1F *hdata; 
    InitHist(h,"h",nBins,0.,767.);
    InitHist(hFreq,"hFreq",NBins_fft/2.,0.,nyquistFrequency);
    InitHist(hReverse,"hReverse",nBins,0.,767.);
    InitHist(hdata,"hdata", nBins,0.,767.);
    //Forward FFT
    double valeurs[768];
    double* in = (double *) fftw_malloc(sizeof(double) * NBins_fft); 
    fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    //Backward FFT
    double* in_r = (double *) fftw_malloc(sizeof(double) * NBins_fft);
    fftw_complex* out_r = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (NBins_fft/2+1));
    //Intialize plans
    fftw_plan r2cPlan = fftw_plan_dft_r2c_1d(NBins_fft, in, out, FFTW_MEASURE);
    fftw_plan c2rPlan = fftw_plan_dft_c2r_1d(NBins_fft, out_r, in_r, FFTW_MEASURE);
    
    //Read data traces
     
    for(int i = 0; i < NREAL; i++) {
      double moy =0.;
      //Get average and values
      for (int bl = 0; bl <nBins ; bl++ ) {
        double  dumbl,sbl,valbl;
        myfile0>>dumbl>>sbl>>valbl;
        moy += valbl/(nBins);
        valeurs[bl] = valbl;
      } 
      //Calculate autocorrelation
     double val0;
      for (int j = 0; j < nBins; j++) {
        double dum,s,val,s0;
        myfile>>dum>>s>>val; 
     
        if (j==0) val0=val;
        //in[j]+=((val0-moy)*(val-moy))/NREAL;
        in[j]=valeurs[j]-moy;
        //cout<< in[j]<<endl;
        if(i==0) hdata->SetBinContent(j,val-moy);
      } 
    }
    
    for (int j = 0; j < nBins-1 ; j++) {
     h->SetBinContent(j+1,in[j]); 
    }  
    h->GetXaxis()->SetTitle("time bins");h->GetYaxis()->SetTitle("C#_{ss}#(n)"); 
    h->Write();h->SaveAs("Cxx.eps");
    hdata->GetXaxis()->SetTitle("time bins");hdata->GetYaxis()->SetTitle("ADC");
    h->SaveAs("data_trace.eps");
    //hdata->Write(); //this is just the last trace of NREAL
    
    fftw_execute(r2cPlan); 

    for (int s = 0; s < (NBins_fft/2+1); s++) {
      double absval = sqrt(out[s][0]*out[s][0]+out[s][1]*out[s][1]);
      hFreq->SetBinContent(s,absval);  // FFt of Auto-correlation function
      //Build input for inverse FFT
      out_r[s][0]= out[s][0];//rGen.Gaus(0, 1./(4*M_PI*M_PI))*sqrt(0.5*absval);
      out_r[s][1]= out[s][1];//rGen.Gaus(0, 1./(4*M_PI*M_PI))*sqrt(0.5*absval);
    }
    hFreq->GetXaxis()->SetTitle("frequency bins");hFreq->GetYaxis()->SetTitle("S(w)");
    //hFreq->Write();
    hFreq->SaveAs("DSP.eps");

    fftw_execute(c2rPlan);

    //Get Reversed FFT (time trace)
    for(int s = 0; s < nBins-1; s++) 
      hReverse->SetBinContent(s+1,in_r[s]); 
    hReverse->GetXaxis()->SetTitle("time bins");hReverse->GetYaxis()->SetTitle("ADC");
   // hReverse->Write();
    // compare distributions of original and reversed FFT traces
    TH1F *h2; InitHist(h2,"h2", 30,-400, 400);
    TH1F *h2_data; InitHist(h2_data,"h2_data", 30,-400, 400);
    for(int s = 0; s < nBins-1; s++) {
      h2->Fill(hReverse->GetBinContent(s+1));
      h2_data->Fill(hdata->GetBinContent(s+1));
    }
    //h2->Write(); 
    //h2_data->Write();
    
    TCanvas *c = new TCanvas();
    c->Divide(1,3);
    c->cd(1);
    hdata->Draw();
    c->cd(2);
    hFreq->Draw();
    c->cd(3);
    h2_data->Draw();
    
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
  
 

 theApp.Run();
  return 0;

}

