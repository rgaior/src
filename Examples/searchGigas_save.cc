// Clopper test
#ifdef _MSC_VER
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4510) // default constructor could not be generated.
#  pragma warning(disable: 4610) // can never be instantiated - user defined constructor required.
#endif
//
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
#include <TGClient.h>

using namespace std;
using namespace utl;


void confidence_limits_on_frequency(unsigned trials, unsigned successes)
{
   //
   // trials = Total number of trials.
   // successes = Total number of observed successes.
   //
   // Calculate confidence limits for an observed
   // frequency of occurrence that follows a binomial
   // distribution.
   //
   using namespace std;
   using namespace boost::math;

   // Print out general info:
   cout <<
      "___________________________________________\n"
      "2-Sided Confidence Limits For Success Ratio\n"
      "___________________________________________\n\n";
   cout << setprecision(7);
   cout << setw(40) << left << "Number of Observations" << "=  " << trials << "\n";
   cout << setw(40) << left << "Number of successes" << "=  " << successes << "\n";
   cout << setw(40) << left << "Sample frequency of occurrence" << "=  " << double(successes) / trials << "\n";
   //
   // Define a table of significance levels:
   //
   double alpha[] = { 0.319, 0.25, 0.1, 0.05, 0.01, 0.001, 0.0001, 0.00001 };
   //
   // Print table header:
   //
   cout << "\n\n"
           "_______________________________________________________________________\n"
           "Confidence        Lower CP       Upper CP       Lower JP       Upper JP\n"
           " Value (%)        Limit          Limit          Limit          Limit\n"
           "_______________________________________________________________________\n";
   //
   // Now print out the data for the table rows.
   //
   for(unsigned i = 0; i < sizeof(alpha)/sizeof(alpha[0]); ++i)
   {
      // Confidence value:
      cout << fixed << setprecision(3) << setw(10) << right << 100 * (1-alpha[i]);
      // Calculate Clopper Pearson bounds:
      double l = binomial_distribution<>::find_lower_bound_on_p(trials, successes, alpha[i]/2);
      double u = binomial_distribution<>::find_upper_bound_on_p(trials, successes, alpha[i]/2);
      // Print Clopper Pearson Limits:
      cout << fixed << setprecision(5) << setw(15) << right << l;
      cout << fixed << setprecision(5) << setw(15) << right << u;
      // Calculate Jeffreys Prior Bounds:
      l = binomial_distribution<>::find_lower_bound_on_p(trials, successes, alpha[i]/2, binomial_distribution<>::jeffreys_prior_interval);
      u = binomial_distribution<>::find_upper_bound_on_p(trials, successes, alpha[i]/2, binomial_distribution<>::jeffreys_prior_interval);
      // Print Jeffreys Prior Limits:
      cout << fixed << setprecision(5) << setw(15) << right << l;
      cout << fixed << setprecision(5) << setw(15) << right << u << std::endl;
   }
   cout << endl;
} // void confidence_limits_on_frequency()


/* Looping on all data, first, define for each trace a signal window where we expect the signal to arrive on each antenna. extract SNR in each window, and plot SNR distribution of the signal windows. */

vector<vector<double> > GetExpectedSignalSNR(Event& event,DetectorSetup& detector, vector<vector<double> >& SNR) {
  
  ArrayData& arrayData = event.GetArrayData();
  vector <double> SNR_Signal; 
  vector <double> SNR_Noise;
  int counttraces= 0; // Number of traces
  double timeOffset = 0 ;
  // event
  const unsigned int eventId = event.GetEventId();
  //UInt_t gpsSec = event.GetGPSSecond() ; cout <<"---------------------> "<<gpsSec<<endl;
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
      if (channelData.HasRealDataTrace() /*& antennaId !=384*/) // Reject peculiar traces on 384
      {
        expectedSignal = event.GetExpectedSignalWindow(antennaId, channelId, expSignalConeAngle,0);
          
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
  return SNR;
}



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
  /*outputFile.WriteDetectorSetup(detector);
  outputFile.cd();
  cout << "Found " << outputFile.GetNumberOfEvents() << " signal candidates" << endl;*/
  
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
