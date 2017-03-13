#ifndef _KGBits_h_
#define _KGBits_h_


// used in Trgg
enum EKGTriggerBits {
  eExternal = 0,
  eSevenOfSeven = 1,
  eFourOfSeven = 2
};

// used in Itrg and Iact
enum EKGComponentBits {
  eArray = 0,
  eEmagArray = 1,
  eMuonArray = 2,
  eCalorimeter = 3,
  eMWPCs = 4,
  eTriggerPlane = 5,
  eMuonTracker = 6,
  eTOPTrigger = 7,
  eFlashADCs = 8,
  eMCSim = 9,
  eStreamerTubes = 10,
  eGrandeArray = 11,
  ePiccoloArray = 12,
  eLOPES = 13
};

#endif
