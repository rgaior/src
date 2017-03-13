#include <boost/foreach.hpp>
#include <TRandom3.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <GHShowerParameterization.h>
#include <Configuration.h>
#include <GHShowerGenerator.h>
#include <TraceSimulator.h>
#include <BackgroundSimulator.h>
#include <ElectronicsSimulator.h>
#include <KGSimulator.h>
#include <Event.h>
#include <EventFile.h>
#include <Detector.h>
#include <DetectorSetup.h>
#include <TimedClass.h>
#include <AugerUnits.h>
#include <Atmosphere.h>
#include <PhysicalConstants.h>
#include <BackgroundSimulator.h>
#include <ElectronicsSimulator.h>
#include <KGSimulator.h>

using namespace std;
using namespace utl;

//Initialize simulation
TraceSimulator* gTraceSimulator = NULL;
GHShowerParameterization* gGHParameters = NULL;
KGSimulator* kgSimulator = NULL;

void fillEvent(string name,int i, Event*);
void SimulateShower(string dataFilename, Event& theEvent, DetectorSetup& detectorsetup);
void FillDataTraces(string dataFilename, Event& theEvent,  const Detector& detector, DetectorSetup& detectorsetup);
void addChannels(Event& event) {
  BOOST_FOREACH(Antenna antenna, Detector::GetInstance().GetSetup().GetAntennas()) {
    event.MakeAntennaData(antenna.GetId());
    AntennaData& antennaData = event.GetAntennaData(antenna.GetId());
    BOOST_FOREACH(Channel channel, antenna.GetChannels()) {
      antennaData.MakeChannelData(channel.GetId());
    }
  }
}

/************************************************************************************/
int main(int argc, char** argv)
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " Config.xml <dat files> [...]" << endl;
    return 1;
  }
   
  // Detector configuration
  Configuration& config = Configuration::GetInstance();
  config.Init(string(argv[1]));
  //Initialize detector (array, antennas...)
  Detector::GetInstance().Init(); 
  cout << "Intializing detector [OK] ..."<<endl;
  const Detector& detector = Detector::GetInstance();
  // Create a detector setup based on the configuration
  const DetectorSetup& detectorSetup = Detector::GetInstance().GetSetup();
  DetectorSetup detectorsetup;
  EventFile testFile("Helix_test.root", EventFile::eWrite); //output file 
  const int nFiles = argc -2;
  const unsigned int nEvents = nFiles;
  vector<string> datFilenames;
  
  //Read .dat files
  for ( int i = 0 ; i < nFiles ; ++i )  datFilenames.push_back(string(argv[2+i]));
  //Loop on events
  int evt = 0;
  for ( unsigned int i = 0 ; i < nEvents ; i++ ) { 
    gTraceSimulator = new TraceSimulator(&detectorsetup);
    gGHParameters = new GHShowerParameterization(100);
    kgSimulator = new KGSimulator();
    //Start a fresh event
    Event* testEvent = new Event(); 
    testFile.SetEventBuffer(testEvent);
    //Fill simulated traces
    SimulateShower(datFilenames[i],*testEvent, detectorsetup); 
    cout << "Simulating showers [OK] ......."<<endl;
    // Fill data traces
    FillDataTraces(datFilenames[i],*testEvent, detector, detectorsetup); 
    cout << "Acquiring Data traces [OK]......"<<endl;
    testFile.WriteEvent();
    evt++;
  }
  cout << "--> Total number of events: "<<evt<<endl;
  testFile.WriteDetectorSetup(Detector::GetInstance().GetSetup());
  testFile.Close();
}

/*************************************************************************************/

void FillDataTraces(string dataFilename, Event& theEvent, const Detector& detector, DetectorSetup& detectorsetup) { 

  ifstream datfile; 
  const Array& array = detectorsetup.GetArray();
  const std::vector<Antenna> Antennas = detector.GetSetup().GetAntennas();
  //Loop on antennas
  for(unsigned int iAntenna = 0 ; iAntenna < Antennas.size() ; iAntenna++) {
    const Antenna& antenna = Antennas[iAntenna];
    const unsigned int antennaId = antenna.GetId(); 
    AntennaData& antennaData = (&theEvent)->GetAntennaData(antennaId);
    ArrayData& arrayData = (&theEvent)->GetArrayData();
    const std::vector<Channel>& Channels = antenna.GetChannels();
    //Loop on channels
    for(unsigned int iCh=0; iCh<Channels.size(); iCh++) {
      StationData& theStation = arrayData.GetStationData(antennaId);
      const Channel& channel = Channels[iCh];
      const unsigned int channelId = channel.GetId();
      cout << " Antenna "<< antennaId;
      //Read data files
      ostringstream filename;
      string line; 
      int countline =0;  
      double starttime; int starttime_bin ;
      int stat_id, bin, adc;
      filename << string(dataFilename);
      cout << " 1 " << endl;
      datfile.open(filename.str().c_str());
      //Initialize time traces
      cout << " 2 " << endl;
      ChannelData& channelData = antennaData.GetChannelData(channelId);
      cout << " 3 " << endl;
      channelData.MakeRealDataTrace(channelData.GetRealDataStartTime(), channelData.GetSimulationBinWidth(), channelData.GetSimulationBinCount());
      cout << " 4 " << endl;
      std::vector<double>& trace = channelData.GetRealDataTrace(); //calibrated trace
      //std::vector<double> traceadc = theStation.GetSignal() ; //raw ADC trace
      double baseline;  
      cout << " 5 " << endl;
      if(datfile.is_open()) { int count=0;
        while(1) {
          if(!datfile.good()) break;
          getline(datfile,line);
          size_t pos = line.find("Station:") ;
	  cout << " 6 " << endl;
          if(pos!=std::string::npos) {
	    cout << " 7 " << endl;
	    string station = line.substr(pos+9); 
	    cout << "station =  " << station << endl;
	    string starttimes = line.substr(pos+22,3);
	    cout << "starttimes = " << starttimes <<endl;
	    cout << " 9 " << endl;
	    stat_id = atoi(station.c_str());
            starttime_bin = atoi(starttimes.c_str());
            starttime=starttime_bin*channel.GetTraceBinWidth();
            baseline=10.;
	    //            baseline=atof(line.substr(pos+26,5).c_str());
	    cout << " 10 " << endl;
            //Keep stations with data signal  
            if(stat_id!=antennaId) {
              for(int ii=0 ; ii<768 ; ii++)
		datfile>>bin>>adc;
              continue;
            } 
            else {
              count++;
              cout <<"---> Station "<<antennaId <<"-> found in data."<<endl;
              AntennaData theAntenna; 
              theAntenna.SetId(stat_id); 
              theStation.SetId(stat_id) ; 
              theStation.SetSignalStartTime(starttime); // this to synchronize time frames
              cout << setprecision(5)<<starttime<<endl;
              //Transform from ADC counts to W (Private comm. with C.Berat)
              for(int ii=0 ; ii < 768 ; ii++) { 
                datfile>>bin>>adc;
 	        /*double G_LNB = 65;
                double Vin_UB= (56. - adc)/502.;
                double A = -0.093, B= -2.46, Offset=-0.8;
                double Pin_PD = (Vin_UB - B - Offset)/A;    // new GHz ecard
                double Psys = pow(10, (Pin_PD- G_LNB)/10.); //in m Watt
                trace[ii] = Psys*1e-3;//*1e9;*/
                trace[ii] = adc; 
              } 
              theStation.SetSignal(trace);
              if (!theStation.HasSignal()) arrayData.AddStationData(theStation);   
              break; 
             } //End filling data trace
          } //End Station found
        } //End reading file   
        datfile.close();
      } //File closing  
      else  cout << "Unable to open file"<<endl; 
    } //End loop on channel
  } //End loop on antennas
}


void SimulateShower(string dataFilename,Event& theEvent, DetectorSetup& detectorsetup) { 
  
  cout.precision(9);
  TraceSimulator traceSimulator;
  BackgroundSimulator bgSimulator;
  ElectronicsSimulator elecSimulator;
  ofstream outfile("outfile");
  ifstream datfile;
  int countline = 0;
  double E, x, y, theta, phi, startimesec, baseline;
  long int gps; char c; int startime; long eventId;
  ostringstream filename;
  string line;
  filename << string(dataFilename);
  datfile.open(filename.str().c_str());
    size_t found = dataFilename.rfind("Event");
    size_t stop = dataFilename.find(".");
    int len = stop-(found+5);
    string ID = dataFilename.substr(found+5,len);
    //sscanf(ID.c_str(),"%lu",&eventId);
    
    istringstream buf(ID);
    //long int eventId ;
    buf >> eventId;
    (&theEvent)->SetEventId(eventId);
    
    //(&theEvent)->SetEventId(1053);
    cout << "check IDs: "<< eventId <<" "<< (&theEvent)->GetEventId()<<endl;
    outfile<<eventId<<" ";
   

  if (datfile.is_open()) {
    cout << " ********************" <<filename.str().c_str() <<" is open **************************"<<endl;
    while(getline(datfile, line)) {
      if (countline==0)  {
        datfile>>E>>x>>y>>theta>>phi>>gps>>baseline;
        sscanf(line.c_str(), "%lf %lf %lf %lf %lf %li %lf", &E,&x, &y, &theta, &phi, &gps, &baseline);
        cout <<"Energy (EeV) : "<<E<<" | X(m): "<<x<<" | Y(m): "<<y<<" | theta: "<<theta<<" | Phi: "<<phi<<" | GPS time: "<<gps<<" |baseline; "<<baseline<<endl;
        (&theEvent)->SetRunId(123);
        (&theEvent)->SetGPSSecond(gps);
        //Fill the associated shower
        Shower& theShower = (&theEvent)->GetArrayData().GetShower();
        theShower.SetEnergy(E*1.E18);
        TVector3 core;
        core.SetXYZ(x,y,0);
        theShower.SetCore(core);  
        //shower.SetCoreError();
        TVector3 axis(1,1,1);
        axis.SetTheta(theta*degree); 
        axis.SetPhi(phi*degree);
        theShower.SetAxis(axis);  
        //shower.SetAxisError(theta,phi);
        GHShowerParameterization ghParameters(100); //protons
        const double cosz = theShower.GetAxis().CosTheta();
        ghParameters.FillShower(theShower, theShower.GetEnergy(), 2.5*(g/cm2), cosz, false);
          // Time of impact
          const Atmosphere& atmo = Atmosphere::GetInstance();
          const double coreHeight = theShower.GetCore().Z() + detectorsetup.GetArray().GetHeight();
          const double hStart = fmin(atmo.Height(theShower.GetX1(), cosz), atmo.GetMaxHeight());
          const double tGround = (hStart - coreHeight) / kSpeedOfLight / cosz;
          theShower.SetCoreTime(0, (UInt_t) (tGround / nanosecond));
  
        //Copy it into simulation
        (&theEvent)->GetSimData().GetShower() = (&theEvent)->GetArrayData().GetShower();
        // Run the simulation
        addChannels(theEvent);
    	cout << "Running simulation... " << flush;
    	traceSimulator.Run(theEvent);
        bgSimulator.Run(theEvent);
    	elecSimulator.Run(theEvent);
        kgSimulator->Run(theEvent);
    	cout << "... done." << endl;
      }
    }
  } 
  datfile.close();
}


void fillEvent(string dataFilename,int iEvent, Event* theEvent) {

  // Create a detector setup based on the configuration
  const DetectorSetup& detectorSetup = Detector::GetInstance().GetSetup();
  DetectorSetup detectorsetup;

  const Detector& detector = Detector::GetInstance();
  const unsigned int nAntennas = detector.GetSetup().GetAntennas().size(); 
  if ( nAntennas <= 0 ) {
    cerr << " error: no antennas in detector " << endl;
    return;
  }

  // Read E, x, y, theta, phi, gpstime from first line of file; and simulate shower
  const unsigned int nTraceBins = 768;
  ifstream datfile;
  cout.precision(9);  
  int countline = 0; int bin,adc;
  double E,x,y,theta,phi; 
  long int gps; char c;
  ostringstream filename;
  int Nstations =0; string line; 
  filename << string(dataFilename);
  datfile.open(filename.str().c_str());
  if (datfile.is_open()) {
    int stat_id=-1; vector <int> stations; 
    cout << " ********************" <<filename.str().c_str() <<" is open **************************"<<endl;
    while(getline(datfile, line)) {
      if (countline==0) 
        {
          datfile>>E>>x>>y>>theta>>phi>>gps; 
          sscanf(line.c_str(), "%lf %lf %lf %lf %lf %li", &E,&x, &y, &theta, &phi,&gps);
          cout <<E<<" "<<x<<" "<<y<<" "<<theta<<" "<<phi<<" "<<gps<<endl;
     
        }
      else {
        getline(datfile,line);
        int countStat =0;
        size_t pos = line.find("Station:") ;
        if (pos!=std::string::npos) {
          string station = line.substr(pos+9); cout << "-------------> Station :"<<station<<endl;
          stat_id=atoi(station.c_str());
          stations.push_back(stat_id);
          AntennaData theAntenna;
          theAntenna.SetId(stat_id);
          for ( unsigned int j=0; j<1; j++ ) {
            ChannelData theChannel;
            theChannel.GetId();
            theChannel.MakeDataTrace(-2000*ns, 25*ns,768);
            vector<Double_t>& dataTrace = theChannel.GetDataTrace();
            for(int ii=0;ii<768; ii++) {
              datfile >> bin >> adc;
              dataTrace.push_back(adc);
            }
            theAntenna.AddChannelData(theChannel);
          }
          theEvent->AddAntennaData(theAntenna);
          countStat++;
        }
        int NStations=countStat;
      }
      countline++;
    }
    size_t pos = line.find("NStations") ; 
    int statnumber = atoi((line.substr(pos+9)).c_str());
    // cout <<"--------->" <<statnumber<< " touched antennas... "<<endl;
    // start with a fresh event
    *theEvent = Event();
    string ID = dataFilename.substr(38,12);istringstream buf(ID); cout << dataFilename <<" "<<ID<<endl;
    long int eventId ; 
    buf >> eventId;
    theEvent->SetEventId(eventId);
    theEvent->SetRunId(123);
    theEvent->SetGPSSecond(gps);
    Shower& theShower = theEvent->GetArrayData().GetShower();
    //Energy
    theShower.SetEnergy(E*1.E18); 
    //core
    TVector3 core; 
    core.SetXYZ(x,y,0);
    theShower.SetCore(core);  //shower.SetCoreError();
    //axis
    TVector3 axis;//(1); 
    axis.SetTheta(theta); axis.SetPhi(phi); 
    theShower.SetAxis(axis);  //shower.SetAxisError(theta,phi);
    //cout << "shower ok ..."<<endl;
    // Set these parameters into array data
    theEvent->GetArrayData().SetShower(theShower);
    // Create the simulation chain
    GHShowerParameterization ghParameters(100); //protons
    // copy the shower parameters to create simulation data
    theEvent->GetSimData().GetShower() = theEvent->GetArrayData().GetShower(); 
    cout << "theta "<<theta <<" Phi "<<phi<<endl;
    ghParameters.FillShower(theShower, theShower.GetEnergy(), 2.5*(g/cm2), theShower.GetCosZenith(), true, detectorsetup);
    TraceSimulator traceSimulator;
    vector<Double_t> trace;
    double startTime;
    BackgroundSimulator bgSimulator;
    ElectronicsSimulator elecSimulator;
    KGSimulator kgSimulator;
    // Run the simulation
    addChannels(*theEvent);
    cout << "Running simulation... " << flush;
    traceSimulator.Run(*theEvent);
    bgSimulator.Run(*theEvent);
    elecSimulator.Run(*theEvent);
    kgSimulator.Run(*theEvent);
    cout << "done." << endl;
 
    /*for ( unsigned int i=0; i<stations.size(); i++ ) {
      AntennaData theAntenna;
      theAntenna.SetId(stations[i]);
      for ( unsigned int j=0; j<1; j++ ) { //one station per channel
        ChannelData theChannel;
        theChannel.GetId();
        theChannel.MakeDataTrace(-2000*ns, 25*ns,768);
        vector<Double_t>& dataTrace = theChannel.GetDataTrace(); 
        //cout <<" Get data trace ok... "<<endl;
         for ( unsigned int k=0; k<nTraceBins; k++ ) {
          dataTrace.push_back(stat[i][k]);
        }
        theAntenna.AddChannelData(theChannel);
      }
      theEvent->AddAntennaData(theAntenna);
    }*/

  }
}






















/*int main(int argc, char** argv)
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " Config.xml <dat files> [...]" << endl;
    return 1;
  }

  // Parse the configuration file
  Configuration& config = Configuration::GetInstance();
  config.Init(string(argv[1]));

  // Create a detector setup based on the configuration
  Detector::GetInstance().Init();
  const DetectorSetup& detectorSetup = Detector::GetInstance().GetSetup();
  DetectorSetup detector;

  //output files
  EventFile eventFile("converted.root", EventFile::eWrite); // change this to monthly based naming 
  const int nFiles = argc -2;
  vector<string> datFilenames; 
  ifstream datfiles[nFiles];
  //loop on events(1 file = 1 event) 
  for ( int i = 0; i < nFiles; ++i )  datFilenames.push_back(string(argv[2+i]));
  for ( int i = 0; i < nFiles; ++i ) {
    int countline = 0;
    double E,x,y,theta,phi;
    long int gps; char c;
    ostringstream filename;
    filename << string(argv[2+i]);
    datfiles[i].open(filename.str().c_str());

    if (datfiles[i].is_open()) {
      cout << filename.str().c_str() <<" is open."<<endl;
      Event event;
      Shower& shower = event.GetSimData().GetShower();
      while(datfiles[i].get(c)) {
        if (countline==0) datfiles[i]>>E>>x>>y>>theta>>phi>>gps;
        countline++;
      }
      //shower
      shower.SetEnergy(E*1.e18);   //shower.SetEnergyError();
      //core
      TVector3 core; 
      core.SetXYZ(x,y,0);
      shower.SetCore(core);  //shower.SetCoreError();
      //axis
      TVector3 axis(1); axis.SetTheta(theta); axis.SetPhi(phi);
      shower.SetAxis(axis);  //shower.SetAxisError(theta,phi);
      // Set these parameters into array data
      event.GetArrayData().SetShower(shower);
      // Create the simulation chain
      GHShowerParameterization ghParameters(100); //protons
      // copy the shower parameters to create simulation data
      event.GetSimData().GetShower() = event.GetArrayData().GetShower();
      ghParameters.FillShower(shower, shower.GetEnergy(), 2.5*(g/cm2), shower.GetCosZenith(), true, detector);
      TraceSimulator traceSimulator;
      // Run the simulation
      cout << "Running simulation... " << flush;
      traceSimulator.Run(event);
      cout << "done." << endl;
      // Now fill the event characteristics
      eventFile.SetEventBuffer(&event);
      string ID = datFilenames[i].substr(38,12); istringstream buf(ID);
      long int eventId ; buf >> eventId;
      event.SetEventId(eventId);
      event.SetGPSSecond(gps);
      cout << "Event ID " << eventId<<endl;
      eventFile.WriteEvent();
    }
    else cout << "could not open file!"<<endl;
    datfiles[i].close();
  }
  cout << "Writing detector setup.." << endl;
  eventFile.WriteDetectorSetup(detectorSetup);



  return 0;
}*/
