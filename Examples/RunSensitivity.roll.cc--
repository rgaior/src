/********************************************************************************* 
Detection efficiency calculation following specified criteria on N_st and sigma.
Outputs the number of expected events per year in the detection area
Needs as input the root files produced with analyzeSim
*********************************************************************************/

#include <TApplication.h>
#include <TRint.h> 
#include <fstream>
#include <iostream>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>
#include <numeric>
#include <TKey.h>
#include <Event.h>
#include <TROOT.h>
#include <TClass.h>
#include <string.h>
#include <math.h>
//#include <MathFunctions.h>
#include <GHShowerGenerator.h>
#include <AugerUnits.h>
#include <BitTools.h>
#include <algorithm>
#include <MathConstants.h>
#include <stdexcept>
using namespace std; 
using namespace utl;


double Integrate (double (*f)(double), double a, double b, int n) {
  int i;
  double pas, r, res;

  pas = (b - a) / 2 / n;
  res = (*f) (a);
  res += (*f) (b);
  res /= 2;
  for (i = 1; i < 2 * n; i++) {
    r = (*f) (a + i * pas);
    if (i % 2)  res += r + r;
    else res += r;
  }
  return (res * pas * 2 / 3);
}


double LogIntegrate (double (*f)(double), double a, double b, int n) {
  int i;
  double pas, r, res;
  a=log(a); b=log(b);
  pas = (b - a) / 2 / n;
  res = ( exp(a)*(*f)(exp(a))+exp(b)*(*f)(exp(b)) )/2.;
  for (i = 1; i < 2 * n; i++) {
    r = exp(a+i*pas)*(*f)(exp(a+i*pas));
    if (i % 2)  res += r + r;
    else res += r;
  }
  return (res * pas * 2 / 3);
}


double func_angle(double th) {
  return sin(th)*cos(th);
}

double func(double e) {
  double index = 2.63;
  double log10e12 = 19.63;
  double log10wc = 0.15;
  double spec = pow(e,-index)/(1+ exp((log10(e)-log10e12)/(log10wc)));
  return spec;
}


int main(int argc, char** argv) {
  
  TApplication *app = new TRint("app",0,0,0);
  
  if ( argc < 2 ) {
    cerr << " usage: RunSensitivity <simFiles> " << endl;
    return 1;
  }
  vector<string> fileNames;
  
  for (int i = 1; i < argc; i++) {
    fileNames.push_back(string(argv[i]));
  }

  //misc

  TH2D *h2 = new TH2D ("h2","h2",200,0.,20.,4,0.,4.);

  double NAntennas = 7. ;
  double dT = 1. ; // sensitivity in dT years  
  double dS = 9./4.;//225. ;// (3.*sqrt(3.)/2.)*pow(1.5,2) ; // 193.5 surface of detection (km^2)
  double J0 = 5.E30 ; // km^{-2}.sr^{-1}.yr^{-1}.eV^{gamma-1}
  double Omega = 2.*kPi* Integrate(&func_angle,0.,60*kPi/180.0,100);
  double deltaE = LogIntegrate (&func,4.e18,3E20,100);
  
  int Nfiles =  fileNames.size() ;
  vector <string> EventId ; vector <string> EventClean ;
  vector <string> ChannelId ; vector <string> ChannelClean ; 
  vector <double> Maxima ; vector <double> sigma ;
  vector <string> ChannelIdSens ;
  vector <double> noise ; vector <double> vtheta; vector <double> vEnergy; 
  ofstream myfile;
  //myfile.open ("RunSensitivity_DUCK_1.2GHz.out");
  //double eps , y;
  for (int i =0; i< Nfiles ; i++) {
    //int counter = 0 ;
    cout << "Processing file " << fileNames[i] << endl ;
    TFile *file  = new TFile(fileNames[i].c_str(), "", fileNames[i].c_str(), 1) ;
    if (file->IsOpen()) { 
      cout << "File "<<fileNames[i]<< " is Ok ... "<<endl ;
      TIter nextkey(file->GetListOfKeys()) ;
      TKey *key ;
      //TCanvas c1; // Draw each histo in a pdf document 
      //c1.Print("All signals.pdf[");
      while (key = (TKey*)nextkey()) {
        TClass *cl = gROOT->GetClass(key->GetClassName()) ;
        if (!cl->InheritsFrom("TH1")) continue ;
        TH1 *h = (TH1*)key->ReadObj() ;
        string str = "simTrace_ev"  ; 
        const char * name = str.c_str() ;
        const char * title = key->GetName() ;
        string titlestring(title) ; 
        unsigned first_ev = titlestring.find_last_of("ev")+1 ; unsigned last_ev = titlestring.find_last_of("_") ;
        unsigned first_channel = titlestring.find_last_of("_")+1 ; unsigned last_channel = titlestring.length()+1 ;
        int sensOrsignal = strncmp (title,name, 10) ; 
          if(sensOrsignal == 0) {
            EventId.push_back(titlestring.substr(first_ev,last_ev-first_ev)) ;  
            ChannelId.push_back(titlestring.substr(first_channel,last_channel-first_channel)) ; 
            Maxima.push_back(h->GetBinContent(h->GetMaximumBin())) ; 
          }
          else { 
            if (strncmp (title,"hDist", 5) !=0 ){
              ChannelIdSens.push_back(titlestring.substr(first_channel,last_channel-first_channel)) ; 
              //if (h->GetBinContent(h->GetMaximumBin()) !=0) 
              noise.push_back(h->GetBinContent(h->GetMaximumBin())) ; 
            } 
          } 
      }
      cout << "Data size : "<< EventId.size()/ NAntennas <<endl ; //cout << Maxima.size() <<" " <<noise.size()<<endl;
    }
    
    //loop on yield values
    
    //for (double lgy = -3; lgy < 3; lgy+=0.05) {
    for (int  k = 0; k < 200. ;k++) {
      double stepk =0.1*k;
      for (int  l = 0; l < 5 ;l++) {
        double y=pow(10.0,0); 
        double eps = 0 ; double tau = 5*ns; double bandwidth= 0.6;
        int counter = 0 ;
        for (int i = 0 ; i < EventId.size() ; i+=NAntennas ) {  
          int nchannel = 0 ; 
          for (int j = 0 ; j < NAntennas ; j++) { 
            if ( ( y * Maxima[ i + j ] - noise[i+j]*sqrt(tau * bandwidth) ) / noise[i+j]> stepk ) { // S-<P>/delta(P)
              nchannel++ ; 
            }
          }
          if (nchannel > l) {  
            eps+= 1./((EventId.size()/NAntennas));
          }
        }
        //cout << "SNR >"<< k <<" , Nchannel > "<< l <<" --> Eff =  "<<eps <<" --> N events : "<<dT * dS * Omega * J0 * deltaE * eps<<endl; 
        h2->Fill( stepk , l ,dT * dS * Omega * J0 * deltaE * eps/Nfiles);
      }
    } 
       //cout << k <<" "<< l <<" "<<eps <<" "<<dT * dS * Omega * J0 * deltaE * eps<<endl;
       //h2->Fill(k,l,dT * dS * Omega * J0 * deltaE * eps);
       //}
     //}
     // h2->Fill(k,lvec,dT * dS * Omega * J0 * deltaE * eps);
   //file->Close();
   
  
//      eps = double ((counter/NAntennas)/(EventId.size()*Nfiles)) ; 
      //cout <<" yield * "<<y <<" --> efficiency : "<< eps*100 <<" %"<< endl ;
     

      //double mu = dT * dS * Omega * J0 * deltaE * eps ;
      //cout << y <<" "<< mu<<" "<< eps <<endl;
      //myfile << y <<" "<<mu<<endl;
   //}
}
     // cout << "eps "<<eps <<" deltaE "<<deltaE<<" Omega "<<Omega<<endl; 
    
  //double eps = counter/(EventId.size()/7.0);
   //cout << "efficiency : "<< eps << endl;

  // multiply efficiency by dT x dS x Omega x Integral(E)?
  
   

  TCanvas *c = new TCanvas("c","c",200,200);
  c->cd();
  h2->Draw("COLZ");
  myfile.close();
  app->Run();
  //myfile.close();
}



