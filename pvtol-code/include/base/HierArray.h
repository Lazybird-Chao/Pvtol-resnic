/**
 *  File: HierArray.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the HierArray class template.
 *           The HierArray class is the PVTOL hierarchical, distributed array
 *           class.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: HierArray.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_HIER_ARRAY_H_
#define PVTOL_HIER_ARRAY_H_

#include <BasicTypes.h>
#include <LocalMap.h>
#include <Dense.h>

namespace ipvtol
{
  /// \brief class HierArray
  template<dimension_type N=1, 
	   typename T=float, 
	   class Block=Dense<N, T>, 
	   class Map=LocalMap>
  class HierArray; // No generic definition

  /// HierArray equality operator.
  template<dimension_type N, typename T1, class Block1, class Map1,
	   typename T2, class Block2, class Map2>
  bool operator==(const HierArray<N, T1, Block1, Map1>& lhs,
		  const HierArray<N, T2, Block2, Map2>& rhs);

} // namespace ipvtol

// include HierArray specializations
#include <HierArray_LocalMap.h>  // specialization for LocalMap map parameter

// include generic HierArray operator definitions
#include <HierArray.inl>

#endif // PVTOL_HIER_ARRAY_H_ not defined

