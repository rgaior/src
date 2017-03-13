#ifndef  __EventFile_H
#define  __EventFile_H

class TTree;
class TFile;
class TBranch;
class Event;
class DetectorSetup;


#include <vector>
#include <string>

#include <Rtypes.h>
#include <TObject.h>


class EventFile {

public:

  enum Mode { eRead, eWrite };
  enum Status { eSuccess, eFailure };

  EventFile() {};

  /// constructor for read and write
  EventFile(const std::string& fname, const Mode mode = eRead);

  /// constructor to read a list of files
  EventFile(const std::vector<std::string>& fname);

  virtual ~EventFile();

  Status SetEventBuffer(Event* ev);
  void Close();
   // write event
  Status WriteEvent();
  /// read next event
  Status ReadNextEvent();
  /// read event given TTree entry number
  Status ReadEvent(const int iEvent);

  /// get total number of events
  int GetNumberOfEvents() const {return fNEvents;}
  /// get current event number
  int GetCurrentEventNumber() const { return fCurrentEvent;}
  /// get current file name
  std::string GetCurrentEventString() const;
  /// read detector setup
  Status ReadDetectorSetup(DetectorSetup& geom) const;
  /// write detector setup
  Status WriteDetectorSetup(const DetectorSetup& geom);
  /// change ROOT directory to current file
  void cd() const;

private:

  void AddFile(const std::string& fname, const Mode mode);

  TFile* fFile;
  TTree* fEventTree;
  Event* fEvent;

  Mode fMode;

  int fSaveSelf;
  int fNEvents;
  int fCurrentEvent;
  std::vector<std::string> fFileList;

  static const char* const kDataTreeName;
  static const char* const kDataBranchName;
  static const char* const kDetectorSetupName;

  ClassDef(EventFile, 1);
};

#endif
