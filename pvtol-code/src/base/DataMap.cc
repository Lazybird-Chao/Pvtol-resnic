/**
 *  File: DataMap.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline implementations of the base class DataMap.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataMap.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATAMAP_CC_
#define PVTOL_DATAMAP_CC_

#include <PvtolBasics.h>
#include <DataMap.h>
#include <TemporalMapBase.h>
#include <Map.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <LocalMap.h>
#include <RuntimeMap.h>
#include <AutoMap.h>
#include <TemporalMap.h>
#include <AutoTemporalMap.h>

#include <vector>

namespace ipvtol
{

  // \brief Copy contructor
  // Construct a DataMap, given another DataMap.
  DataMap::DataMap(const DataMap & other)
    : Map(other),
      m_numChildDataMaps(other.m_numChildDataMaps),
      m_hasTemporalMap(other.m_hasTemporalMap),
      m_dataMapType(other.m_dataMapType)
  {
    if(m_hasTemporalMap) {
      m_temporalMapPtr = other.m_temporalMapPtr->clone();
    }
    else {
      m_temporalMapPtr = NULL;
    }
    copyChildDataMaps(other.m_childDataMaps);
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  int DataMap::getSerializedSize() const {

    // Get the max size from the parent object.
    int size = Map::getSerializedSize();

    // Allocate 1 buffer space for the number of child maps, 1 for the boolean
    // for potential TemporalMap, and 1 for the map data type.
    size += 3;

    // For each child map, accumulate the sizes.  Add one for the map type.
    for(int i = 0; i < m_numChildDataMaps; i++) {
      size += m_childDataMaps[i]->getSerializedSize() + 1;
    }

    // Add the size of the temporal map(s).  Add one for the map type.
    if(m_hasTemporalMap) {
      size += m_temporalMapPtr->getSerializedSize() + 1;
    }

    return size;
  }

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * DataMap::serialize(int * buffer) const {

    buffer = Map::serialize(buffer);

    *(buffer++) = static_cast<int>(m_dataMapType);
    *(buffer++) = m_numChildDataMaps;

    for(int i = 0; i < m_numChildDataMaps; i++) {
      *(buffer++) = static_cast<int>(m_childDataMaps[i]->getDataMapType());
      buffer = m_childDataMaps[i]->serialize(buffer);
    }

    *(buffer++) = static_cast<int>(m_hasTemporalMap);

    if(m_hasTemporalMap) {
      *(buffer++)
	= static_cast<int>(m_temporalMapPtr->getTemporalMapBaseType());
      buffer = m_temporalMapPtr->serialize(buffer);
    }

    return buffer;
  }

  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available
  // storage location.
  int * DataMap::deserialize(int * buffer) {

    buffer = Map::deserialize(buffer);

    m_dataMapType = static_cast<DataMap::DataMapType>(*buffer++);
    m_numChildDataMaps = *buffer++;

    for(int i = 0; i < m_numChildDataMaps; i++) {
      switch(static_cast<DataMap::DataMapType>(*buffer++)) {
      case DataMap::LOCAL_MAP:
	m_childDataMaps.push_back(new LocalMap);
	break;
      case DataMap::RUNTIME_MAP:
	m_childDataMaps.push_back(new RuntimeMap);
	break;
      case DataMap::AUTO_MAP:
	m_childDataMaps.push_back(new AutoMap);
	break;
      default:
	throw Exception("deserialize: Invalid DataMap::DataMapTyep."
			__FILE__, __LINE__);
      }
      buffer = m_childDataMaps[i]->deserialize(buffer);
    }

    m_hasTemporalMap = static_cast<bool>(*buffer++);

    if(m_hasTemporalMap) {
      switch(static_cast<TemporalMapBase::TemporalMapBaseType>(*buffer++)) {
      case TemporalMapBase::AUTO_MAP:
	m_temporalMapPtr = new AutoTemporalMap;
	break;
      case TemporalMapBase::RUNTIME_MAP:
	m_temporalMapPtr = new TemporalMap;
	break;
      default:
	throw Exception("deserialize: Invalid TemporalMapBaseType.",
			__FILE__, __LINE__);
      }
      buffer = m_temporalMapPtr->deserialize(buffer);
    }

    return buffer;
  }

  // \brief Assignment operator
  const DataMap & DataMap::operator=(const DataMap & other) {
    if(*this != other) {
      if(m_dataMapType == other.m_dataMapType) {
	throw Exception("DataMap assignment operator: Map types do not match.",
			__FILE__, __LINE__);
      }
      // Clean any existing memory prior to assignment.
      freeDataMapMemory();

      // Assign from the input Map.
      Map::operator=(other);

      // Assign from the input DataMap.
      m_temporalMapPtr = other.m_temporalMapPtr->clone();
      m_hasTemporalMap = other.m_hasTemporalMap;
      m_numChildDataMaps = other.m_numChildDataMaps;
      copyChildDataMaps(other.m_childDataMaps);
    }

    return *this;
  }

  // \brief Equality operator
  bool DataMap::operator==(const DataMap & other) const {
    // Verify simple parameters.
    if(m_dataMapType == other.m_dataMapType
       && Map::operator==(other)
       && m_hasTemporalMap == other.m_hasTemporalMap
       && m_numChildDataMaps == other.m_numChildDataMaps) {

      // Verify any temporal maps are equal.
      if(m_hasTemporalMap) {
	if(*(m_temporalMapPtr) != *(other.m_temporalMapPtr)) {
	  return false;
	}
      }

      // Verify that any child data maps are equal.
      for(int i = 0; i < m_numChildDataMaps; i++) {
	if(*m_childDataMaps[i] != *other.m_childDataMaps[i]) {
	  return false;
	}
      }

      return true;
    }

    return false;
  }

  // \Brief copyChildMaps
  // Copy an input RuntimeMap vector.
  void DataMap::copyChildDataMaps(const std::vector<RuntimeMap *> &
				     childMaps) {
    for(unsigned int i = 0; i < childMaps.size(); i++) {
      m_childDataMaps.push_back(childMaps[i]->clone());
    }
  }

  // \brief copyChildMaps
  // Copy an input LocalMap vector.
  void DataMap::copyChildDataMaps(const std::vector<LocalMap *> &
				     childMaps) {
    for(unsigned int i = 0; i < childMaps.size(); i++) {
      m_childDataMaps.push_back(childMaps[i]->clone());
    }
  }

  // \brief copyChildMaps
  // Copy an input AutoMap vector.
  void DataMap::copyChildDataMaps(const std::vector<AutoMap *> &
				     childMaps) {
    for(unsigned int i = 0; i < childMaps.size(); i++) {
      m_childDataMaps.push_back(childMaps[i]->clone());
    }
  }

  // \brief copyChildDataMaps
  void
  DataMap::copyChildDataMaps(const std::vector<DataMap *> & childDataMaps) {
    for(unsigned int i = 0; i < childDataMaps.size(); i++) {
      m_childDataMaps.push_back(childDataMaps[i]->clone());
    }
  }

  // \brief freeDataMapMemory()
  void DataMap::freeDataMapMemory() {

    // Iterate over child DataMap objects, deleting each.
    std::vector<DataMap *>::iterator it;
    for(it = m_childDataMaps.begin(); it != m_childDataMaps.end(); it++) {
      if(*it != NULL) {
	delete (*it);
	*it = NULL;
      }
    }

    // Delete the temporal map object.
    if(m_temporalMapPtr != NULL) {
      delete m_temporalMapPtr;
      m_temporalMapPtr = NULL;
    }
  }

} // namespace ipvtol


#endif // PVTOL_DATAMAP_CC_
