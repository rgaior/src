#ifndef __RandomGenerator__
#define __RandomGenerator__

#include "Singleton.h"

#include <cstddef>


namespace ROOT {
  namespace Math {
    class GSLRngMT;
  }
}


class RandomGenerator : public utl::Singleton<RandomGenerator>
{
 public:
  void Init(unsigned int showerSeed,
            unsigned int detectorSeed);

  ROOT::Math::GSLRngMT& GetShowerRandom() const;

  ROOT::Math::GSLRngMT& GetDetectorRandom() const;


 private:
  friend class utl::Singleton<RandomGenerator>;

  RandomGenerator()
    : fShowerRandom(NULL),
      fDetectorRandom(NULL)
  {
  }

  mutable ROOT::Math::GSLRngMT* fShowerRandom;

  mutable ROOT::Math::GSLRngMT* fDetectorRandom;
};

#endif
