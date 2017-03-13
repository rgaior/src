#include "TimedClass.h"
#include <TStopwatch.h>
#include <iostream>
#include <iomanip>

TimedClass::TimedClass(const std::string& name) : 
  fClassName(name) {
  fStopwatch = new TStopwatch();
  StopTimer();
} 


TimedClass::~TimedClass() {
  fStopwatch->Stop();
  const double realTime = fStopwatch->RealTime();
  const double cpuTime = fStopwatch->CpuTime();
  
  std::cout << std::setw(20) << fClassName << "|| real time: "
            << std::setprecision(1)
            << std::setw(7) << realTime << " sec, cpu time: " 
            << std::setw(7) << cpuTime << " sec." << std::endl;
  delete fStopwatch;

}

void TimedClass::StartTimer() {
  fStopwatch->Start(kFALSE);
}

void TimedClass::StopTimer() {
  fStopwatch->Stop();
}
