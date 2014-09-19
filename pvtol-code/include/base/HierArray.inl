/**
 *  File: HierArray.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of generic operators for the HierArray class template.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: HierArray.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

namespace ipvtol
{
  // 1D array equality operator
  template<typename T1, class Block1, class Map1,
	   typename T2, class Block2, class Map2>
  bool operator==(const HierArray<1, T1, Block1, Map1>& lhs,
		  const HierArray<1, T2, Block2, Map2>& rhs)
  {
    // Compare sizes
    if (lhs.size() != rhs.size())
      {
	return false;
      }

    // Compare elements
    for (unsigned int i = 0; i < lhs.size(); ++i)
      {
	if (lhs(Index<1>(i)) != rhs(Index<1>(i)))
	  {
	    return false;
	  }
      }
    return true;
  }

  // 2D array equality operator
  template<typename T1, class Block1, class Map1,
	   typename T2, class Block2, class Map2>
  bool operator==(const HierArray<2, T1, Block1, Map1>& lhs,
		  const HierArray<2, T2, Block2, Map2>& rhs)
  {
    // Compare sizes
    if (lhs.size(0) != rhs.size(0) || lhs.size(1) != rhs.size(1))
      {
	return false;
      }

    // Compare elements
    for (unsigned int i = 0; i < lhs.size(0); ++i)
      {
	for (unsigned int j = 0; j < lhs.size(1); ++j)
	  {
	    if (lhs(Index<2>(i,j)) != rhs(Index<2>(i,j)))
	      {
		return false;
	      }
	  }
      }
    return true;
  }
  
  // 3D array equality operator
  template<typename T1, class Block1, class Map1,
	   class T2, class Block2, class Map2>
  bool operator==(const HierArray<3, T1, Block1, Map1>& lhs,
		  const HierArray<3, T2, Block2, Map2>& rhs)
  {
    // Compare sizes
    if (lhs.size(0) != rhs.size(0) || lhs.size(1) != rhs.size(1) ||
	lhs.size(2) != rhs.size(2))
      {
	return false;
      }

    // Compare elements
    for (unsigned int i = 0; i < lhs.size(0); ++i)
      {
	for (unsigned int j = 0; j < lhs.size(1); ++j)
	  {
	    for (unsigned int k = 0; k < lhs.size(2); ++k)
	      {
		if (lhs(Index<3>(i,j,k)) != rhs(Index<3>(i,j,k)))
		  {
		    return false;
		  }
	      }
	  }
      }
    return true;
  }

  // scalar/array equality operator
  template<dimension_type N, 
	   typename T1, 
	   class Block, 
	   class Map>
  bool operator==(const HierArray<N, T1, Block, Map>& lhs, const T1& rhs)
  {
    Length<N> len;
    for (unsigned int i = 0; i < N; ++i)
      {
	len[i] = lhs.size(i);
      }
    return (lhs == HierArray<N, T1, Block, Map>(len, rhs, lhs.map()));
  }
  
  // generic inequality operator - just do oposite of equality operator
  template<dimension_type N, typename T1, typename Block1, typename Map1,
	   typename T2>
  bool operator!=(const HierArray<N, T1, Block1, Map1>& lhs, const T2& rhs)
  {
    return !(lhs == rhs);
  }

} // namespace ipvtol

