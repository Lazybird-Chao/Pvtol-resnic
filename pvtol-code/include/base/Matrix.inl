/**
 *  File: Matrix.inl
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of methods for the Matrix class template.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: Matrix.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

namespace ipvtol
{
  // constructors - forward to HierArray
  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>::Matrix(length_type len1, length_type len2, const Map& map) 
    : parent_type(buildLengthArr(len1, len2), map)
  {
  }

  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>::Matrix(length_type len1,
								length_type len2,
								const T& val, 
								const Map& map)
    : parent_type(buildLengthArr(len1, len2), val, map)
  {
  }

  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>::Matrix(length_type len1, 
                                length_type len2,
								Block& localIncoreStorage, 
								const Map& map)
    : parent_type(buildLengthArr(len1, len2), localIncoreStorage, map)
  {
  }

  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>::Matrix(const Matrix<T, Block, Map>& other)
    : parent_type(other)
  {
  }

  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>
  ::Matrix(const HierArray<2, T, Block, Map>& other)
    : parent_type(other)
  {
  }
  
  template<typename T, class Block, class Map>
  Matrix<T, Block, Map>&
  Matrix<T, Block, Map>::operator=(const Matrix<T, Block, Map>& rhs)
  {
    parent_type::operator=((const parent_type&)rhs);
	return *this;
  }

  template<typename T, class Block, class Map>
  template<typename T2, class Block2, class Map2>
  Matrix<T, Block, Map>&
  Matrix<T, Block, Map>::operator=(const Matrix<T2, Block2, Map2>& rhs)
  {
    parent_type::operator=((const HierArray<1, T2, Block2, Map2>&)rhs);
	return *this;
  }

  template<typename T, class Block, class Map>
  template<typename T2>
  Matrix<T, Block, Map>&
  Matrix<T, Block, Map>::operator=(const T2& rhs)
  {
    parent_type::operator=(rhs);
	return *this;
  }
} // end namespace ipvtol
