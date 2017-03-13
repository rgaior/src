#ifndef __Configuration__
#define __Configuration__

#include "Singleton.h"

#include <string>
#include <iostream>
#include <map>


class Configuration : public utl::Singleton<Configuration> {

public:

  void Init(const std::string& fileName, int verbosity=1);

  void DeInit(void);

  const std::string&
  GetConfigurationFileName(const std::string& configId) const;

  int GetVerbosity() const { return fVerbosity; }

private:

  friend class utl::Singleton<Configuration>;
  Configuration();

private:
  int fVerbosity;
  std::map<std::string, std::string> fFileNameMap;

};

#endif
