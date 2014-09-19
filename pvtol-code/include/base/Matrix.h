/**
 *  File: Matrix.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the Matrix class template.  Matrix is the PVTOL
 *           mathematical matrix class.  PVTOL matrices can have hierarchical
 *           distributions.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: Matrix.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_MATRIX_H_
#define PVTOL_MATRIX_H_

#include <HierArray.h>

namespace ipvtol
{
  /// \brief class Matrix<T,Block,LocalMap>
  /// PVTOL mathematical Matrix class template.
  template<typename T, class Block=Dense<2, T>, class Map=LocalMap>
  class Matrix : public HierArray<2, T, Block, Map>
  {
  public:
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC TYPES
    /////////////////////////////////////////////////////////////////////////
    /// expose the needed HierArray typedefs
    typedef HierArray<2, T, Block, Map> parent_type;
    typedef typename parent_type::value_type value_type;
    typedef typename parent_type::reference_type reference_type;
    typedef typename parent_type::const_reference_type const_reference_type;
    typedef typename parent_type::map_type map_type;
    typedef Matrix<value_type, 
		   typename parent_type::subarray_block_type, 
		   map_type> subview_type;
    typedef Matrix<value_type, 
		   typename parent_type::const_subarray_block_type, 
		   map_type> const_subview_type;
    
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    /////////////////////////////////////////////////////////////////////////

    /// \brief Constructor
    /// Construct a matrix of the given size
    Matrix(length_type len1, length_type len2, const Map& map = Map()); 

    /// \brief Constructor
    /// Construct a matrix of the given size with all elements set to the 
    /// given element value
    Matrix(length_type len1,
		   length_type len2,
		   const T& val, 
	       const Map& map = Map()); 

    /// \brief Constructor
    /// Construct a matrix of the given size using the given Block for 
    /// local incore storage
    Matrix(length_type len1,
		   length_type len2,
	       Block& localIncoreStorage, 
	       const Map& map = Map()); 

    /// \brief Copy Constructor
    /// Construct a shallow copy of the given HierArray.
    Matrix(const Matrix<T, Block, Map>& other);

    /// \brief Constructor
    /// Convert from 1D HierArray to matrix
    Matrix(const HierArray<2, T, Block, Map>& other);

	/// \brief Assignment
	Matrix<T, Block, Map>& operator=(const Matrix<T, Block, Map>& rhs);
	template<typename T2, class Block2, class Map2>
	Matrix<T, Block, Map>& operator=(const Matrix<T2, Block2, Map2>& rhs);
	template<typename T2>
	Matrix<T, Block, Map>& operator=(const T2& rhs);

  }; // class Matrix
  
} // namespace ipvtol


#include <Matrix.inl>

#endif // PVTOL_MATRIX_H_ not defined

