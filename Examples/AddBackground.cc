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

  if ( argc < 2 ) {
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
  TGraph*S_graph_WithN=new TGraph();
  TGraph*maxi_graph=new TGraph();
    
  vector <double> S_events, S_events_WithN;
  vector <double> maxi_trace_bck, maxi_trace;
  vector <double> B_mean, B_RMS, B_mean_max, B_RMS_max, B_mean_WithN, B_RMS_WithN; //Background distributions
    
  //loop on signal events
  while (inputFile.ReadNextEvent() == EventFile::eSuccess ) {
    long long eventId = event->GetEventId();
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
    //cout << "E [Eev]= "<<energy/1.e18<<" theta: "<<theta/degree<<" phi: "<<phi/degree<<" x: "<<xCore<<" y: "<<yCore<<endl;
    cout <<"---------------------------------------------------------------------------------------------------------"<<"\n"<<endl;
    TH1F *hist_bck= new TH1F("noise","noise", 250, 1.e-16,5.e-12); //Background with no weight
    TH1F *hist_bck_max= new TH1F("noise_max","noise_max", 250, 1.e-16,5.e-12); // Background weighted with S = Aeff/4*pi*r^2
    TH1F *hist_bck_WithN= new TH1F("noise_N","noise_N", 250, 1.e-16,5.e-12); // Background weighted with S*N%
      
    // loop on antennas
    for ( Event::ConstAntennaDataIterator antennaIter = event->AntennasBegin(); antennaIter != event->AntennasEnd(); ++antennaIter ) {
      const AntennaData& antennaData = *antennaIter;
      const int antennaId = antennaData.GetId();
      const Antenna& antenna = detector.GetAntenna(antennaId);
      // weight vectors
      vector<double>w_x, w_y, w_yWithN ; // time, S, S*N
      SensitivityTrace sensTrace, sensTraceWithN;
        
      TGraph* sensGraph = new TGraph();
      TGraph* sensGraphWithN = new TGraph();
      TGraph* windowedTrace = new TGraph();
      TGraph* weightedTrace = new TGraph();
      TGraph* weightedTraceWithN = new TGraph();
        
      // loop on channels
      for ( AntennaData::ConstChannelDataIterator chanIter =  antennaData.ChannelsBegin(); chanIter != antennaData.ChannelsEnd(); ++chanIter ) {
        const ChannelData& channelData = *chanIter;
        const unsigned int channelId = channelData.GetId();
        const Channel& channel = antenna.GetChannel(channelId);
        vector<Double_t> trace_data, trace_sim, copy_0;
        vector<Double_t> windowed_sig, weighted_sig, weighted_sigWithN;
        trace_data = channelData.GetRealDataTrace();
        trace_sim = channelData.GetSimulatedPowerTrace();
        const unsigned int traceLength = trace_data.size();
        for (int i =0; i< traceLength; i++) copy_0.push_back(0);
          //Reject traces with no data
          if ((trace_data.size()>0) && (trace_data != copy_0)) {
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
              sensTraceWithN =(*event).GetSensitivityTraceWithN(antennaId, channelId, expSignalConeAngle, 2.*meter, 0);
              
              sensGraph = new TGraph();
              sensGraphWithN= new TGraph();
              windowedTrace =new TGraph();
                
              int nn =sensTrace.trace.first.size();
              double sum_weight = 0.;double sum_weightWithN = 0.;
              double sum_binweight = 0.;double sum_binweightWithN = 0.;
              
              double max = *max_element(sensTrace.trace.second.begin(), sensTrace.trace.second.end());
              double maxWithN = *max_element(sensTraceWithN.trace.second.begin(), sensTraceWithN.trace.second.end());
                
              double S=0.;
              double S_WithN=0.;
                
              if (max !=0 && maxWithN !=0) { // avoid problematic sensitivity traces
                int t0_sens=round(sensTrace.trace.first[nn-1]*1.e-6/25.e-9);
                int t1_sens=round(sensTrace.trace.first[0]*1.e-6/25.e-9);
                int size_sens=round(sensTrace.trace.first[0]*1.e-6/25.e-9)-round(sensTrace.trace.first[nn-1]*1.e-6/25.e-9);
                for (int i = 0; i < size_sens; i++ ) {
                  w_x.push_back(t0_sens+i); // time
                  w_y.push_back(sensTrace.trace.second[nn*i/size_sens]/max); // S/S_{max}
                  w_yWithN.push_back(sensTraceWithN.trace.second[nn*i/size_sens]/max); // (S*N%)/(S_{max}*Nmax)
                    
                  sum_weight+=sensTrace.trace.second[nn*i/size_sens]/max;
                  sum_weightWithN+=sensTraceWithN.trace.second[nn*i/size_sens]/maxWithN;
                    
                  sensGraph->SetPoint(sensGraph->GetN(),t0_sens+i, sensTrace.trace.second[nn*i/size_sens]/max);
                    
                  sensGraphWithN->SetPoint(sensGraphWithN->GetN(),t0_sens+i, sensTraceWithN.trace.second[nn*i/size_sens]/maxWithN);
                }// Retrieve weight function
                double a = 0.;
                for (int i  = t0_sens; i<t0_sens+size_sens; i++) {
                  sum_binweight+=(w_y[i-t0_sens]*trace_data[i]);
                  sum_weightWithN+=(w_yWithN[i-t0_sens]*trace_data[i]);
                  windowed_sig.push_back(trace_data[i]);
                  weighted_sig.push_back((w_y[i-t0_sens])*trace_data[i]);
                  weighted_sigWithN.push_back((w_yWithN[i-t0_sens])*trace_data[i]);
                  if (trace_data[i]>a) a =trace_data[i];
                   
                } // fill data and weighted data
                  
                for (int i=0; i<size_sens; i++) {
                  windowedTrace->SetPoint( windowedTrace->GetN(),w_x[i] , windowed_sig[i]);
                  weightedTrace->SetPoint( weightedTrace->GetN(),w_x[i] , weighted_sig[i]);
                  weightedTraceWithN->SetPoint( weightedTraceWithN->GetN(),w_x[i] , weighted_sigWithN[i]);
                } // Fill graphs
                S = double(sum_binweight/sum_weight);
                S_WithN = double(sum_binweightWithN/sum_weightWithN);

                if((sum_weight>0) && (sum_binweight>0)){
                  S_events.push_back(S);
                  S_events_WithN.push_back(S_WithN);
                  cout <<"  \n -------------> S: "<< S <<" sum weight: "<<sum_weight<<" sum bin weight: "<<sum_binweight<<endl;
                  maxi_trace.push_back(a);
                  /***********************************************/
                  // open background trace files
                  string infile_bck = string(argv[2]);
                  EventFile inputFile_bck(infile_bck);
                  // Read the detector setup
                  DetectorSetup detector_bck;
                  inputFile_bck.ReadDetectorSetup(detector_bck);
                  Detector::GetInstance().InitFromFile(inputFile_bck);
                  //New event bck
                  Event* event_bck = new Event();
                  inputFile_bck.SetEventBuffer(event_bck);
                  while (inputFile_bck.ReadNextEvent() == EventFile::eSuccess ) {
                    long long eventId_bck = event_bck->GetEventId();
                    if (eventId_bck==eventId) continue;
                    ArrayData& arrayData_bck = event_bck->GetArrayData();
                    // loop on antennas
                    for ( Event::ConstAntennaDataIterator antennaIter = event_bck->AntennasBegin(); antennaIter != event_bck->AntennasEnd(); ++antennaIter ) {
                      const AntennaData& antennaData_bck = *antennaIter;
                      const int antennaId_bck = antennaData_bck.GetId();
                      const Antenna& antenna_bck = detector_bck.GetAntenna(antennaId_bck);
                      // loop on channels
                      for ( AntennaData::ConstChannelDataIterator chanIter = antennaData_bck.ChannelsBegin(); chanIter != antennaData_bck.ChannelsEnd(); ++chanIter ) {
                        const ChannelData& channelData_bck = *chanIter;
                        const unsigned int channelId_bck = channelData_bck.GetId();
                        const Channel& channel_bck = antenna_bck.GetChannel(channelId_bck);
                        vector<Double_t> trace_data_bck;
                        vector<Double_t> windowed_bck;
                        vector<Double_t> weighted_bck;vector<Double_t> weighted_bckWithN;
                        trace_data_bck = channelData_bck.GetRealDataTrace();
                        double a_bck = 0.;
                        for (int i = 0; i < size_sens; i++) {
                          windowed_bck.push_back(trace_data_bck[i]);
                          //weighted_bck.push_back(w_y[i]*trace_data_bck[i]);
                          weighted_bck.push_back(w_y[i]*trace_data_bck[i]);
                          weighted_bckWithN.push_back(w_yWithN[i]*trace_data_bck[i]);
                          if (trace_data_bck[i]>a_bck) a_bck = trace_data_bck[i];
                          maxi_trace_bck.push_back(a_bck);
                          hist_bck_max->Fill(a_bck);
                          hist_bck_WithN->Fill(w_yWithN[i]*trace_data_bck[i]);
                          hist_bck->Fill(w_y[i]*trace_data_bck[i]);
                            
                        }
                      }//channels
                    }//antennas
                  } //loop on background
                  /***********************************************/
                } //got S and background distribution for each trace
                else cout << inputFile.GetCurrentEventNumber() << " is negative or diverges!" <<endl;
              }
              CountIn++;
            } // End calculation of time windows
            good_trace++;
          } //End sanity check on traces
          
          
          //Example plots
          if ((antennaId == 385 ) && (inputFile.GetCurrentEventNumber() == 29)){
            cout << "S to compare "<<S_events[29]<<endl;
            TCanvas*c = new TCanvas();
            c->Divide(1,4);
            c->cd(1);
            sensGraph->Draw("ALP");
            c->cd(2);
            windowedTrace->Draw("ALP");
            c->cd(3);
            weightedTrace->Draw("ALP");
            c->cd(4);
              hist_bck->SetLineColor(2);
            hist_bck->Draw();
            hist_bck_WithN->SetLineColor(1);
            hist_bck_WithN->Draw("same");
            hist_bck_max->SetLineColor(4);
            hist_bck_max->Draw("same");
              
              double gia = maxi_trace[58];double giS = S_events[58] ;double giSN = S_events_WithN[58];
              double xsa[2] = {gia,gia};double xsS[2] = {giS,giS};double xsSN[2] = {giSN,giSN};
              double ys[2] = {-5000.,5000.};
              TGraph* grapha = new TGraph(2, xsa, ys);TGraph* graphS = new TGraph(2, xsS, ys);TGraph* graphSN = new TGraph(2, xsSN, ys);
              graphSN->SetLineStyle(2); graphSN->SetLineWidth(2);graphSN->SetLineColor(1);
              graphS->SetLineStyle(2); graphS->SetLineWidth(2);graphS->SetLineColor(2);
              grapha->SetLineStyle(2); grapha->SetLineWidth(2);grapha->SetLineColor(4);
              c->cd(4);
              grapha->Draw("lsame"); graphSN->Draw("lsame");graphS->Draw("lsame");
              
              
              
          } // Plots
      } // End loop on channels
        
      // Background on S
      if (hist_bck->GetRMS()!=0){
        B_mean.push_back(hist_bck->GetMean());
        B_RMS.push_back(hist_bck->GetRMS());
      }
      else cout << "0" <<endl;
      // Background on Max
      if (hist_bck_max->GetRMS()!=0){
        B_mean_max.push_back(hist_bck_max->GetMean());
        B_RMS_max.push_back(hist_bck_max->GetRMS());
      }
      else cout << "0" <<endl;
      // Background on S*N
        if (hist_bck_WithN->GetRMS()!=0){
            B_mean_WithN.push_back(hist_bck_WithN->GetMean());
            B_RMS_WithN.push_back(hist_bck_WithN->GetRMS());
        }
        else cout << "0" <<endl;
  
    }// End loop on antennas
    eventcounter++;
  }// End loop on selected events
  
  for (int i =0; i < S_events.size(); i++){
    if (((S_events[i]-B_mean[i])/B_RMS[i]) <= 5.) S_graph->SetPoint(S_graph->GetN(), i , (S_events[i]-B_mean[i])/B_RMS[i]);
    if (((maxi_trace[i]-B_mean_max[i])/B_RMS_max[i]) <= 5.) maxi_graph->SetPoint(maxi_graph->GetN(), i , ((maxi_trace[i]-B_mean_max[i])/B_RMS_max[i]));
    if (((S_events_WithN[i]-B_mean_WithN[i])/B_RMS_WithN[i]) <= 5.) S_graph_WithN->SetPoint(S_graph_WithN->GetN(), i , (S_events_WithN[i]-B_mean_WithN[i])/B_RMS_WithN[i]);
  }
    
  TCanvas *c_all= new TCanvas ("c_all","c_all",800,500);
  c_all->cd();
  S_graph->SetLineColor(2);
  S_graph->Draw();
  S_graph_WithN->SetLineColor(1);
  S_graph_WithN->Draw("same");
  maxi_graph->SetLineColor(4);
  maxi_graph->Draw("same");
    
  
    
    
  cout <<"\n"<<"---------------------------------------------------------------------------------------------------------"<<"\n"<<endl;
  cout << "\n"<<"Total number of events: "<<inputFile.GetNumberOfEvents()<<endl;
  cout <<"Total good traces: "<< good_trace <<endl;
  cout <<"Total showers inside FOVs: "<< CountIn <<endl;

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
