/**
 *    File: MathBasics.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    February 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Declarations of scalar math utility functions.
 *
 *     See MathBasics.inl for the inline methods.
 *
 *  $Id: MathBasics.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_MATHBASICS_H
#define PVTOL_MATHBASICS_H

#include <cmath>
#include <stdlib.h>


namespace ipvtol
{

// USE The following c++ implementation of min and max instead of MIN and MAX
//  #include <algorithm>
//  using std::min;
//  using std::max;
//#define MIN(a,b)  ((a) < (b) ? (a) : (b))
//#define MAX(a,b)  ((a) > (b) ? (a) : (b))

  using std::fabs;

  // Greatest common divisor and least common multiple
  int gcd(int a, int b);
  int lcm(int a, int b);

  // Magnitude (absolute value) for real numbers.  Complex version is in
  // Complex.h
  template<class T>
     T abs(const T& a);
  template<class T>
     T mag(const T& a);

  /// conj scalar /maa
  template<class T>
     T conj(const T & a);

  /// sign scalar /maa
  template<class T>
     T sign(const T & a);

  /// magsq scalar /maa
  template<class T>
     T magsq(const T & a);

  /// round scalar /maa
  template<class T>
     T round(const T & a);

  //exponetial
  template<class T>
    T exponential(const T & a);

//                 I N L I N E     Methods
//----------------------------------------------------------------
  template<class T>
  inline
  T abs(const T& a)
  {
    return static_cast<T>(fabs(static_cast<double>(a)));
  }

  template<>
  inline
  int abs(const int& a)
  {
    return static_cast<int>(
    		std::abs(static_cast<double>(a))
			);

  }

  template<>
  inline
  long abs(const long& a)
  {
    return labs(a);
  }

  template<class T>
  inline
  T mag(const T& a)
  {
    return abs(a);
  }

  template<class T>
  inline
  T conj(const T & a)
  {
    return a;
  }


  template<class T>
  inline
  T sign(const T & a)
  {
    if (a>=0)
      return 1;
    else
      return -1;
  }

  template<class T>
  inline
  T magsq(const T & a)
  {
    return a*a;
  }

  template<class T>
  inline
  T round(const T & a)
  {
    T RetVal = a + .5;
    return std::floor(RetVal);
  }


  template<class T>
  inline
  T exponential(const T & a)
  {
    return std::exp(a);
  }

  inline
  double atan(int a)
  {
    return std::atan(static_cast<double>(a));
  }


}//end namespace


#endif // PVL_MATHBASICS_H not defined

