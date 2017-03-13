#include "Configuration.h"
#include "RandomGenerator.h"

#include <stdexcept>
#include <tinyxml.h>
#include <tinyxmlCromeUtilities.h>
#include <time.h>

using namespace std;

Configuration::Configuration() :
  fVerbosity(0) 
{

}



void Configuration::Init(const string& fileName, int verbosity) 
{

  fVerbosity = verbosity;

  if ( ! fFileNameMap.empty() ) {
    cerr << " Configuration::Init() - Error: fileNameMap already "
         << " initialized --> IGNORE! " << endl;
    return;
  }

  if ( verbosity > 0 ) 
    cout << "\n  ==[Configuration::Init()]==>\n"
         << "   configuration file: " << fileName << endl;
 
  TiXmlDocument doc(fileName);
  if ( doc.LoadFile() ) {
    const TiXmlElement* topBranch = doc.FirstChildElement("Configuration");
    if (!topBranch ) {
      throw std::runtime_error(string("Configuration::Init(): ")+
			       string("invalid configuration file")+
			       fileName);
    }

    const TiXmlElement* configBranch = 
      topBranch->FirstChildElement("configLink");

    while( configBranch ) {
      const char* id = configBranch->Attribute("id");
      const char* path = configBranch->Attribute("path");
      if ( id && path ) { 
        fFileNameMap[string(id)] = string(path);
        if ( verbosity > 0 ) 
          cout << "   configId=\"" << id << "\"\n   with file " 
               << path << endl; 
      }
      configBranch = configBranch->NextSiblingElement("configLink");
    }


    //const int showerSeed = (int)(time(NULL)); //for sensitivity studies
    const int showerSeed =  GetIntValue(topBranch->FirstChildElement("showerSeed"));
    //const int detectorSeed = (int)(time(NULL))+10;
    const int detectorSeed =  GetIntValue(topBranch->FirstChildElement("detectorSeed"));
    
    RandomGenerator::GetInstance().Init(showerSeed, detectorSeed);


  }
  else {
    throw std::runtime_error(string("Configuration::Init(): ")+
			     string("error opening/reading ")+
			     fileName);
  }

}


void Configuration::DeInit()
{
  fVerbosity = 0;
  fFileNameMap.clear();
}


const std::string& 
Configuration::GetConfigurationFileName(const std::string& configId) 
  const {

  std::map<std::string, std::string>::const_iterator iter = 
    fFileNameMap.find(configId);


  if ( iter == fFileNameMap.end() )
    throw std::runtime_error(string("Configuration::"
                                    "GetConfigurationFileName():")+
                             string(" Error - request for non-existing"
                                    " configId ") + configId);
  else
    return iter->second;
       
}
