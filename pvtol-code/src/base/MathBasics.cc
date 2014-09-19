// File: MathBasics.cc
//
// Contents:
// Definitions of math utility functions.
//
// Author: Eddie Rutledge
//
// Revision History:
// 05/30/2000 - initial revision (contains only gcd and lcm).
/////////////////////////////////////////////////////////////////////////////
#ifndef Pvtol_MathBasics_cc
#define Pvtol_MathBasics_cc

#include <MathBasics.h>


namespace ipvtol
{

  // Greatest common divisor
  int gcd(int a, int b)
  {
    
    if(b == 0)
      {
	return a;
      }
    else
      {
	return gcd(b, a % b);
      }
  }

  // Least common multiple
  int lcm(int a, int b)
  {
    int tmp = gcd(a, b);

    if(a / tmp >= 1)
      {
	a = a / tmp;
      }

    if(b / tmp >= 1)
      {
	b = b / tmp;
      }

    if(tmp == 1)
      {
	return a * b;
      }
    else
      {
	return tmp * lcm(a, b);
      }
  }


}// end namespace


#endif // Pvl_MathUtil_cc

