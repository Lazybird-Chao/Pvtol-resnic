/**
 *  File: Grid.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    Grid.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the Grid class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: Grid.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_GRID_CC_
#define PVTOL_GRID_CC_


#include <PvtolBasics.h>
#include <Grid.h>
#include <Exception.h>
#include <vector>

namespace ipvtol
{

  // \brief getDimension
  // Return the value for the requested dimension of the grid.
  int Grid::getDimension(int dimension) const {
    if(static_cast<unsigned>(dimension) < m_dimensions.size()) {
      return m_dimensions[dimension];
    }
    else {
      throw Exception("getDimension: Invalid dimension.",
		      __FILE__, __LINE__);
    }
  }

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * Grid::serialize(int * buffer) const {
    // Store the size of the Grid, along with all the dimensions.
    *(buffer++) = m_dimensions.size();
    for(unsigned int i = 0; i < m_dimensions.size(); i++) {
      *(buffer++) = m_dimensions[i];
    }
    return buffer;
  }

  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available
  // storage location.
  int * Grid::deserialize(int * buffer) {
    m_dimensions.clear();
    int numDimensions = *(buffer++);
    for(int i = 0; i < numDimensions; i++) {
      m_dimensions.push_back(*(buffer++));
    }
    return buffer;
  }

} // namespace ipvtol


#endif // PVTOL_GRID_CC_ not defined
