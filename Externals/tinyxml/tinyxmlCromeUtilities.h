#ifndef __INCLUDE_TIUTL_H__
#define __INCLUDE_TIUTL_H__

#include <stdexcept>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

#include <tinyxml.h>

#include <TVector3.h>


using namespace std;


inline bool GetBoolValue(const TiXmlElement* branch) {
  if (branch != NULL) {
    const string valueChar = branch->FirstChild()->Value();

    unsigned int value;
    if ((sscanf(valueChar.c_str(), "%u", &value) == 1) &&
        ((value == 0) || (value == 1))) {
      return value == 1;
    } else {
      throw runtime_error(string("GetBoolValue(): Could not "
                                 "convert '") + valueChar +
                          string("' from branch ") + branch->Value() +
                          string("."));
    }
  } else {
    throw runtime_error(string("NULL bool branch"));
  }

  return 0;
}


inline double GetDoubleValue(const TiXmlElement* branch) {
  if (branch != NULL) {
    const char* valueChar = branch->FirstChild()->Value();

    double value;
    if (sscanf(valueChar, "%lf", &value) == 1) {
      return value;
    } else {
      throw runtime_error(string("error reading double ") +
                          string(" from branch "));
    }
  } else {
    throw runtime_error(string("NULL double branch"));
  }

  return 0;
}


inline int GetIntValue(const TiXmlElement* branch) {
  if (branch != NULL) {
    const char* valueChar = branch->FirstChild()->Value();

    int value;
    if (sscanf(valueChar, "%d", &value) == 1) {
      return value;
    } else {
      throw runtime_error(string("Error reading int from branch."));
    }
  } else {
    throw runtime_error(string("NULL integer branch"));
  }

  return 0;
}


inline int GetUnsignedValue(const TiXmlElement* branch) {
  if (branch != NULL) {
    const string valueChar = branch->FirstChild()->Value();

    unsigned int value;
    if (sscanf(valueChar.c_str(), "%u", &value) == 1) {
      return value;
    } else {
      throw runtime_error(string("GetUnsignedValue(): Could not "
                                 "convert '") + valueChar +
                          string("' from branch ") + branch->Value() +
                          string("."));
    }
  } else {
    throw runtime_error(string("NULL integer branch"));
  }

  return 0;
}


inline vector<double> GetDoubleTable(const TiXmlElement* branch)
{
  vector<double> value;
  if (branch != NULL) {
    const char* valueChar = branch->FirstChild()->Value();

    istringstream input(valueChar);
    while (input) {
      double tmp;
      input >> tmp;
      value.push_back(tmp);
    }
  } else {
    throw runtime_error(string("NULL double branch"));
  }

  const unsigned int size = value.size() - 1;
  value.resize(size);


  return value;
}



inline string GetStringValue(const TiXmlElement* branch) {
  if (branch != NULL) {
    const char* valueChar = branch->FirstChild()->Value();
    string value = valueChar;
    return value;
  } else {
    throw runtime_error(string("NULL string branch"));
  }

  return 0;
}


inline TVector3 GetPosition(const TiXmlElement* branch) {
  if (branch != NULL) {
    const TiXmlElement* xBranch = branch->FirstChildElement("x");
    const double x = GetDoubleValue(xBranch);
    const TiXmlElement* yBranch = branch->FirstChildElement("y");
    const double y = GetDoubleValue(yBranch);
    const TiXmlElement* zBranch = branch->FirstChildElement("z");
    const double z = GetDoubleValue(zBranch);
    return TVector3(x,y,z);
  }
  else
    throw runtime_error(string("NULL branch"));
}


inline string GetStringAttribute(const TiXmlElement* branch,
                                 const char* attributeName) {
  if (branch == NULL) {
    throw runtime_error(string("GetStringAttribute(): Got a null pointer "
                               "as branch (requested attribute: ") +
                        attributeName + string(")."));
  }

  string s;
  int result = branch->QueryStringAttribute(attributeName, &s);
  if (result != TIXML_SUCCESS) {
    throw runtime_error(string("GetStringAttribute(): Could not find "
                               "attribute '") + attributeName +
                        string("' from branch '") + branch->Value() +
                        string("'."));
  }

  return s;
}


inline int GetIntAttribute(const TiXmlElement* branch,
                           const char* attributeName) {
  if (branch == NULL) {
    throw runtime_error(string("GetIntAttribute(): Got a null pointer as "
                               "branch (requested attribute: ") +
                        attributeName + string(")."));
  }

  int i;
  int result = branch->QueryIntAttribute(attributeName, &i);
  if (result == TIXML_NO_ATTRIBUTE) {
    throw runtime_error(string("GetIntAttribute(): Could not find "
                               "attribute '") + attributeName +
                        string("' from branch '") + branch->Value() +
                        string("'."));
  } else if (result == TIXML_WRONG_TYPE) {
    throw runtime_error(string("GetIntAttribute(): Could not convert "
                               "attribute '") + attributeName +
                        string("' from branch '") + branch->Value() +
                        string("'."));
  }

  if (result != TIXML_SUCCESS) {
    throw runtime_error(string("GetIntAttribute(): Unhandled error "
                               "while requesting attribute '") +
                        attributeName + string("' from branch '") +
                        branch->Value() + string("'."));
  }

  return i;
}


inline unsigned int GetUnsignedAttribute(const TiXmlElement* branch,
                                         const char* attributeName) {
  if (branch == NULL) {
    throw runtime_error(string("GetUnsignedIntAttribute(): Got a null pointer "
                               "as branch (requested attribute: ") +
                        attributeName + string(")."));
  }

  unsigned int i;
  int result = branch->QueryUnsignedAttribute(attributeName, &i);
  if (result == TIXML_NO_ATTRIBUTE) {
    throw runtime_error(string("GetIntAttribute(): Could not find "
                               "attribute '") + attributeName +
                        string("' from branch '") + branch->Value() +
                        string("'."));
  } else if (result == TIXML_WRONG_TYPE) {
    throw runtime_error(string("GetIntAttribute(): Could not convert "
                               "attribute '") + attributeName +
                        string("' from branch '") + branch->Value() +
                        string("'."));
  }

  if (result != TIXML_SUCCESS) {
    throw runtime_error(string("GetIntAttribute(): Unhandled error "
                               "while requesting attribute '") +
                        attributeName + string("' from branch '") +
                        branch->Value() + string("'."));
  }

  return i;
}


inline TiXmlElement* GetFirstChild(TiXmlElement* branch,
                                   const char* childName)
{
  if (branch == NULL) {
    throw runtime_error(string("GetFirstChild(): Got a null pointer as "
                               "branch (requested child: ") + childName +
                        string(")."));
  }

  TiXmlElement* child = branch->FirstChildElement(childName);
  if (child == NULL) {
    throw runtime_error(string("GetFirstChild(): Could not find child "
                               "element '") + childName +
                        string("' in branch '") + branch->Value() +
                        string("'."));
  }

  return child;
}


inline const TiXmlElement* GetFirstChild(const TiXmlElement* branch,
                                         const char* childName)
{
  if (branch == NULL) {
    throw runtime_error(string("GetFirstChild(): Got a null pointer as "
                               "branch (requested child: ") + childName +
                        string(")."));
  }

  const TiXmlElement* child = branch->FirstChildElement(childName);
  if (child == NULL) {
    throw runtime_error(string("GetFirstChild(): Could not find child "
                               "element '") + childName +
                        string("' in branch '") + branch->Value() +
                        string("'."));
  }

  return child;
}


inline bool HasBranch(const TiXmlElement* topBranch,
                      string branchName) {
  const TiXmlElement* branch = topBranch->FirstChildElement(branchName);

  return branch != NULL;
}

#endif
