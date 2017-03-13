#include <ArrayData.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <Array.h>

#include <AugerUnits.h>
#include <MathFunctions.h>

#include <TGraphErrors.h>
#include <TF1.h>

#include <algorithm>
#include <sstream>
#include <stdexcept>


using namespace utl;
using namespace std;


ClassImp(ArrayData)


ArrayData::ArrayData() :
  fType(eUndefined),
  fEventId(0),
  fRunId(0),
  fGPSSecond(0),
  fGPSNanoSecond(0),
  fTriggerFlag1(0),
  fTriggerFlag2(0),
  fTriggerFlag3(0),
  fTriggerFlag4(0),
  fDaqFlag1(0)
{

}


void
ArrayData::AddStationData(StationData stationData)
{
  if ( HasStationData(stationData.GetId()) ) {
    ostringstream errMsg;
    errMsg << "ArrayData::AddStationData(): "
           << "Data for station " << stationData.GetId()
           << " is already present.";
    throw std::runtime_error(errMsg.str());
  }

  fStationData[stationData.GetId()] = stationData;
}


const StationData&
ArrayData::GetStationData(int stationId)
  const
{
  ConstStationDataIterator stationDataIt = fStationData.find(stationId);
  if ( stationDataIt == fStationData.end() ) {
    ostringstream errMsg;
    errMsg << "ArrayData::GetStationData(): "
           << "Could not find data for station " << stationId;
    throw std::runtime_error(errMsg.str());
  }

  return stationDataIt->second;
}


StationData&
ArrayData::GetStationData(int stationId)
{
  StationDataIterator stationDataIt = fStationData.find(stationId);
  if ( stationDataIt == fStationData.end() ) {
    ostringstream errMsg;
    errMsg << "ArrayData::GetStationData(): "
           << "Could not find data for station " << stationId;
    throw std::runtime_error(errMsg.str());
  }

  return stationDataIt->second;
}


bool
ArrayData::HasStationData(int stationId)
  const
{
  return (fStationData.count(stationId) != 0);
}


bool
ArrayData::comparePair(const pair<int, double>& i, const pair<int, double>& j)
{
  return i.second > j.second;
}


pair<int,double>
ArrayData::GetGroundImpactTimeKG(int hotStationIt, bool corrected)
  const
{
  vector<double> hottestStationTime;
  vector<double> groundImpactTime;
  vector< pair<int,double> > hottestStationEnergy;

  for(ConstStationDataIterator stationDataIt = fStationData.begin();
      stationDataIt != fStationData.end();
      ++stationDataIt) {

    const StationData &stationData = stationDataIt->second;

    if ((stationData.HasTriggerTime() || stationData.HasCorrectedTriggerTime()) && stationData.HasEnergyDeposit()) {
      const int stationId = stationData.GetId();
      const double energy = stationData.GetEnergyDeposit();
      hottestStationEnergy.push_back(make_pair(stationId,energy));
    }
  }

  sort(hottestStationEnergy.begin(),hottestStationEnergy.end(),comparePair);

  const int stationId = hottestStationEnergy[hotStationIt].first;
  if(!HasStationData(stationId))
    return make_pair(0,0);

  const StationData &stationData = GetStationData(stationId);

  if(corrected && stationData.HasCorrectedTriggerTime())
    hottestStationTime.push_back(stationData.GetCorrectedTriggerTime());
  else if(stationData.HasTriggerTime())
    hottestStationTime.push_back(stationData.GetTriggerTime());
  else
    return make_pair(0,0);

  const Array& theArray = Detector::GetInstance().GetSetup().GetArray();
  const TVector3& hottestStationPosition = theArray.GetStationPosition(stationId);

  const Shower& theShower = GetShower();
  const TVector3& showerCore = theShower.GetCore();
  const TVector3& showerAxis = theShower.GetAxis();

  // projection of hottest station position on shower plane
  // at ground impact time
  const double ac = showerAxis*showerCore;
  const double aa = showerAxis*showerAxis;
  const double ah = showerAxis*hottestStationPosition;
  const double kk = (ac - ah)/aa;
  TVector3 hottestStationProj = hottestStationPosition + kk*showerAxis;

  // distance hottest station to projection [m] (is equal to k since axis vector
  // is normalized)
  int signk = signum(kk);
  double hottestStationDist = signk*(hottestStationProj-hottestStationPosition).Mag();

  // ground impact time relative to times of grande stations [ns]
  groundImpactTime.push_back(hottestStationTime[0] - hottestStationDist/kSpeedOfLight);

  pair<int,double> GIT = make_pair(stationId,groundImpactTime[0]);

  return GIT;
}


Double_t
ArrayData::GetGroundImpactTimeKG()
  const
{
  // it is possible to average over n stations with the highest signal,
  // but this feature is disabled (n=1).
  const int numHotStations = 1;

  vector< pair<int,double> > groundImpactTime;
  vector<double> distCoreToHottest;

  for(int hotStationIt = 0; hotStationIt < numHotStations; hotStationIt++) {
    pair<int,double> git = GetGroundImpactTimeKG(hotStationIt, true);
    groundImpactTime.push_back(git); 

    const Shower theShower = GetShower();
    const TVector3 showerCore = theShower.GetCore();

    const Array& theArray = Detector::GetInstance().GetSetup().GetArray();
    const TVector3& hottestStationPosition = theArray.GetStationPosition(git.first);

    distCoreToHottest.push_back((showerCore-hottestStationPosition).Mag());
  }

  double groundImpactTimeMean = 0;
  double totalWeight = 0;
  for(int j = 0; j < int(groundImpactTime.size()); j++) {
    if(distCoreToHottest[j] > 200*meter)
      continue;
    double weight = 1./distCoreToHottest[j];
    groundImpactTimeMean += groundImpactTime[j].second*weight;
    totalWeight += weight;
  }

  if(totalWeight != 0)
    groundImpactTimeMean = groundImpactTimeMean/totalWeight;

  return groundImpactTimeMean;
}


Double_t
ArrayData::GetGroundImpactTimeCrome(int stationId) 
const
{ 
  // Delay found between signals in Crome and calculated time windows of
  // expected signal. Almost no delay before the installation of a discriminator
  // in the digitalization of station 19 on August 2nd.
 /* double cromeDaqDelay;
  if(fGPSSecond < 996278415)
    cromeDaqDelay = -5.1*nanosecond;
  else
    cromeDaqDelay = 101.7*nanosecond;

  // values need to be stored in root file. For now use hard-coded values
  //const double signalTimeDelay = theArray.GetSignalTimeDelay();
  const double signalTimeDelay = 44*nanosecond + 47*nanosecond + 3075*nanosecond + cromeDaqDelay;
  const double groundImpactTimeKG = GetGroundImpactTimeKG();
  double timeDigStation;
  double signalStartTime;

  if(HasStationData(stationId)) {
    const StationData &stationData = GetStationData(stationId);
    // time of first particle at digitized station in KG time
    if(stationData.HasCorrectedTriggerTime())
      timeDigStation = stationData.GetCorrectedTriggerTime();
    else if(stationData.HasTriggerTime())
      timeDigStation = stationData.GetTriggerTime();
    else
      return 0.;
    signalStartTime = stationData.GetSignalTriggerTime();
  }
  else
    return 0.;
*/
  // signal of dig. station at crome corrected for internal station delays and
  // time difference of ground impact particles and arrival at station
  //double groundImpactTimeCr = signalStartTime - signalTimeDelay - (timeDigStation - groundImpactTimeKG);
  double signalStartTime;
  double timeDigStation = 70*ns; //propagation in PMT 
  double signalTimeDelay = 35*ns; // delay due to power detector
  if (HasStationData(stationId)) {
    const StationData &stationData = GetStationData(stationId) ; 
    signalStartTime = stationData.GetSignalStartTime() ;
  }
  double groundImpactTimeCr = signalStartTime - timeDigStation + signalTimeDelay ; //this is to synchronize time frames
  return groundImpactTimeCr;
}


void ArrayData::CalculateCorrectedTriggerTimes()
{
  const Array& array = Detector::GetInstance().GetSetup().GetArray();
  const Shower shower = GetShower();
  const TVector3 showerCore = shower.GetCore();
  const TVector3 showerAxis = shower.GetAxis();

  // Get ground impact times using all stations as a reference within 300m of
  // shower core
  vector<int> ids;
  vector<double> vdist, vdisterr, vtime, vtimeerr;
  for(unsigned int i = 0; i < array.GetNumberOfStations()-1; i++) {
    pair<int,double> git = GetGroundImpactTimeKG(i, false);
    const int stationId = git.first;
    const double time = git.second;
    if(stationId > 0) {
      const TVector3& stationPos = array.GetStationPosition(stationId);
      const double ac = showerAxis*showerCore;
      const double aa = showerAxis*showerAxis;
      const double ah = showerAxis*stationPos;
      const double kk = (ac - ah)/aa;
      TVector3 stationProj = showerCore + kk*showerAxis;
      const double dist = (stationProj-stationPos).Mag();
      if(dist <= 300*meter) {
        ids.push_back(stationId);
        vdist.push_back(dist);
        vdisterr.push_back(6*meter);
        vtime.push_back(time);
        vtimeerr.push_back(5.6*nanosecond);
      }
    }
  }

  TGraphErrors *sTimeVsDist = new TGraphErrors(ids.size(),&vdist[0],&vtime[0],&vdisterr[0],&vtimeerr[0]);

  double lowestChi2 = 10000.;
  TF1 *bestfit;
  for(int i = 0; i < sTimeVsDist->GetN(); i++) {
    TF1 *fit = new TF1("fit","pol1",0,300);
    TGraphErrors *copy = (TGraphErrors*)sTimeVsDist->Clone("copy");
    copy->RemovePoint(i);
    copy->Fit(fit,"RQ0");

    if(fit->GetChisquare()/fit->GetNDF() < lowestChi2) {
      lowestChi2 = fit->GetChisquare()/fit->GetNDF();
      bestfit = (TF1*)fit->Clone("bestfit");
    }

    fit->Delete();
    copy->Delete();

  }

  sTimeVsDist->Delete();

  for(unsigned int i = 0; i < ids.size(); i++) {
    if(!HasStationData(ids[i]))
      continue;

    double groundImpactTimeCorr = bestfit->Eval(vdist[i]);

    const TVector3& stationPosition = array.GetStationPosition(ids[i]);
    const double ac = showerAxis*showerCore;
    const double aa = showerAxis*showerAxis;
    const double ah = showerAxis*stationPosition;
    const double kk = (ac - ah)/aa;
    TVector3 stationProj = stationPosition + kk*showerAxis;

    int signk = signum(kk);
    double dd = signk*(stationProj-stationPosition).Mag();

    StationData &stationData = GetStationData(ids[i]);
    const double correctedTriggerTime = groundImpactTimeCorr + dd/kSpeedOfLight;

    stationData.SetCorrectedTriggerTime(correctedTriggerTime);
  }

  return;

}


void ArrayData::CalculateCorrectedTriggerTimes2()
{

  const Array& array = Detector::GetInstance().GetSetup().GetArray();
  const Shower shower = GetShower();
  const TVector3 showerCore = shower.GetCore();
  const TVector3 showerAxis = shower.GetAxis();

  // Get ground impact times using all stations as a reference within 300m of
  // shower core
  vector<int> ids;
  vector<double> vdist, vdisterr, vtime, vtimeerr;
  for(unsigned int i = 0; i < array.GetNumberOfStations()-1; i++) {
    pair<int,double> git = GetGroundImpactTimeKG(i, false);
    const int stationId = git.first;
    const double time = git.second;
    if(stationId > 0) {
      const TVector3& stationPos = array.GetStationPosition(stationId);
      const double ac = showerAxis*showerCore;
      const double aa = showerAxis*showerAxis;
      const double ah = showerAxis*stationPos;
      const double kk = (ac - ah)/aa;
      TVector3 stationProj = showerCore + kk*showerAxis;
      const double dist = (stationProj-stationPos).Mag();
      if(dist <= 300*meter) {
        ids.push_back(stationId);
        vdist.push_back(dist);
        vdisterr.push_back(6*meter);
        vtime.push_back(time);
        vtimeerr.push_back(5.6*nanosecond);
      }
    }
  }

  vector<double> S1, Sx, Sxx, Sy, Sxy, Syy;
  double tS1 = 0;
  double tSx = 0;
  double tSxx = 0;
  double tSy = 0;
  double tSxy = 0;
  double tSyy = 0;

  double a1, a2, chi2;

  for (unsigned int i = 0; i < vdist.size(); ++i) {
    if (vtimeerr[i] > 0.) {
      const double wi = 1./(vtimeerr[i]*vtimeerr[i]);
      const double wX = wi * vdist[i];
      const double wY = wi * vtime[i];
      S1.push_back(wi);
      Sx.push_back(wX);
      Sxx.push_back(wX * vdist[i]);
      Sy.push_back(wY);
      Sxy.push_back(wX * vtime[i]);
      Syy.push_back(wY * vtime[i]);

      tS1  += wi;
      tSx  += wX;
      tSxx += wX * vdist[i];
      tSy  += wY;
      tSxy += wX * vtime[i];
      tSyy += wY * vtime[i];
    }
  }

  // Linear fit to all points
  const double D = tS1*tSxx - tSx*tSx;
  a1 = (tSxx*tSy - tSx * tSxy)/D;
  a2 = (-tSx*tSy + tS1*tSxy)/D;
  chi2 = tSyy - a1*tSy - a2*tSxy;

  // Remove contributions of single points to find outlier
  double rS1, rSx, rSxx, rSy, rSxy, rSyy;
  for(unsigned int i = 0; i < S1.size(); i++) {
    rS1  = tS1  - S1[i];
    rSx  = tSx  - Sx[i];
    rSxx = tSxx - Sxx[i];
    rSy  = tSy  - Sy[i];
    rSxy = tSxy - Sxy[i];
    rSyy = tSyy - Syy[i];

    const double rD = rS1*rSxx - rSx*rSx;
    const double ra1 = (rSxx*rSy - rSx*rSxy)/rD;
    const double ra2 = (-rSx*rSy + rS1*rSxy)/rD;
    const double rchi2 = rSyy - ra1*rSy - ra2*rSxy;

    if(rchi2 < chi2) {
      chi2 = rchi2;
      a1 = ra1;
      a2 = ra2;
    }
  }

  // Set corrected trigger times
  for(unsigned int i = 0; i < ids.size(); i++) {
    if(!HasStationData(ids[i]))
      continue;

    double groundImpactTimeCorr = a1 + a2*vdist[i];

    const TVector3& stationPosition = array.GetStationPosition(ids[i]);
    const double ac = showerAxis*showerCore;
    const double aa = showerAxis*showerAxis;
    const double ah = showerAxis*stationPosition;
    const double kk = (ac - ah)/aa;
    TVector3 stationProj = stationPosition + kk*showerAxis;

    int signk = signum(kk);
    double dd = signk*(stationProj-stationPosition).Mag();

    StationData &stationData = GetStationData(ids[i]);
    const double correctedTriggerTime = groundImpactTimeCorr + dd/kSpeedOfLight;

    stationData.SetCorrectedTriggerTime(correctedTriggerTime);
  }

  return;

}

