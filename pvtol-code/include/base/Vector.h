/**
 *  File: Vector.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the Vector class template.  Vector is the PVTOL
 *           mathematical vector class.  PVTOL vectors can have hierarchical
 *           distributions.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: Vector.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_VECTOR_H_
#define PVTOL_VECTOR_H_

#include <HierArray.h>

namespace ipvtol
{
  /// \brief class Vector<T,Block,LocalMap>
  /// PVTOL mathematical vector class template.
  template<typename T, class Block=Dense<1, T>, class Map=LocalMap>
  class Vector : public HierArray<1, T, Block, Map>
  {
  public:
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC TYPES
    /////////////////////////////////////////////////////////////////////////
    /// expose the needed HierArray typedefs
    typedef HierArray<1, T, Block, Map> parent_type;
    typedef typename parent_type::value_type value_type;
    typedef typename parent_type::reference_type reference_type;
    typedef typename parent_type::const_reference_type const_reference_type;
    typedef typename parent_type::map_type map_type;
    typedef Vector<value_type, 
		   typename parent_type::subarray_block_type, 
		   map_type> subview_type;
    typedef Vector<value_type, 
		   typename parent_type::const_subarray_block_type, 
		   map_type> const_subview_type;
    
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    /////////////////////////////////////////////////////////////////////////

    /// \brief Constructor
    /// Construct a vector of the given size
    explicit Vector(length_type len, const Map& map = Map()); 

    /// \brief Constructor
    /// Construct a vector of the given size with all elements set to the 
    /// given element value
    Vector(length_type len, 
		   const T& val, 
	       const Map& map = Map()); 

    /// \brief Constructor
    /// Construct a vector of the given size using the given Block for 
    /// local incore storage
    Vector(length_type len,
	       Block& localIncoreStorage, 
	       const Map& map = Map()); 

    /// \brief Copy Constructor
    /// Construct a shallow copy of the given HierArray.
    Vector(const Vector<T, Block, Map>& other);

    /// \brief Constructor
    /// Convert from 1D HierArray to Vector
    Vector(const HierArray<1, T, Block, Map>& other);

	/// \brief Assignment
	Vector<T, Block, Map>& operator=(const Vector<T, Block, Map>& rhs);
	template<typename T2, class Block2, class Map2>
	Vector<T, Block, Map>& operator=(const Vector<T2, Block2, Map2>& rhs);
	template<typename T2>
	Vector<T, Block, Map>& operator=(const T2& rhs);

  }; // class Vector
  
} // namespace ipvtol

#include <Vector.inl>

#endif // PVTOL_VECTOR_H_ not defined

