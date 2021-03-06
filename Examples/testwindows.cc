#include <Event.h>
#include <EventFile.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TimeStamp.h>
#include <BitTools.h>
#include <KGBits.h>
#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include <MathFunctions.h>
#include <stdlib.h>
#include <algorithm>
#include <TGClient.h>
#include <TCanvas.h>
#include <TQObject.h>
#include <array>
#include <stdexcept>
#include <sstream>
#include <algorithm>
//#include </Applications/root/include/TSpectrum2.h>

using namespace std;
using namespace utl;


// Slightly modified for tests on simulated showers

int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << "Usage: testwindow Event.root " << endl;
    return 1;
  }
  
  TApplication app("app",0,0);
  TCanvas c1;
   
  // Station coordinates
  double stat_x[7]={-14696.2,-16197,-13947.5,-15449.5,-14701.7,-16951.4,-16201.4};
  double stat_y[7]={-22923.2,-22920.6,-24226.9,-24224 ,-25529.1,-24222.4, -25524.1};
  c1.Print("testwindows.ps["); //output
  // Histograms on all data
  TH1D *windowlength =new TH1D("length","length",100, 0., 10.); // window length distribution
  TH2D* xy= new TH2D("xy","xy",1000,-35000.,-5000.,1000,-30000.,15000.); // shower positions
  TH2D* xystat= new TH2D("xystat","xystat",1000,-35000.,-5000.,1000,-30000.,15000.); // station positions
  // Counters
  int nev = 0 ; // number of events
  int CountOut = 0 ; // out of FOV
  int counttraces = 0 ; // number of good traces
  int alltraces = 0 ; // all traces
    
  
  // Simulation plots (?)
  /*
  TH1F *theta_dist= new TH1F("theta","theta", 40,0.,60.);
  TH1F *theta_dist_all= new TH1F("theta_all","theta_all", 40,0.,60.);
  TH1F *theta_dist_out= new TH1F("theta_out","theta_out", 40,0.,60.);
  TH2D *theta_dista_corr=new TH2D("theta_dist","theta_dist",60,0.,60.,20,0.,2.);
  */
    
    
    
  // Open preselected data root file
  string infile = string(argv[1]);
  EventFile inputFile(infile);
  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  Detector::GetInstance().InitFromFile(inputFile);
  // New event
  Event* event = new Event();
  inputFile.SetEventBuffer(event);
  
  // Loop on events
  while (inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    
    long long eventId = event->GetEventId() ;
    ArrayData& arrayData = event->GetArrayData();
    // shower
    const Shower& shower = arrayData.GetShower();
    double energy = shower.GetEnergy();
    double theta = shower.GetZenith();
    double phi   = shower.GetAzimuth();
    phi = 90. * degree - detector.GetArray().GetPhi() - phi;
    if (phi < (0.0*degree)) phi = phi + 360.0*degree;
    if (phi > (360.0*degree)) phi = phi - 360.0*degree;
    double xCore = shower.GetCore().X();
    double yCore = shower.GetCore().Y();
    cout << "----> Event "<<eventId<<"\n ";
    cout <<"energy: "<<energy<<", theta: "<<theta/degree <<", Phi: "<<phi/degree<<", x: "<<xCore<<", y: "<<yCore<<endl;
    //theta_dist_all->Fill(theta/degree);
    int nant = 0;
    // loop on antennas
    for ( Event::ConstAntennaDataIterator antennaIter = event->AntennasBegin(); antennaIter != event->AntennasEnd(); ++antennaIter ) {
      const AntennaData& antennaData = *antennaIter;
      const int antennaId = antennaData.GetId();
      const Antenna& antenna = detector.GetAntenna(antennaId);
      const TVector3& antennaPos = detector.GetAntenna(antennaId).GetPosition() - shower.GetCore();
      const double chDist = antennaPos.Mag()/km;
      cout <<"Antenna " <<antennaId <<" at distance "<< chDist <<" km from shower." <<endl;
      SensitivityTrace sensTrace, sensTraceWithN;
        
      int nch = 0;
      // loop on channels
      for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData.ChannelsBegin(); chanIter != antennaData.ChannelsEnd(); ++chanIter ) {
        const ChannelData& channelData = *chanIter;
        const unsigned int channelId = channelData.GetId();
        const Channel& channel = antenna.GetChannel(channelId);
        vector<Double_t> trace_data ; // entire data trace
        vector<Double_t> windowed_trace_data ; // windowed data trace
        trace_data = channelData.GetDataTrace();
        const unsigned int traceLength = trace_data.size();
        
        // keep clean data
        TH1D *hist = new TH1D("h","h",1000, 1.E-15, 1.E-11 ); //distribution of bins of each trace
        TH1D *trace_hist = new TH1D(Form("Entire trace_%i",antennaId),Form("Entire trace_%i",antennaId),768, 0., 767); // the trace itself
        for (unsigned int i = 0 ; i <traceLength; i++) {
	      hist->Fill(trace_data[i]);
          trace_hist->Fill(i,trace_data[i]);
	    }
        //eliminate null traces
        //if (hist->GetMean() == 0 || hist->GetRMS()==0) continue;
        //eliminate pathologic traces
        trace_hist->Scale(500.E12);
        int nfound = trace_hist->ShowPeaks(1.,"", 0.005);
        //if (nfound<100) continue;
        trace_hist->Scale(1./500.E12);
        xy->Fill(xCore,yCore);
        alltraces++; // only good traces are counted
        
        //Search for signal windows
        vector<double> ExpectedTimeWindow ;
        double t0= 0.;  double t1 = 0.;
        vector<double> time_bck, value_bck;
        double timeOffset = 0;
        double expSignalConeAngle = 30*degree;
        ExpectedTimeWindow = event->GetExpectedSignalWindow(antennaId, channelId, expSignalConeAngle,0);
          double groundImpactTime = 0;//arrayData.GetGroundImpactTimeCrome(antennaId)/microsecond ;
        cout << " Ground Impact Time : "<< groundImpactTime << endl ;
          
        /*
        // Only for simulation
        const TVector3& pos = channel.GetGlobalPosition();
        const TVector3& pointing = channel.GetPointingDirection();
        vector<TVector3> intersection = RayConeIntersection(shower.GetCore(), shower.GetAxis(), pos,pointing, expSignalConeAngle);
        */
        
        
          
        // if no intersection, just store trace for background
        if (ExpectedTimeWindow.size()==0) {
          for (unsigned int i = 0 ; i< traceLength; i++ ) {
            time_bck.push_back(0.025*i-groundImpactTime);
            value_bck.push_back(trace_data[i]);
          }
           /*
           //Only for simulation (?)
           theta_dist_out->Fill(theta/degree);
           theta_dista_corr->Fill(theta/degree,chDist,t0);
           */
          cout << "No time window for this trace... "<<endl ;
          CountOut++ ;
        }
        else {
          // if intersection, draw trace, retreive t0 and t1
          int bin_0, bin_1, window_l;
          if (ExpectedTimeWindow[0] < ExpectedTimeWindow[1]) {
            //cout <<"---> angle low "<<channel.AngleToFOV(intersection[0])/degree<<" ---> angle high "<<channel.AngleToFOV(intersection[1])/degree<<endl;
            trace_hist->Draw();
            c1.Print("testwindows.ps");
            // Windows from GetExpectedWindow()
            t0 = (ExpectedTimeWindow[0] + timeOffset)/1000.;
            t1 = (ExpectedTimeWindow[1] + timeOffset)/1000.;
            //cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
            bin_0 = (t0*1000)/25.;
            bin_1 = (t1*1000)/25.;
            window_l = fabs(bin_1-bin_0);
            //cout << "Size of window : "<< window_l << ", bin 0 = "<<bin_0<<", bin 1 = "<< bin_1 <<endl;
            windowlength->Fill(t1-t0);
          }
          else {
            //cout <<"---> angle low "<<channel.AngleToFOV(intersection[0])/degree<<" ---> angle high "<<channel.AngleToFOV(intersection[1])/degree<<endl;
            trace_hist->Draw();
            c1.Print("testwindows.ps");
            // Windows from GetExpectedWindow()
            t0 = (ExpectedTimeWindow[1] + timeOffset)/1000.;
            t1 = (ExpectedTimeWindow[0] + timeOffset)/1000.;
            //cout << " ---->-k!j nfd Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
            bin_0 = (t0*1000)/25.;
            bin_1 = (t1*1000)/25.;
            window_l = fabs(bin_1-bin_0);
            windowlength->Fill(t1-t0);
          }
            
          // Windowed signal
          vector <double> time_data, value_data ;
          TGraph *windowed_graph = new TGraph();
          TGraph *bck_graph = new TGraph();
            cout << "T0 "<< t0 <<" T1 "<<t1 <<" T1-T0 "<<t1-t0<< endl;
          // Inside: fill with trace values and background with 0,  Outside: fill background
          for (unsigned int i = 0; i < traceLength; i ++) {
            if ((0.025*i/*-groundImpactTime*/ >= t0) && (0.025*i/*-groundImpactTime*/ <= t1+0.5)) { //this 0.5 is to include the time response otherwise flaaing edge not taken into account
              time_data.push_back(0.025*i-groundImpactTime);
              // cout << "windowed signal "<<0.025*i-groundImpactTime<< " "<<trace_data[i]/watt<<endl;
              value_data.push_back(trace_data[i]);
              windowed_graph->SetPoint(windowed_graph->GetN(), 0.025*i-groundImpactTime, trace_data[i]) ;
              time_bck.push_back(0.025*i-groundImpactTime);
              value_bck.push_back(0.);
            }
            else {
              time_bck.push_back(0.025*i-groundImpactTime);
              value_bck.push_back(trace_data[i]);
              bck_graph->SetPoint(bck_graph->GetN(), 0.025*i-groundImpactTime, trace_data[i]) ;
            }
          }
          // Draw windowed trace
          windowed_graph->SetLineColor(1) ;
          windowed_graph->SetLineWidth(2) ;
          windowed_graph->SetTitle (Form("windowed signal in %i", antennaId));
          windowed_graph->SetLineWidth(3) ;
          windowed_graph->Draw("ALP");
          
	      // Draw an area corresponding to the time window
          double xmin[2] = {t0-groundImpactTime ,t0-groundImpactTime };
          double xmax[2] = {t1-groundImpactTime+0.5 ,t1-groundImpactTime+0.5 };
          double y[2] = {-5000.,5000.};
          TGraph *grshade = new TGraph(4);
          for (int i = 0; i < 2; i++) {
            grshade->SetPoint(i,xmax[i],y[i]);
            grshade->SetPoint(2+i,xmin[2-i-1],y[2-i-1]);
          }
          grshade->SetFillStyle(3944);
          grshade->SetFillColor(4);
          grshade->Draw("fsame");
          c1.Print("testwindows.ps");
          // Draw background sample
          bck_graph-> SetLineColor(4) ;
          bck_graph->SetTitle(Form("background sample from %i", antennaId));
          bck_graph->SetLineWidth(3) ;
          bck_graph->Draw("ALP");
          grshade->Draw("fsame");
          c1.Print("testwindows.ps");
            
          // Windows from GetSensitivityTrace()
         // sensTrace = event->GetSensitivityTrace(antennaId, channelId, expSignalConeAngle, 2.*meter, 0) ;
         // sensTraceWithN = event->GetSensitivityTraceWithN(antennaId, channelId, expSignalConeAngle, 2.*meter, 0) ;
          int nn = sensTrace.trace.first.size();
          vector <double> time_s, value_s ;
          TGraph *sensitivity_graph= new TGraph();
          TGraph *sensitivity_graphWithN= new TGraph();
          // Truncate the sensitivity function to fit into the expected time window
	      for (int i = 0; i < nn; i++) {
            if(sensTraceWithN.trace.first[i]>t0-t0 ){
            //  sensitivity_graph->SetPoint(sensitivity_graph->GetN(),sensTrace.trace.first[i]-groundImpactTime,sensTrace.trace.second[i]) ;
            //  sensitivity_graphWithN->SetPoint(sensitivity_graphWithN->GetN(),sensTraceWithN.trace.first[i]-groundImpactTime,sensTraceWithN.trace.second[i]) ;
            }
          }
          sensitivity_graph->SetLineWidth(4) ;
          sensitivity_graph->SetLineColor(2) ;
          sensitivity_graph->SetTitle(Form("sensitivity of %i", antennaId)) ;
            sensitivity_graph->GetXaxis()->SetTitle("time (#mus) "); sensitivity_graph->GetYaxis()->SetTitle("1./4*pi*r^2 ");
          sensitivity_graph->Draw("ALP") ; grshade->Draw("fsame") ;
          c1.Print("testwindows.ps");
          sensitivity_graphWithN->SetLineWidth(4) ;
          sensitivity_graphWithN->SetLineColor(7);
          sensitivity_graphWithN->GetXaxis()->SetTitle("time (#mus) "); sensitivity_graphWithN->GetYaxis()->SetTitle("Fraction of N_{max} ");
          sensitivity_graphWithN->SetTitle(Form("Fraction of Xmax of %i", antennaId));
          sensitivity_graphWithN->Draw("ALP") ;
          grshade->Draw("fsame");
          c1.Print("testwindows.ps");
            
          counttraces ++; // traces with time windows
        } // condition on time window existance
        nch++;
      } // channels
      //cout << "processed antenna "<<nant<<endl;
      nant++ ;
    } // antennas
    cout << "--------> Ended event "<<nev <<endl;
    nev++;
  } //events
  
  cout <<" traces with windows "<<counttraces <<" all traces "<< alltraces <<endl;

  xy->SetMarkerStyle(2);
  xy->SetMarkerColor(1);
  xy->Draw("");

  for (int i = 0; i <7; i++) {
    xystat->Fill(stat_x[i],stat_y[i]);
  }
  xystat->SetMarkerStyle(20);
  xystat->SetMarkerColor(4);
  xystat->Draw("*same");
  xystat->SaveAs("stations.root");
  xy->SaveAs("bckpos.root");
   
    //cout << "fraction of in "<<theta_dist->GetEntries()*100./theta_dist_all->GetEntries()<<", fraction of out "<<theta_dist_out->GetEntries()*100./theta_dist_all->GetEntries()<<endl;
    //theta_dist_all->SetLineColor(1);theta_dist_all->Draw();
    //theta_dist_out->SetLineColor(4);theta_dist_out->Draw("same");
    //theta_dist->SetLineColor(2);theta_dist->Draw("same");
    //theta_dista_corr->GetZaxis()->SetRangeUser(-5., 5.);theta_dista_corr->SetStats(0);
    //theta_dista_corr->Draw("COLZ");
    
  cout << "END "<<endl;
  c1.Print("testwindows.ps]");
  app.Run();
}

