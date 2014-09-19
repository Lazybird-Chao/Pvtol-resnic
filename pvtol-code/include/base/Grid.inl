/**
 *  File: Grid.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Inline methods of the Grid class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: Grid.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_GRID_INL_
#define PVTOL_GRID_INL_


#include <vector>

namespace ipvtol
{

  // \brief Constructor
  // Construct a 1d Grid, given the size of the dimension.
  inline
  Grid::Grid(int dim0Size) {
    m_dimensions.push_back(dim0Size);
  }
  
  // \brief Constructor
  // Construct a 2d Grid, given the size of the dimensions.
  inline
  Grid::Grid(int dim0Size, int dim1Size) {
    m_dimensions.push_back(dim0Size);
    m_dimensions.push_back(dim1Size);
  }
  
  // \brief Constructor
  // Construct a 3d Grid, given the size of the dimensions.
  inline
  Grid::Grid(int dim0Size, int dim1Size, int dim2Size) {
    m_dimensions.push_back(dim0Size);
    m_dimensions.push_back(dim1Size);
    m_dimensions.push_back(dim2Size);
  }
  
  // \brief Copy Constructor
  // Construct a Grid, given another grid.
  inline
  Grid::Grid(const Grid & other) {
    m_dimensions = other.m_dimensions;
  }
  
  // \brief getNumGridDimensions
  // Return the number of dimensions in the grid.
  inline
  int Grid::getNumDimensions() const {
    return m_dimensions.size();
  }
  
  // \brief getDimension
  // Return the value for the requested dimension of the grid.
  inline
  int Grid::getDimension(int dimension) const {
    return m_dimensions[dimension];
  }
  
  // \brief Assignment operator
  inline
  const Grid & Grid::operator=(const Grid & other) {
    m_dimensions = other.m_dimensions;
    return (*this);
  }
  
  // \brief Equality operator
  inline
  bool Grid::operator==(const Grid & other) const {
    return (m_dimensions == other.m_dimensions);
  }
  
  // \brief Inequality operator
  inline
  bool Grid::operator!=(const Grid & other) const {
    return (m_dimensions != other.m_dimensions);
  }
  
  // \brief Destructor
  // Destruct the object.
  inline
  Grid::~Grid() {
  }
  
} //namespace ipvtol


#endif // PVTOL_GRID_INL_ not defined

