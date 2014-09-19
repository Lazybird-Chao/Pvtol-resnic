/**
 *  File: Map.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline implementations of the Map abstract base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: Map.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_MAP_CC_
#define PVTOL_MAP_CC_


#include <Map.h>
#include <Exception.h>
#include <vector>
#include <TaskManager.h>
#include <TaskBase.h>

namespace ipvtol
{

  // \brief isLocal
  // Return a boolean indicating if the calling thread/process taskRank id is 
  // in the map's RankList.
  bool Map::isLocal() const {
    return
      m_rankList.hasRank(TaskManager::getCurrentTask().getGlobalThreadRank());
  }

  /// \brief getSerializedSize
  /// Return size of the serialized object.
  int Map::getSerializedSize() const {
    // Add 1 buffer space for the map type, plus the space required by the 
    // rankList and grid.
    int size = 1 + m_rankList.getSerializedSize() 
      + m_grid.getSerializedSize();
    
    // Add the space for the DistDescription object + 1 for the type.
    if(m_distDescription != NULL) {
      size += m_distDescription->getSerializedSize() + 1;
    }
    return size;
  }
  
  /// \brief serialize
  /// Store all object information within the supplied integer
  /// buffer.  Return the buffer, incremented to the next available storage
  /// location.
  int * Map::serialize(int * buffer) const {
    *(buffer++) = static_cast<int>(m_mapType);
    buffer = m_rankList.serialize(buffer);
    buffer = m_grid.serialize(buffer);

#if PVTOL_DEVELOP
    if(m_distDescription == NULL) {
      cerr << "Warning: Map::serialize.  Attempting to serialize a map "
	   << "without a defined DistDescription." << endl;
    }
#endif
    
    *(buffer++) 
      = static_cast<int>(m_distDescription->getDistDescriptionType());
    buffer = m_distDescription->serialize(buffer);
    
    return buffer;
  }
  
  //// \brief deserialize
  /// Initialize the object with the information supplied within
  /// the buffer.  Return the buffer, incremented to the next available 
  /// storage location.
  int * Map::deserialize(int * buffer) {
    
    m_mapType = static_cast<Map::MapType>(*buffer++);
    buffer = m_rankList.deserialize(buffer);
    buffer = m_grid.deserialize(buffer);

    switch(static_cast<DistDescription::DistDescriptionType>(*buffer++)) {
    case DistDescription::DATA_DIST_DESCRIPTION:
      m_distDescription = new DataDistDescription;
      break;
    case DistDescription::TASK_DIST_DESCRIPTION:
      m_distDescription = new TaskDistDescription;
      break;
    case DistDescription::FUNCTOR_DIST_DESCRIPTION:
      throw Exception("deserialize: Unimplemented DistDescriptionType."
			__FILE__, __LINE__);
      break;
    default:
      throw Exception("deserialize: Invalid DistDescriptionType."
			__FILE__, __LINE__);
    }
    
    buffer = m_distDescription->deserialize(buffer);   

    return buffer;
  }
  
  // \brief Assignment operator
  const Map & Map::operator=(const Map & other) {
    
    if(*this != other) {
      if( m_mapType != other.m_mapType ) {
	throw Exception("Map assignment operator: Map types do not match.",
			__FILE__, __LINE__);
      }
      freeMapMemory();
      m_rankList = other.m_rankList;
      m_grid = other.m_grid;
      m_distDescription = other.m_distDescription->clone();
    }
    
    return *this;	
  }

  // \brief Equality operator
  bool Map::operator==(const Map & other) const {
    if(m_mapType == other.m_mapType && m_grid == other.m_grid 
       && m_rankList == other.m_rankList 
       && *m_distDescription == *(other.m_distDescription)) {
      return true;
    }
    else {
      return false;
    }
  }

  // \brief Inequality operator
  bool Map::operator!=(const Map & other) const {
    return !(*this == other);
  }

  // \brief freeMemory
  // Free all allocated Map memory.
  void Map::freeMapMemory() {
    if(m_distDescription != NULL) {
      delete m_distDescription;
      m_distDescription = NULL;  
    }  
  }

} // namespace ipvtol


#endif // PVTOL_MAP_CC_ not defined
