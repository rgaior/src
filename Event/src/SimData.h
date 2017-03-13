#ifndef _SimData_h_
#define _SimData_h_

#include <TObject.h>
#include <Shower.h>

class SimData : public TObject { 

public:
  
  SimData();

  /// get shower 
  const Shower& GetShower() const { return fShower; }
  Shower& GetShower() { return fShower; }
  /// set shower 
  void SetShower(const Shower& shower) { fShower = shower; }

private:

  Shower fShower;

  ClassDef(SimData, 1);

};

#endif
