/**
 *  File: Vector.inl
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of methods for the Vector class template.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: Vector.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

namespace ipvtol
{
  // constructors - forward to HierArray
  template<typename T, class Block, class Map>
  Vector<T, Block, Map>::Vector(length_type len, const Map& map) 
    : parent_type(buildLengthArr(len), map)
  {
  }

  template<typename T, class Block, class Map>
  Vector<T, Block, Map>::Vector(length_type len, 
								const T& val, 
								const Map& map)
    : parent_type(buildLengthArr(len), val, map)
  {
  }

  template<typename T, class Block, class Map>
  Vector<T, Block, Map>::Vector(length_type len, 
								Block& localIncoreStorage, 
								const Map& map)
    : parent_type(buildLengthArr(len), localIncoreStorage, map)
  {
  }

  template<typename T, class Block, class Map>
  Vector<T, Block, Map>::Vector(const Vector<T, Block, Map>& other)
    : parent_type(other)
  {
  }

  template<typename T, class Block, class Map>
  Vector<T, Block, Map>
  ::Vector(const HierArray<1, T, Block, Map>& other)
    : parent_type(other)
  {
  }
  
  template<typename T, class Block, class Map>
  Vector<T, Block, Map>&
  Vector<T, Block, Map>::operator=(const Vector<T, Block, Map>& rhs)
  {
    parent_type::operator=((const parent_type&)rhs);
	return *this;
  }

  template<typename T, class Block, class Map>
  template<typename T2, class Block2, class Map2>
  Vector<T, Block, Map>&
  Vector<T, Block, Map>::operator=(const Vector<T2, Block2, Map2>& rhs)
  {
    parent_type::operator=((const HierArray<1, T2, Block2, Map2>&)rhs);
	return *this;
  }

  template<typename T, class Block, class Map>
  template<typename T2>
  Vector<T, Block, Map>&
  Vector<T, Block, Map>::operator=(const T2& rhs)
  {
    parent_type::operator=(rhs);
	return *this;
  }
} // end namespace ipvtol
