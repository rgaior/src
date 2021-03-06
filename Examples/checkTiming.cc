#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <utility>

#include <Event.h>
#include <Detector.h>
#include <Antenna.h>
#include <Channel.h>
#include <DetectorSetup.h>
#include <EventFile.h>
#include <StationData.h>
#include <ArrayData.h>
#include <Array.h>
#include <Atmosphere.h>
#include <TimeStamp.h>
#include <MathFunctions.h>

#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TROOT.h>

using namespace std;
using namespace utl;


enum ESelFlag {
  eInit,
  eFill,
  eFinish
};

//------------------------------------------------
bool MakePlots(Event& event, ESelFlag flag) {

  static vector<TH1D*> hsimtrace;
  static vector<TH1D*> hdatatrace;

  static TH1D* hDiffTime_nocal1;
  static TH1D* hDiffTime_nocal2;
  static TH1D* hDiffTime1;
  static TH1D* hDiffTime2;

  static TH1D* hDistHottest;
  static TH1D* hDistHottestDiff;

  static TH1D* hWindowSize1;
  static TH1D* hWindowSize2;
  static TH1D* hDiffWindowLow1;
  static TH1D* hDiffWindowLow2;
  static TH1D* hDiffWindowHigh1;
  static TH1D* hDiffWindowHigh2;

  static TGraph* sTimeVsDist;
  static TGraph* sTimeVsDist19;

  if ( flag == eInit ) {
      hDiffTime1 = new TH1D("diffTime1","diff in simulated vs. station 1 time",30,-150.,150);
      hDiffTime1->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffTime2 = new TH1D("diffTime2","diff in simulated vs. station 2 time",30,-150.,150);
      hDiffTime2->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffTime_nocal1 = new TH1D("diffTime_nocal1","diff in simulated vs. station 1 time without calib",30,-150.,150);
      hDiffTime_nocal1->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffTime_nocal2 = new TH1D("diffTime_nocal2","diff in simulated vs. station 2 time without calib",30,-150.,150);
      hDiffTime_nocal2->GetXaxis()->SetTitle("diff. time [ns]");

      hDistHottest = new TH1D("distHottest","distance of core to hottest station",30,0.,150.);
      hDistHottestDiff = new TH1D("distHottestDiff","difference in ground impact time dep. on hottest station",60,-150.,150.);

      hWindowSize1 = new TH1D("windowSize1","size of expected signal window",21,-105.,105);
      hWindowSize1->GetXaxis()->SetTitle("window size [ns]");
      hWindowSize2 = new TH1D("windowSize2","size of expected signal window",21,-105.,105);
      hWindowSize2->GetXaxis()->SetTitle("window size [ns]");

      hDiffWindowLow1 = new TH1D("diffWindowLow1","diff to lower window bound 3^{o}",31,-155.,155);
      hDiffWindowLow1->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffWindowLow2 = new TH1D("diffWindowLow2","diff to lower window bound 0.8^{o}",31,-155.,155);
      hDiffWindowLow2->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffWindowHigh1 = new TH1D("diffWindowHigh1","diff to upper window bound 3^{o}",31,-155.,155);
      hDiffWindowHigh1->GetXaxis()->SetTitle("diff. time [ns]");
      hDiffWindowHigh2 = new TH1D("diffWindowHigh2","diff to upper window bound 0.8^{o}",31,-155.,155);
      hDiffWindowHigh2->GetXaxis()->SetTitle("diff. time [ns]");

      sTimeVsDist = new TGraph;
      sTimeVsDist19 = new TGraph;

      return true;
  }
  else if ( flag == eFinish ) {
    for(unsigned int i = 0; i < hsimtrace.size(); i++)
      hsimtrace[i]->Write();
    for(unsigned int i = 0; i < hdatatrace.size(); i++)
      hdatatrace[i]->Write();

    hDiffTime1->Write();
    hDiffTime2->Write();
    hDiffTime_nocal1->Write();
    hDiffTime_nocal2->Write();

    hDistHottest->Write();
    hDistHottestDiff->Write();

    hWindowSize1->Write();
    hWindowSize2->Write();
    hDiffWindowLow1->Write();
    hDiffWindowLow2->Write();
    hDiffWindowHigh1->Write();
    hDiffWindowHigh2->Write();

    sTimeVsDist->Write("sTimeVsDist");
    sTimeVsDist19->Write("sTimeVsDist19");

    return true;
  }
  else {
    const double eventId = event.GetEventId();

    const DetectorSetup detSetup = Detector::GetInstance().GetSetup();
    const Array& array = detSetup.GetArray();

    ArrayData& arrayData = event.GetArrayData();
    const Shower shower = arrayData.GetShower();
    const TVector3 showerCore = shower.GetCore();
    const TVector3 showerAxis = shower.GetAxis();

    //arrayData.CalculateCorrectedTriggerTimes();
    arrayData.CalculateCorrectedTriggerTimes2();

    // Get ground impact time in Crome time using the signal from any
    // Grande station that was digitized.
    double groundImpactTime = 0.;
    double stationTime = 0.;
    for ( Array::ConstStationIterator iter = array.StationsBegin();
          iter != array.StationsEnd(); ++iter ) {
      int stationId = iter->first;
      if(!arrayData.HasStationData(stationId))
        continue;
      const StationData& stationData = arrayData.GetStationData(stationId);
      if(!stationData.HasTriggerTime() || !stationData.HasCorrectedTriggerTime())
        continue;
      const double triggerTime = stationData.GetTriggerTime();
      const double correctedTriggerTime = stationData.GetCorrectedTriggerTime();

cout << stationId << "  " << triggerTime << "  " << correctedTriggerTime << endl;

      const TVector3& stationPos = array.GetStationPosition(stationId);
      if (stationData.HasSignal()) {
        stationTime = stationData.GetSignalTriggerTime();
        groundImpactTime = arrayData.GetGroundImpactTimeCrome(stationId);
      }

      /*const double ac = showerAxis*showerCore;
      const double aa = showerAxis*showerAxis;
      const double ah = showerAxis*stationPos;
      const double kk = (ac - ah)/aa;
      TVector3 stationProj = showerCore + kk*showerAxis;
      const double dist = (stationProj-stationPos).Mag();

      sTimeVsDist->SetPoint(sTimeVsDist->GetN(),dist,triggerTime);
      if(stationId == 19)
        sTimeVsDist19->SetPoint(sTimeVsDist19->GetN(),dist,triggerTime);*/
    }

    vector<double> groundImpacts;
    for(int i = 0; i < array.GetNumberOfStations()-1; i++) {
      pair<int,double> git = arrayData.GetGroundImpactTimeKG(i,true);
      if(git.first > 0) {
        const StationData& stationData = arrayData.GetStationData(git.first);
        const TVector3& stationPos = array.GetStationPosition(git.first);
        //const double dist = (showerCore-stationPos).Mag();

        const double ac = showerAxis*showerCore;
        const double aa = showerAxis*showerAxis;
        const double ah = showerAxis*stationPos;
        const double kk = (ac - ah)/aa;
        TVector3 stationProj = showerCore + kk*showerAxis;
        const double dist = (stationProj-stationPos).Mag();

        groundImpacts.push_back(git.second);
        sTimeVsDist->SetPoint(sTimeVsDist->GetN(),dist,git.second);
        if(git.first == 19)
          sTimeVsDist19->SetPoint(sTimeVsDist19->GetN(),dist,git.second);
      }
    }

    double startTime, endTime;
    if(event.GetGPSSecond() < 996278415) {
      // before 2011-08-02
      startTime = stationTime - 3.020*microsecond;
      endTime = stationTime - 2.920*microsecond;
    }
    else {
      // after 2011-08-02
      startTime = stationTime - 3.140*microsecond;
      endTime = stationTime - 3.040*microsecond;
    }

    /*vector< pair<int,double> > groundImpactTimeKG;
    vector<double> distCoreToHottest;

    for(int hotStationIt = 0; hotStationIt < 3; hotStationIt++) {
      pair<int,double> git = arrayData.GetGroundImpactTimeKG(hotStationIt);
      groundImpactTimeKG.push_back(git);

      const Array& theArray = Detector::GetInstance().GetSetup().GetArray();
      const TVector3& hottestStationPosition = theArray.GetStationPosition(git.first);

      distCoreToHottest.push_back((showerCore-hottestStationPosition).Mag());
    }

    hDistHottest->Fill(distCoreToHottest[0]);

    for(int i = 1; i < 3; i++) {
      if(distCoreToHottest[i] < distCoreToHottest[0] + 30) {
        cout << i << "  " << groundImpactTimeKG[i].first << "  " << groundImpactTimeKG[i].second-groundImpactTimeKG[0].second << "  " << distCoreToHottest[i] << "  " << distCoreToHottest[0] << endl;
        hDistHottestDiff->Fill(groundImpactTimeKG[i].second-groundImpactTimeKG[0].second);
      }
    }*/

    for(int antennaId = 1; antennaId < 4; antennaId++) {

      if(!event.HasAntennaData(antennaId))
        continue;

      const Antenna& antenna = detSetup.GetAntenna(antennaId);
      const AntennaData& antennaData = event.GetAntennaData(antennaId);

      for (AntennaData::ConstChannelDataIterator chanIter = antennaData.ChannelsBegin();
           chanIter != antennaData.ChannelsEnd();
           ++chanIter) {

        const ChannelData& channelData = *chanIter;
        const unsigned int channelId = channelData.GetId();

        const vector<Double_t>& trace_sim = channelData.GetSimulatedPowerTrace();
        const vector<Double_t>& trace_data = channelData.GetDataTrace();

        const unsigned int traceLength = trace_data.size();
        const unsigned int traceLengthSim = trace_sim.size();

        const double t1 = channelData.GetDataStartTime() / microsecond;

        const Channel& channel = antenna.GetChannel(channelId);
        const double traceBinWidth = channel.GetTraceBinWidth();
        const double t2 = t1 + traceLength * traceBinWidth / microsecond;

        const TVector3 pos = channel.GetGlobalPosition();

        // Fix fake side LNB positions
        const TVector3& pointing =
          channelId == 10 ? antenna.GetChannel(3).GetPointingDirection() :
          channelId == 11 ? antenna.GetChannel(5).GetPointingDirection() :
          channelId == 12 ? antenna.GetChannel(7).GetPointingDirection() :
          channelId == 13 ? antenna.GetChannel(9).GetPointingDirection() :
          channel.GetPointingDirection();

        vector<TVector3> intersection = 
          RayConeIntersection(showerCore,showerAxis,pos,pointing,3*degree);

        const double bandWidth = channel.GetBandWidth();
        const double noiseTemperature = channel.GetNoise();
        const double effectiveArea = channel.GetBoresightEffectiveArea();
        const double noisePower = kBoltzmann * noiseTemperature * bandWidth;
        const double RC = 4.0 * nanosecond;
        const double detectionThreshold = (noisePower + kBoltzmann * noiseTemperature / (effectiveArea * sqrt(RC * bandWidth))) / watt;

        stringstream histname;
        histname << eventId << "_" << antennaId << "_" << channelId;
        TH1D* traceHist_sim = new TH1D(("simulated"+histname.str()).c_str(), ";Time [#mus];Received Power [dBm]", traceLength, t1, t2);
        TH1D* traceHist_data = new TH1D(("measured"+histname.str()).c_str(), ";Time [#mus];Signal [mV]", traceLength, t1, t2);

        bool detected = false;

        for (unsigned int k = 0; k < traceLengthSim; k++) {
          double fillValue = (trace_sim[k] + noisePower) / watt;
          if(fillValue > detectionThreshold)
            detected = true;
          traceHist_sim->SetBinContent(k + 1, fillValue);
        }

        double mean = 0;
        for (unsigned int k = 0; k < traceLength; k++) {
          double fillValue = trace_data[k] / (utl::milli * volt);
          mean += fillValue/traceLength;
        }

        // Calculate angle between shower axis and boresight axis
        TVector3 up(0., 0., 1.);
        // shifted channels for side LNBs
        if (channelId >= 10) {
          unsigned int channelId_temp;
          if (channelId == 10) channelId_temp = 3;
          if (channelId == 11) channelId_temp = 5;
          if (channelId == 12) channelId_temp = 7;
          if (channelId == 13) channelId_temp = 9;
          const Channel& channel_temp = antenna.GetChannel(channelId_temp);
          channel_temp.RotateFromChannelToAntenna(up);
        }
        else {
          channel.RotateFromChannelToAntenna(up);
        }
        antenna.RotateFromAntennaToArray(up);
        double emisAngle = up.Angle(showerAxis);

        // Calculate angle for given altitude
        double altitude = 0.;
        double coreDistance;
        const int numAltitudes = 10000;
        double observationAngle[numAltitudes];
        double obsAngle = 999.;
        TVector3 antennaPos = antenna.GetPosition();
        for (int j = 0; j < numAltitudes; j++) {
          altitude = 1.*j;
          coreDistance = altitude / cos(shower.GetZenith());
          TVector3 corePosition = event.GetArrayData().GetShower().GetCore();
          TVector3 point = corePosition + coreDistance * showerAxis;
          TVector3 axis = point - antennaPos;
          observationAngle[j] = up.Angle(axis);
          if (observationAngle[j] < obsAngle)
            obsAngle = observationAngle[j];
        }

        double peak_data = 0;
        for (unsigned int k = 0; k < traceLength; k++) {
          double fillValue = trace_data[k] / (utl::milli * volt);
          traceHist_data->SetBinContent(k + 1, fillValue);
          double signal = (mean-fillValue)/2.5;
          //if(fillValue < -1.7 && traceHist_data->GetBinCenter(k) < -4.8 && traceHist_data->GetBinCenter(k) > -5.2) {
          //if(signal > 7. && traceHist_data->GetBinCenter(k) < -4.8 && traceHist_data->GetBinCenter(k) > -5.2) {
          double peak = traceHist_data->GetBinCenter(k)*microsecond;
          if(signal > 8. && emisAngle < 4.*degree && obsAngle < 3.*degree && peak < endTime && peak > startTime) {
            peak_data = traceHist_data->GetBinCenter(k)*microsecond;
          }
        }

        if(detected && peak_data != 0) {
          double peak_sim = traceHist_sim->GetBinCenter(traceHist_sim->GetMaximumBin())*microsecond;
          TimeStamp ts(event.GetGPSSecond());

          cout << eventId << "\t" << antennaId << "\t" << channelId << "\t" << event.GetGPSSecond() << "\t"
            //<< ts.GetYear() << "-" << ts.GetMonth() << "-" << ts.GetDay() << "\t" << ts.GetHour() << ":" << ts.GetMinute() << ":" << ts.GetSecond() << "\t\t"
            << peak_data << "\t" << peak_sim+groundImpactTime << flush; // "\t" << (peak_data-groundImpactTime)-peak_sim << endl;

          vector<double> window1 = event.GetExpectedSignalWindow(antennaId,channelId,3*degree);
          vector<double> window2 = event.GetExpectedSignalWindow(antennaId,channelId,0.8*degree);

          if(window1.size() != 0)
            cout << "\t" << window1[0] << "\t" << window1[1] << flush;
          else
            cout << "\t0\t0" << flush;

          if(window2.size() != 0)
            cout << "\t" << window2[0] << "\t" << window2[1] << endl;
          else
            cout << "\t0\t0" << endl;

          /*if(window1.size() != 0) {
            hWindowSize1->Fill(window1[1]-window1[0]);
            hDiffWindowLow1->Fill(peak_data - window1[0]);
            hDiffWindowHigh1->Fill(window1[1] - peak_data);
            if(peak_data-window1[0] >= 0 && window1[1]-peak_data >= 0)
              cout << "<<<3.0   inside window: " << window1[0] << "  " << window1[1] << endl;
            else
              cout << "<<<3.0   outside window: " << window1[0] << "  " << window1[1] << endl;
          }
          else
            cout << "<<<3.0   no cone intersection" << endl;

          if(window2.size() != 0) {
            hWindowSize2->Fill(window2[1]-window2[0]);
            hDiffWindowLow2->Fill(peak_data - window2[0]);
            hDiffWindowHigh2->Fill(window2[1] - peak_data);
            if(peak_data-window2[0] >= 0 && window2[1]-peak_data >= 0)
              cout << "<<<0.8   inside window: " << window2[0] << "  " << window2[1] << endl;
            else
              cout << "<<<0.8   outside window: " <<  window2[0] << "  " << window2[1] << endl;
          }
          else
            cout << "<<<0.8   no cone intersection" << endl;
            */

          // 5 May 2012 6:00
          if(event.GetGPSSecond() < 1020405615) {
            if(antennaId == 1)
              hDiffTime1->Fill((peak_data-groundImpactTime)-peak_sim);
            else if(antennaId == 2)
              hDiffTime2->Fill((peak_data-groundImpactTime)-peak_sim);
          }
          if((peak_data-groundImpactTime)-peak_sim > 50) {
            if(antennaId == 1)
              hDiffTime_nocal1->Fill((peak_data-groundImpactTime)-peak_sim);
            else if(antennaId == 2)
              hDiffTime_nocal2->Fill((peak_data-groundImpactTime)-peak_sim);
          }
          hsimtrace.push_back(traceHist_sim);
          hdatatrace.push_back(traceHist_data);
        }
      }
    }

    return true;
  }

  return false;
}


int main(int argc, char** argv) {

  if ( argc < 2 ) {
    cerr << " usage: checkTiming <Files> " << endl;
    return 1;
  }

  int j = 1;
  vector<string> fileNames;
  while ( argv[j] != NULL ) {
    fileNames.push_back(string(argv[j]));
    j++;
  }

  Event* event = new Event();
  EventFile inputFile(fileNames);
  inputFile.SetEventBuffer(event);
  Detector::GetInstance().InitFromFile(inputFile);
  DetectorSetup detector;
  inputFile.ReadDetectorSetup(detector);
  EventFile outputFile("checkTiming.root", EventFile::eWrite);
  outputFile.SetEventBuffer(event);

  MakePlots(*event, eInit);

  while ( inputFile.ReadNextEvent() == EventFile::eSuccess ) {

    /*if( !event->HasSimData() ) {
      cerr << " No Sim Data in the event ... please check!" << endl;
      return 2;
    }*/

    MakePlots(*event, eFill);

  }

  outputFile.cd();
  MakePlots(*event, eFinish);
  outputFile.Close();

  return 0;

}

