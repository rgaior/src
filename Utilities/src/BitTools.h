#ifndef _BitTools_h_
#define _BitTools_h_

#include <string>
#include <sstream>

namespace utl {

  bool BitIsSet(const unsigned int bitNumber, const unsigned int word) {
    const unsigned int bitMask = (1 << bitNumber);
    return (word & bitMask); 
  }
  
  std::string BitsAsString(const unsigned int word, 
                           const unsigned int firstBit,
                           const unsigned int lastBit) {
    
    std::ostringstream bitStream;
    const int i1 = firstBit;
    const int i2 = lastBit;
    
    for ( int i=i2; i>=i1; i-- )
      bitStream << BitIsSet(i, word);
    
    return bitStream.str();
    
  }
}

#endif 
