/**
 *  File: BasicTypes.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of inline methods and operators for types defined
 *           in BasicTypes.h
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: BasicTypes.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

namespace ipvtol
{
  // unnamed namespace for helper functions
  namespace
  {
    // Make 1D Domain from 1D Length
    static inline
    Domain<1> makeDomain(const Length<1>& len)
    {
      return Domain<1>(len[0]);
    }

    // Make 2D Domain from 2D Length
    static inline
    Domain<2> makeDomain(const Length<2>& len)
    {
      return Domain<2>(Domain<1>(len[0]), 
		       Domain<1>(len[1]));
    }

    // Make 3D Domain from 3D Length
    static inline
    Domain<3> makeDomain(const Length<3>& len)
    {
      return Domain<3>(Domain<1>(len[0]), 
		       Domain<1>(len[1]), 
		       Domain<1>(len[2]));
    }
  } // end unnamed namespace for helper functions


  // Operator to convert Length<N> to Domain<N>
  template<dimension_type N>
  Length<N>::operator Domain<N>() const
  {
    return makeDomain(*this);
  };

  // functions for building Length<N> objects from length_types
  inline
  Length<1> buildLengthArr(length_type len)
  {
    Length<1> arr;
    arr[0] = len;
    return arr;
  }
  inline
  Length<2> buildLengthArr(length_type len0, length_type len1)
  {
    Length<2> arr;
    arr[0] = len0;
    arr[1] = len1;
    return arr;
  }
  inline
  Length<3> buildLengthArr(length_type len0, 
                           length_type len1,
                           length_type len2)
  {
    Length<3> arr;
    arr[0] = len0;
    arr[1] = len1;
    arr[2] = len2;
    return arr;
  }
  template<dimension_type N>
  inline
  Length<N> buildLengthArr(const Domain<N>& dom)
  {
    Length<N> arr;
	for (int i = 0; i < N; i++)
	{
	  arr[i] = dom[i].length();
	}
	return arr;
  }

} // namespace ipvtol


