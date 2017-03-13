#ifndef _TimedClass_h_
#define _TimedClass_h_
#include <string>

class TStopwatch;

class TimedClass {

public:

  TimedClass(const std::string& name);
  ~TimedClass();

  void StartTimer();
  void StopTimer();

private:    
  
  TimedClass();
  std::string fClassName;
  TStopwatch* fStopwatch;
  

};

#endif
