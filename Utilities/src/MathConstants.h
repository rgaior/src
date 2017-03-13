#ifndef _utl_MathConstants_h_
#define _utl_MathConstants_h_

/**
  \file
  \brief Mathematical constants

  For unit conversions (including degree <-> radian), use the
  constants from the utl/AugerUnits.h header.

  For machine limits, use the numerical_limits template from the
  standard limits header.

  \author Lukas Nellen
  \version $Id: MathConstants.h 746 2012-01-31 13:27:22Z fwerner $
  \date 23 Sep 2003
  \ingroup units math
*/

#include <cmath>

namespace utl {

  const double kPi      = 3.1415926535897932384626;
  const double kPiOnTwo = 0.5*kPi;
  const double kTwoPi   = 2*kPi;
  const double kE       = 2.7182818284590452353602;
  const double kLn10    = 2.3025850929940456840180;
  const double kSqrt2   = 1.4142135623730950488017;
  const double kSqrt3   = 1.7320508075688772935274;
  const double kSqrt2Pi = 2.5066282746310005024158;

  // Euler-Mascheroni Constant
  const double kEulerGamma = 0.5772156649015328606065;

} // utl


#endif

// Configure (x)emacs for this file ...
// Local Variables:
// mode: c++
// compile-command: "make -C .. -k"
// End:
