#include <Atmosphere.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <iostream>
#include <iomanip>
#include <TGraph.h>
#include <TCanvas.h>
#include <TRint.h>
#include <TApplication.h>
#include <TraceSimulator.h>
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
//#include <tinyxml.h>
//#include <tinyxmlCromeUtilities.h>
#include <Configuration.h>
#include <Math/GSLRndmEngines.h>
#include <Styler.h>

using namespace std;
using namespace utl;

int main() {
  TApplication* app;
  app = new TRint("app",0,0,0);
  const Atmosphere& atmo = Atmosphere::GetInstance();
  TGraph* graph = new TGraph();
  TGraph* graph_a = new TGraph();
  const double dHeight1 = 0.5*km;
  const double dHeight2 = 20*km;
  const double fractionN2 = 0.78084;
  const double fractionO2 = 0.209476;
  const double fractionCD = 1.0 - fractionN2 - fractionO2;
  double height = 0*m;
  const double katt1 = 2e-30*cm3*cm3/s;
  const double katt2 = 8e-32*cm3*cm3/s;
  const double AirMolW = (kN2MolarMass*fractionN2 + kO2MolarMass*fractionO2 + kCO2MolarMass*fractionCD);
  cout << kDryAirMolarMass /(gram/mole) << " ---- ----- "<< AirMolW / (gram/mole) << endl;
  
  while ( height < 20*km ) {
    const double X = atmo.Depth(height,1.);  
    const double pressure = atmo.Pressure(height);
    const double temp = atmo.Temperature(height); 
    const double density = kDryAirMolarMass * pressure / (kMolarGasConstant * temp);
    cout << "density = " << density/(1./cm3) << " " << atmo.Density(height)/(1./cm3) << endl;
 
    const double N = (density*kAvogadro/kDryAirMolarMass);

    /*cout << "h=" << showpoint << setiosflags(ios::fixed)  << setprecision(1)
         << setw(4) << height/km << " km, " 
         << "X=" << setprecision(1) << setw(7) <<  X/g*cm2 << " g/cm^2 "
	 << "h(X)=" << setprecision(1) << setw(4) 
         << atmo.Height(X,1.)/km << " km, " 
         << "particle density = " << setprecision(1) << setw(4) 
         << N / cm3 << " particles/cm^3, "
	 << "tau = " << setprecision(1) << setw(4) 
	 << 1./(katt1*N*N*fractionO2*fractionO2 + katt2*N*N*fractionN2* fractionO2) /ns 
	 << " tau from software  = " << setprecision(1) << setw(4) 
	 << atmo.GetLifeTime(height)/ns
	 << endl;
      
       
      cout  << "T=" << setprecision(1) << setw(5) 
       << atmo.Temperature(height) << " K, " << setprecision(6)
       << "n(400 nm)="
       << atmo.RefrIndex(height,kSpeedOfLight/(400*nanometer)) << ", "
       << "n(1GHz)="
       << atmo.RefrIndex(height,1*GHz) << ", "
       << resetiosflags(ios::scientific) << setprecision(2)
       << "dT(1GHz)=" << atmo.GetTimeOfFlight(0.,height,1.,1*GHz)/microsecond-height/kSpeedOfLight << " microsecond, " 
      << " rMoliere=" << atmo.MoliereRadius(X,1.) << " m" 
         << endl; */
     
    graph->SetPoint(graph->GetN(),height/km,atmo.GetLifeTime(height)/ns);
    //graph_a->SetPoint(graph_a->GetN(),height/km,atmo.RefrIndex(height,2*GHz));
    
    if ( height < 10*km ) 
      height+=dHeight1;
    else
      height+=dHeight2;
  }


  TCanvas *c= Styler::Row(1);
  TH2D* frame1_0 = new TH2D("frame1_0",";Height[km]; n(h,#nu)",1,0.,10.,1,10.,140.);

  //TCanvas *c = new TCanvas("c","c", 800,600);
  c->cd(); 
  frame1_0->Draw();
  graph->Draw("same");
  //graph_a->SetLineColor(2); graph_a->Draw("same");
  c->SaveAs("TimeOfFlight_GHz.eps");

  app->Run();
}
