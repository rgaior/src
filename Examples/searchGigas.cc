#include <Event.h>
#include <EventFile.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <AugerUnits.h>
#include <PhysicalConstants.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TimeStamp.h>
#include <BitTools.h>
#include <KGBits.h>
#include <boost/math/distributions/binomial.hpp>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <TGClient.h>
#include <TCanvas.h>
#include <TQObject.h>
// std::all_of
#include <array>


using namespace std;
using namespace utl;


/* ** DATA:
Program selecting time traces where a signal is expected (for eg. with T5 on one of the tanks), and time traces where noise is calculated for all time traces present in a tank.
   ** SIM:
The calculated signal window is projected onto the simulated time trace. Thus you have an associated MBR simulation in that time window. This is a new variable Sum(wi*bi) where each bin is weighted by (1/peak_value in that window)
   ** INPUTS:
2 root files: the preselected traces via T5, and their simulation, and the bunch of traces used to calculate the background
*/



int main(int argc, char** argv) {

  if ( argc < 3 ) {
    cerr << "Usage: searchGigas Preselected.root AllForNoise.root " << endl;
    return 1;
  }
  
  TApplication app("app",0,0);
  
  //Open preselected data root file
  string infile = string(argv[1]);
  EventFile inputFile(infile);
  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  Detector::GetInstance().InitFromFile(inputFile);
  //New event
  Event* event = new Event();
  inputFile.SetEventBuffer(event);
 
  int good_trace = 0;
  int CountIn = 0; int CountOut = 0;
  double timeOffset=0.;
  double t0, t1;
  int eventcounter = 0;
  TGraph*S_graph=new TGraph();
  TGraph*maxi_graph=new TGraph();
  vector <double> S_events; vector <double> maxi_trace;
  vector <long int> candidates;
  vector <int> candidate_Stations;
  //loop on signal events
  while (inputFile.ReadNextEvent() == EventFile::eSuccess ) {
      
    cout <<"\n"<<"---------------------------------------------------------------------------------------------------------"<<endl;
    cout << "\t \t \t"<<"* EVENT "<<inputFile.GetCurrentEventNumber()<<": "<<event->GetEventId()<<" *"<<endl;
      
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
    cout << "E [Eev]= "<<energy/1.e18<<" theta: "<<theta/degree<<" phi: "<<phi/degree<<" x: "<<xCore<<" y: "<<yCore<<endl;
    cout <<"---------------------------------------------------------------------------------------------------------"<<"\n"<<endl;
      
    // loop on antennas
    for ( Event::ConstAntennaDataIterator antennaIter = event->AntennasBegin(); antennaIter != event->AntennasEnd(); ++antennaIter ) {
      
      const AntennaData& antennaData = *antennaIter;
      const int antennaId = antennaData.GetId();
      const Antenna& antenna = detector.GetAntenna(antennaId);
      // weight vectors
      vector<double>w_x; //time
      vector<double>w_y; //A_eff/4piR^2
      // weight function A_eff/4piR^2
      SensitivityTrace sensTrace;
      TGraph* sensGraph = new TGraph();
      TGraph* windowedTrace = new TGraph();
      TGraph* weightedTrace = new TGraph();
     
      // loop on channels
      for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData.ChannelsBegin(); chanIter != antennaData.ChannelsEnd(); ++chanIter ) {
         
        const ChannelData& channelData = *chanIter;
        const unsigned int channelId = channelData.GetId();
        const Channel& channel = antenna.GetChannel(channelId);
        vector<Double_t> trace_data; vector<Double_t> trace_sim; vector<Double_t> copy_0;
        vector<Double_t> windowed_sig; vector<Double_t> weighted_sig;
        trace_data = channelData.GetRealDataTrace();
        trace_sim = channelData.GetSimulatedPowerTrace();
        const unsigned int traceLength = trace_data.size();
        for (int i =0; i< 768; i++) copy_0.push_back(0);
          //Reject traces with no data
          if((trace_data.size()>0) && (trace_data != copy_0)) {
            cout <<antennaId<<".";
            // Calculate expected signal time window
            vector<double> ExpectedTimeWindow ;
            double expSignalConeAngle= 40.0*degree;
            ExpectedTimeWindow = event->GetExpectedSignalWindow(antennaId, channelId, expSignalConeAngle,0);
            if (ExpectedTimeWindow.size()==0) CountOut++;
            else {
              if (ExpectedTimeWindow[0] < ExpectedTimeWindow[1]) {
                t0 = (ExpectedTimeWindow[0] + timeOffset)/1000.;  // minimum time before trigger en microsecondes
                t1 = (ExpectedTimeWindow[1] + timeOffset)/1000.;  // maximum time before trigger
                //cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
              }
              else {
                t0 = (ExpectedTimeWindow[1] + timeOffset)/1000.;  // minimum time before trigger
                t1 = (ExpectedTimeWindow[0] + timeOffset)/1000.;  // maximum time before trigger
                //cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
              }
              sensTrace = (*event).GetSensitivityTrace(antennaId, channelId, expSignalConeAngle, 2.*meter, 0);
              sensGraph = new TGraph();
              windowedTrace =new TGraph();
                
              int nn =sensTrace.trace.first.size();
              double sum_weight = 0.;
              double sum_binweight = 0.;
              double max = *max_element(sensTrace.trace.second.begin(), sensTrace.trace.second.end());
              double S=0.;
              if (max !=0) { // avoid problematic sensitivity traces
                //cout << "size of sens trace "<<nn<<endl;
                int t0_sens=round(sensTrace.trace.first[nn-1]*1.e-6/25.e-9); //cout <<t0_sens<<endl;
                int t1_sens=round(sensTrace.trace.first[0]*1.e-6/25.e-9);//cout <<t1_sens<<endl;
                int size_sens=round(sensTrace.trace.first[0]*1.e-6/25.e-9)-round(sensTrace.trace.first[nn-1]*1.e-6/25.e-9);
                //cout << "size rounded "<<size_sens<<endl;
                //cout <<"step "<<nn/size_sens<<endl;
                for (int i = 0; i < size_sens; i++ ) {
                  w_x.push_back(t0_sens+i);
                  w_y.push_back(sensTrace.trace.second[nn*i/size_sens]);
                  //cout <<t0_sens+i<<" "<<sensTrace.trace.second[nn*i/size_sens]/max<<endl;
                  sum_weight+=sensTrace.trace.second[nn*i/size_sens];
                  sensGraph->SetPoint(sensGraph->GetN(),t0_sens+i , sensTrace.trace.second[nn*i/size_sens]);
                }// Retrieve weight function
                double a = 0.;
                for (int i  = t0_sens; i<t0_sens+size_sens; i++) {
                  sum_binweight+=(w_y[i-t0_sens]*trace_data[i]);
                  windowed_sig.push_back(trace_data[i]);
                  weighted_sig.push_back((w_y[i-t0_sens])*trace_data[i]);
                    if (trace_data[i]>a) a =trace_data[i];
                  //cout <<i<<" "<<trace_data[i]<<" "<<w_y[i-t0_sens]<<endl;
                }
                  
                  // fill data and weighted data
                for (int i  = 0; i<size_sens; i++) {
                  //cout << i<<" "<<w_y[i]<<" "<<windowed_sig[i]/weighted_sig[i]<<endl;
                  windowedTrace->SetPoint( windowedTrace->GetN(),w_x[i] , windowed_sig[i]);
                  weightedTrace->SetPoint( weightedTrace->GetN(),w_x[i] , weighted_sig[i]);
                } // Fill graphs
                S = double(sum_binweight/sum_weight);
                //cout << "-------------------->S = "<<sum_binweight/sum_weight<<endl;
                if( (sum_weight>0) && (sum_binweight>0) ) {
                  S_events.push_back(S);
                  //cout <<"  \n -------------> S: "<< S <<" sum weight: "<<sum_weight<<" sum bin weight: "<<sum_binweight<<endl;
                  candidates.push_back(event->GetEventId());
                  candidate_Stations.push_back(antennaId);
                  maxi_trace.push_back(a);
                }
                else cout << inputFile.GetCurrentEventNumber() << " is negative or diverges!" <<endl;
              }
              CountIn++;
            } // End calculation of time windows
            good_trace++;
          } //End sanity check on traces
          if ((antennaId == 432 ) && (inputFile.GetCurrentEventNumber() == 22)){
              TCanvas*c = new TCanvas();
              c->Divide(1,3);
              c->cd(1);
              sensGraph->Draw("ALP");
              c->cd(2);
              windowedTrace->Draw("ALP");
              c->cd(3);
              weightedTrace->Draw("ALP");
          } // Plots
          
      }// End loop on channels
      
    }// End loop on antennas
    eventcounter++;
     
  }// End loop on events
  
  for (int i =0; i < S_events.size(); i++) {
    S_graph->SetPoint(S_graph->GetN(), i , S_events[i]);
    cout <<"event " <<i <<" ID: " <<" "<<candidates[i]<<" station: "<<candidate_Stations[i]<<endl;
    cout << " S: "<<S_events[i]<<" Maxi: "<<maxi_trace[i]<<endl;
    maxi_graph-> SetPoint(maxi_graph->GetN(), i , maxi_trace[i]); //cout <<" Maximum in the trace: "<<maxi_trace[i]<<endl;
  }
  TCanvas *c_all= new TCanvas ("c_all","c_all",800,500);
  c_all->cd();
  maxi_graph->Draw();
  //S_graph->Draw();
    
  cout <<"\n"<<"---------------------------------------------------------------------------------------------------------"<<"\n"<<endl;
  cout << "\n"<<"Total number of events: "<<inputFile.GetNumberOfEvents()<<endl;
  cout <<"Total good traces: "<<good_trace<<endl;
  cout <<"Total showers inside FOVs: "<< CountIn <<endl;
  
  /********************************************************/
    
  // Now, the background part
  //Open preselected data root file
  string infile_bck = string(argv[2]);
  EventFile inputFile_bck(infile_bck);
  // Read the detector setup
  DetectorSetup detector_bck;
  inputFile_bck.ReadDetectorSetup(detector_bck);
  Detector::GetInstance().InitFromFile(inputFile_bck);
  //New event
  Event* event_bck = new Event();
  inputFile_bck.SetEventBuffer(event_bck);
  vector<Double_t> bck_422; vector<Double_t> bck_432; vector<Double_t> bck_431; vector<Double_t> bck_433; vector<Double_t> bck_385; vector<Double_t> bck_427; vector<Double_t> bck_384;vector<Double_t> copy_0;
  while (inputFile_bck.ReadNextEvent() == EventFile::eSuccess ) {
    for (int i= 0; i < S_events.size(); i++) {
      if (event_bck->GetEventId()==candidates[i])
        continue;
        // retrieve signal window size and borders and weight every background trace !!!!! 
      else {
        // loop on antennas
        for ( Event::ConstAntennaDataIterator antennaIter = event->AntennasBegin(); antennaIter != event->AntennasEnd(); ++antennaIter ) {
          const AntennaData& antennaData_bck = *antennaIter;
          const int antennaId_bck = antennaData_bck.GetId();
          const Antenna& antenna_bck = detector_bck.GetAntenna(antennaId_bck);
          // loop on channels
          for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData_bck.ChannelsBegin(); chanIter != antennaData_bck.ChannelsEnd(); ++chanIter ) {
            const ChannelData& channelData_bck = *chanIter;
            const unsigned int channelId_bck = channelData_bck.GetId();
            const Channel& channel_bck = antenna_bck.GetChannel(channelId_bck);
            const std::vector<Double_t>& trace_data_bck=channelData_bck.GetRealDataTrace();
              
            if (trace_data_bck.empty() )
              continue;
            for (int k = 0; k< 768; k++) copy_0.push_back(0);
            
            if((trace_data_bck.size()>0)) {
                
              for (int i = 0; i< 768; i++ ) {
                 if (antennaId_bck==422  && (trace_data_bck[i]!=copy_0[i]) ) bck_422.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==432 && (trace_data_bck[i]!=copy_0[i]))  bck_432.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==431 && (trace_data_bck[i]!=copy_0[i]))  bck_431.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==433 && (trace_data_bck[i]!=copy_0[i]))  bck_433.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==385 && (trace_data_bck[i]!=copy_0[i]))  bck_385.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==427 && (trace_data_bck[i]!=copy_0[i]))  bck_427.push_back(trace_data_bck[i]);
                 else if (antennaId_bck==384 && (trace_data_bck[i]!=copy_0[i]))  bck_384.push_back(trace_data_bck[i]);
                 else break;
              }
            }
          }// loop on channels
        } //loop on antennas
      } // only background traces
    } //loop on candidates
  } //loop on events
    
  TH1D *hist_422 = new TH1D("hist_422","hist_422",50,1.e-15,1.e-12);
  TH1D *hist_432 = new TH1D("hist_432","hist_432",50,1.e-15,1.e-12);
  TH1D *hist_431 = new TH1D("hist_431","hist_431",50,1.e-15,1.e-12);
  TH1D *hist_433 = new TH1D("hist_433","hist_433",50,1.e-15,1.e-12);
  TH1D *hist_385 = new TH1D("hist_385","hist_385",50,1.e-15,1.e-12);
  TH1D *hist_427 = new TH1D("hist_427","hist_427",50,1.e-15,1.e-12);
  TH1D *hist_384 = new TH1D("hist_384","hist_384",50,1.e-15,1.e-12);
    
  for (int i = 0; i < bck_422.size(); i++) {
    hist_422->Fill(bck_422[i]);
  }
  for (int i = 0; i < bck_432.size(); i++) {
    hist_432->Fill(bck_432[i]);
  }
  for (int i = 0; i < bck_431.size(); i++) {
    hist_431->Fill(bck_431[i]);
  }
  for (int i = 0; i < bck_433.size(); i++) {
    hist_433->Fill(bck_433[i]);
  }
  for (int i = 0; i < bck_385.size(); i++) {
    hist_385->Fill(bck_385[i]);
  }
  for (int i = 0; i < bck_427.size(); i++) {
    hist_427->Fill(bck_427[i]);
  }
  for (int i = 0; i < bck_384.size(); i++) {
    hist_384->Fill(bck_384[i]);
  }
    
    
    TCanvas *c_bck= new TCanvas ("c_bck","c_bck",800,500);
    c_bck->Divide(4,2);
    c_bck->cd(1);
    hist_422->Draw();
    c_bck->cd(2);
    hist_432->Draw();
    c_bck->cd(3);
    hist_431->Draw();
    c_bck->cd(4);
    hist_433->Draw();
    c_bck->cd(5);
    hist_385->Draw();
    c_bck->cd(6);
    hist_427->Draw();
    c_bck->cd(7);
    hist_384->Draw();
    
    
    double Q422, Q431, Q385, Q432, Q384,Q427,Q433, Maxi_422, Maxi_431, Maxi_385, Maxi_432, Maxi_384, Maxi_427, Maxi_433;
    TGraph *Q_graph = new TGraph();
    TH1D *SS_hist = new TH1D("SS_hist","SS_hist",50,1.e-15,1.e-12);
    
    for (int i = 0; i<candidates.size(); i++) {
        cout << "Event "<< i <<", ID: "<<candidates[i]<< ", Station:"<<candidate_Stations[i]<<", S: "<<S_events[i] <<endl;
        
        if (candidate_Stations[i]==422) {
            if (hist_422->GetRMS()!=0){
            Q422 = (S_events[i]-hist_422->GetMean())/hist_422->GetRMS();
            Q_graph->SetPoint(Q_graph->GetN(),i, Q422); cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q422;
            Maxi_422 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_422); cout << " ,Max : "<<Maxi_422<<endl;}
            else continue;
            SS_hist->Fill(S_events[i]);
            //cout << "-------->S "<<S_events[i]<<endl;
        }
        else if (candidate_Stations[i]==431) {
            if (hist_431->GetRMS()!=0){
            Q431 = (S_events[i]-hist_431->GetMean())/hist_431->GetRMS(); //cout <<Q431<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q431 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q422;
            Maxi_431 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_431); cout << " ,Max : "<<Maxi_431<<endl;}
            else continue;
        }
        else if (candidate_Stations[i]==432) {
            if (hist_432->GetRMS()!=0){
            Q432 = (S_events[i]-hist_422->GetMean())/hist_422->GetRMS(); //cout <<Q432<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q432 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q432;
            Maxi_432 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_432); cout << " ,Max : "<<Maxi_432<<endl;}
            else continue;
        }
        else if (candidate_Stations[i]==433) {
             if(hist_433->GetRMS()!=0){
            Q433 = (S_events[i]-hist_422->GetMean())/hist_422->GetRMS(); //cout <<Q433<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q433 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q433;
            Maxi_433 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                 maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_433); cout << " ,Max : "<<Maxi_433<<endl;}
            else continue;
        }
        else if (candidate_Stations[i]==385) {
             if(hist_385->GetRMS()!=0){
            Q385 = (S_events[i]-hist_385->GetMean())/hist_385->GetRMS(); //cout <<Q385<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q385 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q385;
            Maxi_385 = (maxi_trace[i]-hist_385->GetMean())/hist_385->GetRMS();
                 maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_385); cout << " ,Max : "<<Maxi_385<<endl;}
            else continue;
        }
        else if (candidate_Stations[i]==427) {
            if(hist_427->GetRMS()!=0){
            Q427 = (S_events[i]-hist_422->GetMean())/hist_422->GetRMS(); //cout <<Q427<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q427 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q427;
            Maxi_427 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_427); cout << " ,Max : "<<Maxi_427<<endl;}
            else continue;
        }
        else if (candidate_Stations[i]==384) {
            if(hist_384->GetRMS()!=0)
            {Q384 = (S_events[i]-hist_422->GetMean())/hist_422->GetRMS(); //cout <<Q384<<endl;
            Q_graph->SetPoint(Q_graph->GetN(),i,Q384 );cout << "------> In station"<<candidate_Stations[i]<<": --> Q: "<<Q384;
            Maxi_384 = (maxi_trace[i]-hist_422->GetMean())/hist_422->GetRMS();
                maxi_graph->SetPoint(maxi_graph->GetN(),i, Maxi_384); cout << " ,Max : "<<Maxi_384<<endl;}
            else continue;
        }
    }
    
    TCanvas *c_bck422= new TCanvas ("c_bck422","c_bck422",800,500);
    c_bck422->cd();SS_hist->SetLineColor(2);
    SS_hist->Draw();
    hist_422->Draw("same");
    
    
   TCanvas *c_Q= new TCanvas ("c_Q","c_Q",800,500);
   c_Q->cd();
    Q_graph->GetYaxis()->SetRangeUser(-.5,5.);
   Q_graph->Draw("AP");
    maxi_graph->SetMarkerColor(2);
    maxi_graph-> Draw("same");
    
    
    
    
  app.Run();
}





















/* Looping on all data, first, define for each trace a signal window where we expect the signal to arrive on each antenna. extract SNR in each window, and plot SNR distribution of the signal windows. */

vector<vector<double> > GetExpectedSignalSNR(Event& event,DetectorSetup& detector, vector<vector<double> >& SNR) {
  
  ArrayData& arrayData = event.GetArrayData();
  vector <double> SNR_Signal, SNR_Noise;
  int counttraces= 0; // Number of traces
  double timeOffset = 0 ;
    
  // event
  const unsigned int eventId = event.GetEventId();
  UInt_t gpsSec = event.GetGPSSecond() ;
    
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
    
  // time window
  double t0, t1; int Nofov =0;
  
  // loop on antennas
  for ( Event::ConstAntennaDataIterator antennaIter = event.AntennasBegin(); antennaIter != event.AntennasEnd(); ++antennaIter ) {
    const AntennaData& antennaData = *antennaIter;
    const int antennaId = antennaData.GetId(); 
    //cout << "-> antenna "<<antennaId<<endl;
    const Antenna& antenna = detector.GetAntenna(antennaId);
    vector <double> SNR_sig;
    double avwindows = 0;
    
    // loop on channels
    for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData.ChannelsBegin(); chanIter != antennaData.ChannelsEnd(); ++chanIter ) {
      vector <double> SNR_Signal;
      const ChannelData& channelData = *chanIter;
      const unsigned int channelId = channelData.GetId();
      const Channel& channel = antenna.GetChannel(channelId);
      
      // get the expected signal window in a channel
      vector<double> expectedSignal;
      vector<double> trace;
      double binwidth = channelData.GetRealDataBinWidth();
      double expSignalConeAngle= 30.0*degree;
      trace = channelData.GetRealDataTrace();
        
        if (channelData.HasRealDataTrace() /*& antennaId !=384*/) { // Reject peculiar traces on 384
            expectedSignal = event.GetExpectedSignalWindow(antennaId, channelId, expSignalConeAngle,0);
            for (int i=0; i< 768; i++){ if (trace[i]!=0) counttraces++;}
        }
          
    
      if (expectedSignal.size()==0) { // no intersection
        //cout << " has no intersection in its fov ..."<<endl; 
        Nofov++;
        continue; // go to the next channel
      }
      //else cout << "--> intersection "<<endl;
      if (expectedSignal[0]<expectedSignal[1]) {
        t0 = (expectedSignal[0] + timeOffset);  // minimum time before trigger 
        t1 = (expectedSignal[1] + timeOffset);  // maximum time before trigger
        //cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
      }
      else {
        t0 = (expectedSignal[1] + timeOffset);  // minimum time before trigger
        t1 = (expectedSignal[0] + timeOffset);  // maximum time before trigger
        //cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
      }
      int bin_0 = t0/binwidth; int bin_1 = t1/binwidth;
      double window_l = bin_1-bin_0;
      double max = 0;
      double av = 0;
      int bins_out =0;
      double noise_out;
      double av_noise = 0; 
      // get peak Power
      for (int i = bin_0 ; i < bin_1 ; i++) {
        if (trace[i]!= 0 ) {
          if (trace[i]>max) max = trace[i];
        }
        else break;
      }
      // get average noise outside window 
      for (int i =0 ; i< bin_0 || i> bin_1 ; i++ ) {
       if (trace[i] != 0 ) {
         av += trace[i]; //average noise outside window
         bins_out++;
       }
       else break;
      }
      // get SNR inside window 
      if (max!=0 & max < 1.e-10 & av!=0 & bins_out !=0) {
        noise_out = av/bins_out;
        //cout << "--> SNR_Signal [dB]: "<< max/noise_out << endl;
        SNR_Signal.push_back(max/noise_out);
      }
      //else cout << max <<" "<<av <<" "<<bins_out<<endl;
      
      vector<vector<double> > ALL; 
      // get times of shifted windows
      if (max!=0 &  max < 1.e-10 & av!=0 & bins_out !=0)  {
        //cout << " searching for noise windows of size "<< window_l<< " : [bin0,bin1] = ["<<bin_0<<","<<bin_1<<"]"<<endl;
        for (int j =0; j<768 ;j+= window_l){ 
          vector <double> N;
          if ( (j<bin_0 || j>bin_1) &( (j+window_l)<bin_0 || (j+window_l)>bin_1) &(j+window_l)<trace.size()) 
          {
            //cout <<" ok-->["<<j<<","<<j+window_l<<"]"<<endl;
            N.push_back(j);
            N.push_back(j+window_l);
            ALL.push_back(N);
          } 
        }
        //cout << "Number of windows: "<<ALL.size()<<endl;
        for (int i =0 ; i < ALL.size() ; i++) {
          //cout <<" t0: " <<ALL[i][0]<<" t1: "<<ALL[i][1]<<endl; cout << "window length "<<ALL[i][1]-ALL[i][0];
          double Nmax = 0.;
          double Nav = 0.;
          int Nbins_out = 0;
          double Nnoise_out = 0.;
          // get peak Power 
          for (int k=ALL[i][0] ; k<ALL[i][1]; k++) {
            if (trace[k] != 0 ) {
              if (trace[k]>Nmax) Nmax = trace[k];
            }
            else break;
          }
          // get average noise outside window 
          for (int m =0 ; m<ALL[i][0] || m>ALL[i][1] ; m++ ) {
            if (trace[m] != 0 ) {
              Nav += trace[m]; //average noise outside window
              Nbins_out++;
            }
            else break;
          }
          // get SNR for this window 
          if (Nmax!=0 & Nav!=0 & Nbins_out !=0) {
            Nnoise_out = Nav/Nbins_out;
            //cout << "Peak power [pw]: "<< Nmax*1.e12 << ", Average noise [pw]: "<<Nnoise_out*1.e12<<endl;
            //cout << "--> SNR_Noise [dB]: "<< Nmax/Nnoise_out << endl;
            avwindows+=Nmax/Nnoise_out;
          }
        } // end loop on shifted noise windows 	
        SNR_Signal.push_back (avwindows/ALL.size());  
        SNR.push_back(SNR_Signal);
      } // end condition on trace content
    } // end channels    
  } // end antennas
  //cout <<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Not in the fov "<<Nofov<<endl;
    cout << "Number of traces "<<counttraces/768<<endl;
  return SNR;
}


/*
int main(int argc, char** argv) {
  if ( argc < 2 ) {
    cerr << "Usage: searchGigas input.root [...]" << endl;
    return 1;
  }
  TApplication app("app",0,0);
  TH1F *hist_signal = new TH1F("sig","sig",200,0.,20);
  TH1F *hist_noise = new TH1F("noise","noise",200,0.,20.);
 
  //Open the input and output files
  vector<string> filenames;
  for (int i = 1; i < argc; i++) {
    filenames.push_back(string(argv[i]));
  }

  // Open the input and output files
  EventFile inputFile(filenames);
  EventFile outputFile("candidates.root", EventFile::eWrite);

  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  Detector::GetInstance().InitFromFile(inputFile);

  // Create an event buffer and assign it to the input and output files
  Event* event = new Event();
  inputFile.SetEventBuffer(event);
  outputFile.SetEventBuffer(event); 

  vector<vector<double> > SNR_sig;
  vector<vector<double> > SNR;
  int count=0;
  double alpha =0.01;
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) { 
    cout << "EVENT "<< count <<": "<<event->GetEventId()<<endl;
    GetExpectedSignalSNR (*event, detector, SNR_sig);
    count++;
  } 
  int NTraces = SNR_sig.size();
  //cout << "Total number of selected time traces: " << NTraces << endl;
  for (int i = 0 ; i < NTraces ; i++)  { 
    //cout <<count<<" " <<SNR_sig [i][0] <<" "<<SNR_sig[i][1]<<endl;
    for (int k =0; k < 200; k++) {
      int count_sig = 0; 
      int count_noise = 0;
      double stepk =0.1*k;
      if (SNR_sig[i][0] > stepk) {
        count_sig++; 
        hist_signal->Fill(stepk,count_sig);
      } 
      if (SNR_sig[i][1] > stepk) {
        count_noise++; 
        hist_noise->Fill(stepk,count_noise);
      }
    } // end loop on SNR
  } // end loop on traces   
  hist_noise->Sumw2();
  hist_signal->Sumw2();
  double x[200],y[200],y_noise[200],exl[200],exh[200],eyl[200],eyh[200] ;
  for (int k = 0; k <200; k++) { 
    x[k] = 0.1*k; 
    y[k]=hist_signal->GetBinContent(k); 
    y_noise[k]=hist_noise->GetBinContent(k);
    exl[k]= 0.001; exh[k]= 0.09;//99% First Must check this with output from stat clopper
    eyl[k]= 0; eyh[k]= 0; 
  }
  TGraphAsymmErrors *graph_signal = new TGraphAsymmErrors(200,x,y,eyl,eyl,eyl,eyh);
  TGraphAsymmErrors *graph_noise = new TGraphAsymmErrors(200,x,y_noise,exl,exh,eyl,eyh);
  
 // cout <<"--> Number of events: "<< count << endl;

  // Finalise the output file
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();
  cout << "Found " << outputFile.GetNumberOfEvents() << " signal candidates" << endl;
  //
  TCanvas *c = new TCanvas("c","c",700,500);
  c->cd();
  hist_signal->GetXaxis()->SetTitle("Minimum SNR");
  hist_signal->GetYaxis()->SetTitle("Number of time traces");
  graph_signal->SetLineColor(2);
  graph_signal->Draw("ALP");
  graph_noise->SetLineColor(4);
  graph_noise->Draw("SAME");

  confidence_limits_on_frequency(NTraces, 1);
  app.Run();
}
*/





















/*

bool IsSignalCandidate(Event& event, DetectorSetup& detector, double decibelAboveMean, double angleThreshold, int printOut, double timeOffset) {
//  const ArrayData& arrayData = event.GetArrayData();

  ArrayData& arrayData = event.GetArrayData();
  angleThreshold = angleThreshold*degree;
  timeOffset = timeOffset * 1000.0; // micro sec into nano
  const unsigned int eventId = event.GetEventId();
  UInt_t gpsSec = event.GetGPSSecond();
  const Shower& shower = arrayData.GetShower();
  double energy = shower.GetEnergy();
  double theta = shower.GetZenith();
  double phi   = shower.GetAzimuth();
  phi = 90. * degree - detector.GetArray().GetPhi() - phi;
  if (phi < (0.0*degree)) phi = phi + 360.0*degree;
  if (phi > (360.0*degree)) phi = phi - 360.0*degree;
  double xCore = shower.GetCore().X();
  double yCore = shower.GetCore().Y();
  double angle = 0.0*degree;
  // time window
  double t0, t1;

  bool hasSignal = true;

  for ( Event::ConstAntennaDataIterator antennaIter = event.AntennasBegin(); antennaIter != event.AntennasEnd(); ++antennaIter ) {

    const AntennaData& antennaData = *antennaIter;
    const int antennaId = antennaData.GetId();
    const Antenna& antenna = detector.GetAntenna(antennaId);
    double energyOld = 0.*eV;
    double thetaOld = 0.*degree;
    double phiOld = 0.*degree;
    double xCoreOld = 0.*m;
    double yCoreOld = 0.*m;

    for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData.ChannelsBegin(); chanIter != antennaData.ChannelsEnd(); ++chanIter ) { // channels

      const ChannelData& channelData = *chanIter;
      const unsigned int channelId = channelData.GetId();
      const Channel& channel = antenna.GetChannel(channelId);
      
      cout << " ANTENNA "<< antennaId;
      // Get the expected signal window in a channel
      vector<double> expectedSignal;
      int drawExpSigError=1; // use fluctuation
      double expSignalConeAngle= 20.0*degree; // gives approx. 500 ns window
      const TVector3& pointing = channel.GetPointingDirection(); 
      cout << "--->>>>>>>>>>>>>>>>>>>>>>>>   PointingDirection "<<pointing.Theta()*degree<<" "<<pointing.Phi()*degree<<endl;
 
      expectedSignal = event.GetExpectedSignalWindow(antennaId, channelId, expSignalConeAngle, drawExpSigError);

      if (expectedSignal.size()==0) { // no intersection
        cout << " has no intersection in its fov ..."<<endl; continue; // go to the next channel
      }
      else cout << "--> intersection "<<endl;
      if (expectedSignal[0]<expectedSignal[1]) {
        t0 = (expectedSignal[0] + timeOffset);  // minimum time before trigger 
        t1 = (expectedSignal[1] + timeOffset);  // maximum time before trigger
        cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
      }
      else {
        t0 = (expectedSignal[1] + timeOffset);  // minimum time before trigger
        t1 = (expectedSignal[0] + timeOffset);  // maximum time before trigger
         cout << " ----> Intersection window [t0,t1] : ["<<t0<<", "<<t1<<"]"<<endl;
      }

      const std::vector<Double_t>& signalTrace = channelData.GetRealDataTrace(); // This is in w
    if (signalTrace.empty()) { 
        continue;
      }
    
      double meanPower =0.0; 
      for (size_t i = 0; i < signalTrace.size(); i++) {
        meanPower = meanPower + signalTrace[i] ; 
      }
      meanPower = meanPower / double(signalTrace.size()); cout <<"-------> Mean power: "<< meanPower;

      double fractionAbove = 0.0;
      unsigned int numberAbove = 0;
      unsigned int numberAboveMean = 0;
      unsigned int numberAbove5 = 0;
      unsigned int numberAbove6 = 0;
      unsigned int numberAbove7 = 0;
      unsigned int numberAbove8 = 0;
      unsigned int numberAbove9 = 0;


      // Exclude traces affected by RFI sources and unplugged channel
      //if ( (meanPower<) && (meanPower>) ) {
        // threshold for printout
        const double signalThreshold =  meanPower*pow(10,decibelAboveMean/10.)  ; cout << " --> SignalThreshold "<<signalThreshold;
        const double signalThreshold5 =  meanPower*pow(10,decibelAboveMean*5/10.)  ; cout << " , 5 dB: "<<signalThreshold5;//(meanPower - 5 ) * utl::milli * volt;
        const double signalThreshold6 =  meanPower*pow(10,decibelAboveMean*6/10.)  ; cout << " , 6 dB: "<<signalThreshold6;//(meanPower - 6 ) * utl::milli * volt;
        const double signalThreshold7 =  meanPower*pow(10,decibelAboveMean*7/10.)  ; cout << ", 7 dB: "<<signalThreshold7; //(meanPower - 7 ) * utl::milli * volt;
        const double signalThreshold8 =  meanPower*pow(10,decibelAboveMean*8/10.)  ; cout << ", 8 dB: "<<signalThreshold8;//(meanPower - 8 ) * utl::milli * volt;
        const double signalThreshold9 =  meanPower*pow(10,decibelAboveMean*9/10.)  ;cout << ", 9 dB: "<<signalThreshold9<<endl; //(meanPower - 9 * 2.5) * utl::milli * volt;
        const double binWidth = channel.GetTraceBinWidth();
        const double startTime = channelData.GetRealDataStartTime();
        unsigned int i0 = (int)( (t0 - startTime) / binWidth );
        unsigned int i1 = (int)( (t1 - startTime) / binWidth ) + 1;
        double timeSignal = 0.;
        double maxSignal = 0.;
        double meanPowerOld = 99.;

        for (unsigned int i = i0; i < i1; i++) {
          if ((signalTrace[i] > signalThreshold)) {
            if (timeSignal==0) {
              printf("%4.0d %2.0d %2.0d  ", eventId, antennaId, channelId);
            }
            // print out only maximum signal
            if ( (signalTrace[i] ) > maxSignal ) {
              timeSignal = ((startTime + i * binWidth) / nanosecond);
              maxSignal = (signalTrace[i]);
              energyOld = energy;
              thetaOld = theta;
              phiOld = phi;
              xCoreOld = xCore;
              yCoreOld = yCore;
              meanPowerOld = meanPower;
            }
          }
        }

        cout << "Maximum power in the window "<<maxSignal<<endl;
        for (size_t i = 0; i < signalTrace.size(); i++) {
          if ( (signalTrace[i]) > maxSignal ) {
            numberAbove++; 
          } 
          if ( i>i0 && i<i1 ) {
            if ( (signalTrace[i] ) > meanPowerOld ) numberAboveMean++;
            if ( (signalTrace[i] ) > (signalThreshold5 ) ) numberAbove5++;
            if ( (signalTrace[i] ) > (signalThreshold6  ) ) numberAbove6++;
            if ( (signalTrace[i] ) > (signalThreshold7  ) ) numberAbove7++;
            if ( (signalTrace[i] ) > (signalThreshold8  ) ) numberAbove8++;
            if ( (signalTrace[i] ) > (signalThreshold9  ) ) numberAbove9++;
          }

        } 
        
        fractionAbove = double(numberAbove) / double(signalTrace.size()); 
        cout <<"Above mean : " << numberAboveMean << " Above 5 "<<numberAbove5 <<" Above 7 "<<numberAbove7<<" Above 9 "<<numberAbove9<<endl;

        if (timeSignal!=0) {
          // Calculate angle between shower axis and boresight axis
          TVector3 showerAxis = shower.GetAxis();
          TVector3 up(0., 0., 1.);
          // shifted channels for side LNBs
          if (channelId>=10) {
            unsigned int channelId_temp;
            if (channelId==10) channelId_temp=3;
            if (channelId==11) channelId_temp=5;
            if (channelId==12) channelId_temp=7;
            if (channelId==13) channelId_temp=9;
            const Channel& channel_temp = antenna.GetChannel(channelId_temp);
            channel_temp.RotateFromChannelToAntenna(up);
          }
          else {
            channel.RotateFromChannelToAntenna(up);
          }
          //
          antenna.RotateFromAntennaToArray(up);
          angle = up.Angle(showerAxis);
//          if ( (angle*180./M_PI)<4.0 ) hasSignal = true;

          // Calculate angle for given altitude
          double altitude=0*m;
          double coreDistance;
          const int numAltitudes=10000;
          double observationAngle[numAltitudes];
          double obsAngle=(999.0*degree), obsAltitude=0.0*m;
          TVector3 antennaPos = antenna.GetPosition();
          for (int j=0; j<numAltitudes; j++) {
            altitude=1.0*j;
            coreDistance = altitude / cos(shower.GetZenith());
            TVector3 corePosition = event.GetArrayData().GetShower().GetCore();
            TVector3 point = corePosition + coreDistance * showerAxis;
            TVector3 axis = point - antennaPos;
            observationAngle[j] = up.Angle(axis);
            if (observationAngle[j]<obsAngle) {  obsAngle = observationAngle[j];  obsAltitude = altitude;  }
          }

//          if ( (obsAngle < angleThreshold) && (printOut!=0) && (energy>(1.0e16*eV)) ) {  hasSignal = true;  }

          printf( "%5.2f  %4.1f %5.1f %6.1f %6.1f  %7.1f %7.1f  %6.2f %6.2f %6.2f  %5.0f %7.5f  %4.1f  %4.1f  %10.0d  %5.2f %7.1f  %5.0f %5.0f %5.0f %5.0f %5.0f %5.0f  * \n",
            log10(energyOld), thetaOld/degree, phiOld/degree, xCoreOld, yCoreOld,
            timeSignal, (t1-t0), maxSignal, (signalThreshold  / (utl::milli * volt)), meanVoltageOld,
            (numberAbove+0.001), fractionAbove, decibelAboveMean, angle/degree, gpsSec, obsAngle/degree, obsAltitude,
            (numberAboveMean+0.001),(numberAbove5+0.001),(numberAbove6+0.001),(numberAbove7+0.001),(numberAbove8+0.001),(numberAbove9+0.001) );

        }
          //cout << "time signal "<<timeSignal <<"  window "<<(t1-t0)<<"  maxSignal "<<maxSignal<<"  (signalThreshold  / (watt)) "<<(signalThreshold )<<"  meanPowerOld " <<meanPowerOld<<"  (numberAbove+0.001) "<<(numberAbove+0.001)<<"  fractionAbove "<<fractionAbove<<"  decibelAboveMean "<< decibelAboveMean<<" (numberAboveMean+0.001)"<<(numberAboveMean+0.001)<<"  (numberAbove5+0.001) "<<(numberAbove5+0.001)<<"  (numberAbove6+0.001)" << (numberAbove6+0.001)<< " (numberAbove7+0.001) " <<(numberAbove7+0.001)<< " (numberAbove8+001 "<<(numberAbove8+0.001)<< "(numberAbove9+0.001) "<<(numberAbove9+0.001)<<endl;
      //} // if voltage
    } // for channels
  } // for antenna

//else { hasSignal = false; }
  return hasSignal;
}




int main(int argc, char** argv) {
  if ( argc < 4 ) {
    cerr << "Usage: searchEvents level[dB] observationangle[deg] printOut[0==no] timeOffset[mus] input.root [...]" << endl;
    return 1;
  }

  double decibelAboveMean = atof(argv[1]);
  cout << " Threshold level in dB above mean voltage: " << decibelAboveMean << endl;
  double angleThreshold = atof(argv[2]);
  cout << " Threshold level for observation angle: " << angleThreshold << endl;
  int printOut = atof(argv[3]);
  cout << " Parameter for print out [0 = no]: " << printOut << endl;
  double timeOffset = atof(argv[4]);
  cout << " Time offset [mus]: " << timeOffset << endl;

  vector<string> filenames;
  for (int i = 5; i < argc; i++) {
    filenames.push_back(string(argv[i]));
  }

  // Open the input and output files
  EventFile inputFile(filenames);
  EventFile outputFile("candidates.root", EventFile::eWrite);

  // Read the detector setup
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  Detector::GetInstance().InitFromFile(inputFile);

  // Create an event buffer and assign it to the input and output files
  Event* event = new Event();
  inputFile.SetEventBuffer(event);
  outputFile.SetEventBuffer(event);

  // Loop over all events
  //cout << "# UTC - eventId - antennaId - channelId - log10(E) - th - phi - Xc - Yc - eventID - time[ns] - time19[ns] - signal[mV] - threshold[mV]" << endl;
  //cout << "# - mean[mV] - numAbove - fracAbove - threshold[dB] - emission_angle[deg] - gpsSec - observation_angle[deg] - altitude[m] - numAboveMean " << endl;
  //cout << "# - numAbove5 - numAbove6 - numAbove7 - numAbove8 - numAbove9 " << endl;

  int found =0; int count=0;
  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) { cout << "EVENT "<<count <<": "<<event->GetEventId()<<endl;
    if ( IsSignalCandidate(*event, detector, decibelAboveMean, angleThreshold, printOut, timeOffset) ) {
      outputFile.WriteEvent();
      found++;
    }  
    count++;
  } 
  //cout <<found <<" for " <<count <<" events"<<endl;

  // Finalise the output file
  outputFile.WriteDetectorSetup(detector);
  outputFile.cd();

  cout << "Found " << outputFile.GetNumberOfEvents() << " signal candidates" << endl;
}
*/
