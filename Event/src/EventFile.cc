#include "EventFile.h"
#include "Event.h"
#include <DetectorSetup.h>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TDirectory.h>

#include <cmath>
#include <iostream>


using namespace std;

ClassImp(EventFile)

const char* const EventFile::kDataTreeName = "MWData";
const char* const EventFile::kDataBranchName = "event";
const char* const EventFile::kDetectorSetupName = "DetectorSetup";


EventFile::EventFile(const string& fname, const Mode mode):
  fFile(0),
  fEventTree(0),
  fEvent(0),
  fMode(mode),
  fSaveSelf(-1),
  fNEvents(0),
  fCurrentEvent(-1)
{
  AddFile(fname, fMode);
}

/// constructor to read a list of files
EventFile::EventFile(const vector<string>& fname) :
  fFile(0),
  fEventTree(0),
  fEvent(0),
  fMode(eRead),
  fSaveSelf(-1),
  fNEvents(0),
  fCurrentEvent(-1)
{
  for (vector<string>::const_iterator iFile = fname.begin();
       iFile != fname.end(); ++iFile) {
    cout << "  input file " << *iFile << endl;
    AddFile(*iFile, eRead);
  }
}

EventFile::~EventFile()
{
  Close();

  if (fMode == eRead)
    delete fEventTree;

}


void
EventFile::Close()
{
  // nothing to do in read mode
  if (fMode == eRead || fFile == NULL)
    return;

  fFile->cd();
  if (fEventTree)
    fEventTree->Write();

  fFile->Close();
  delete fFile;
  fFile = NULL;
}



void
EventFile::AddFile(const string& fname, const Mode mode)
{
  TDirectory* const save = gDirectory;

  if (mode != eRead && fFileList.size() > 0) {
    throw runtime_error("EventFile::AddFile(): Only one file can be opened "
                        "for writing.");
  }

  // prepare access to files by using TChains
  if (mode == eRead) {
    if (fEventTree == NULL) {
      fEventTree = new TChain(kDataTreeName);
    }

    const int nEventChain = ((TChain*)fEventTree)->Add(fname.c_str(), 0);

    if (nEventChain == 0) {
      throw runtime_error(string("EventFile::AddFile(): Failed to open ") +
                          fname + string(" for reading."));
    }
  } else {
    // open file in write mode
    fFile = new TFile(fname.c_str(), "recreate");

    if (fFile == NULL || fFile->IsZombie()) {
      save->cd();
      fFile = NULL;

      throw runtime_error(string("EventFile::AddFile(): Failed to open ") +
                          fname + string(" for writing."));
    }

    fNEvents = 0;
  }

  fFileList.push_back(fname);
  save->cd();
}



EventFile::Status
EventFile::SetEventBuffer(Event* ev)
{
  if (ev == NULL) {
    // TODO We should probably throw something here
    //      -> Refactor SetEventBuffer to require a reference
    return EventFile::eFailure;
  }

  TDirectory* save = gDirectory;

  bool makeBranch = fEvent == NULL;

  fEvent = ev;

  if (fMode == eWrite && fFile) {

    fFile->cd();

    if (makeBranch) {
      fEventTree = new TTree(kDataTreeName, "MicroWave Data Tree");
      fEventTree->Branch(kDataBranchName, "Event", fEvent, 16000, 99);
    } else {
      fEventTree->SetBranchAddress(kDataBranchName, &fEvent);
    }

  } else if (fEventTree) {

    fEventTree->SetBranchAddress(kDataBranchName, &fEvent);
    fNEvents = fEventTree->GetEntries();

  }

  save->cd();
  return EventFile::eSuccess;
}


EventFile::Status
EventFile::WriteEvent()
{
  if (fMode != eWrite) {
    cerr << "Trying to write to read only file!" << endl;
    return eFailure;
  }
  if (!fEventTree)
    return eFailure;

  fEventTree->Fill();
  ++fCurrentEvent;
  ++fNEvents;

  if (fSaveSelf >= 0 && !(fNEvents % fSaveSelf)) {
    fEventTree->AutoSave("SaveSelf");
  }

  return eSuccess;
}


EventFile::Status
EventFile::ReadNextEvent()
{
  if (fMode != eRead) {
    cerr << "Trying to read from write only file!" << endl;
    return eFailure;
  }

  if (fCurrentEvent < fNEvents - 1) {
    ++fCurrentEvent;
    fEventTree->GetEntry(fCurrentEvent);
    return eSuccess;
  }
  return eFailure;
}


EventFile::Status
EventFile::ReadEvent(const int iEvent)
{
  if (fMode != eRead) {
    cerr << "Trying to read from write only file!" << endl;
    return eFailure;
  }

  if (iEvent < fNEvents && iEvent > -1) {
    fEventTree->GetEntry(iEvent);
    fCurrentEvent = iEvent;
    return eSuccess;
  }

  return eFailure;
}

EventFile::Status
EventFile::ReadDetectorSetup(DetectorSetup& theGeo)
  const
{
  if (&theGeo == 0)
    return eFailure;

  if (!fEventTree)
    return eFailure;

  TDirectory* const save = gDirectory;

  TObjArray* const fileElements = ((TChain*)fEventTree)->GetListOfFiles();
  TIter next(fileElements);
  TChainElement* chEl = 0;
  while ((chEl = (TChainElement*)next())) {
    TFile file(chEl->GetTitle());
    file.cd();
    DetectorSetup* const thisGeo = (DetectorSetup*)file.Get(kDetectorSetupName);
    theGeo = *thisGeo;
    delete thisGeo;
  }

  save->cd();
  return eSuccess;
}


EventFile::Status
EventFile::WriteDetectorSetup(const DetectorSetup& geom)
{
  if (!fFile)
    return eFailure;

  TDirectory* const save = gDirectory;
  fFile->cd();
  geom.Write(kDetectorSetupName, TFile::kOverwrite);
  save->cd();
  return eSuccess;
}


string EventFile::GetCurrentEventString() const {
  if ( fEventTree ) {
    TFile* file = ((TChain*) fEventTree)->GetFile();
    if ( file ) {
      return string(" browsing ") + string(file->GetName());

    }
  }
  return string("");

}

void EventFile::cd() const {
  if ( fFile )
    fFile->cd();
}
