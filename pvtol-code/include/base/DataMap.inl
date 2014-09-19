/**
 *  File: DataMap.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Inline methods for abstract base class DataMap.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataMap.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATAMAP_INL_
#define PVTOL_DATAMAP_INL_


#include <vector>
#include <Map.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <TemporalMapBase.h>

namespace ipvtol
{

  // \brief Constructor
  // This constructor is used to build an incomplete DataMap.  
  // This should only be used internal to PVTOL.  It should be 
  // used in combination with the deserialization methods.
  // More initialization needs to be done before an object 
  // created with this constructor can be used.
  inline
  DataMap::DataMap()
    : Map(Map::DATA_MAP),
      m_numChildDataMaps(0),
      m_hasTemporalMap(false),
      m_temporalMapPtr(NULL)
           
  {
  }

  // \brief Constructor
  // Construct a DataMap, given a DataMapType.  This constructor is 
  // used to build incomplete DataMaps.  This should only
  // be used internal to PVTOL, and should be used only by the
  // DataMap child objects.  It should be used in combination with the
  // deserialization methods.
  inline
  DataMap::DataMap(const DataMapType dataMapType)
    : Map(Map::DATA_MAP),
      m_numChildDataMaps(0),
      m_hasTemporalMap(false),
      m_temporalMapPtr(NULL),
      m_dataMapType(dataMapType)
  {
  }

  // \brief Constructor
  // Construct a DataMap, given a DataMapType, RankList, Grid, and 
  // DataDistDescription.
  inline
  DataMap::DataMap(const DataMapType dataMapType, const RankList & rankList, 
		   const Grid & grid, 
		   const DataDistDescription & dataDistDescription)
    : Map(Map::DATA_MAP, rankList, grid, dataDistDescription),
      m_numChildDataMaps(0),
      m_hasTemporalMap(false),
      m_temporalMapPtr(NULL),
      m_dataMapType(dataMapType)
  {
  }
  
  // \brief Constructor
  // Construct a DataMap, given a DataMapType, RankList, Grid,
  // DataDistDescription, and child data maps.
  inline
  DataMap::DataMap(const DataMapType dataMapType, const RankList & rankList, 
		   const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<DataMap *> & childDataMaps)
    : Map(Map::DATA_MAP, rankList, grid, dataDistDescription),
      m_numChildDataMaps(childDataMaps.size()),
      m_hasTemporalMap(false),
      m_temporalMapPtr(NULL),
      m_dataMapType(dataMapType)
  {
    copyChildDataMaps(childDataMaps);
  }
  
  // \brief Constructor
  // Construct a DataMap, given a DataMapType, RankList, Grid,
  // DataDistDescription, and a temporal map.
  inline
  DataMap::DataMap(const DataMap::DataMapType dataMapType, 
		   const RankList & rankList, 
		   const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const TemporalMapBase & temporalMap)
    : Map(Map::DATA_MAP, rankList, grid, dataDistDescription),
      m_numChildDataMaps(0),
      m_hasTemporalMap(true),
      m_temporalMapPtr(temporalMap.clone()),
      m_dataMapType(dataMapType)
  {
  }
  
  // \brief Constructor
  // Construct a DataMap, given a DataMapType, RankList, Grid,
  // DataDistDescription, child data maps, and a temporal map.
  inline
  DataMap::DataMap(const DataMap::DataMapType dataMapType, 
		   const RankList & rankList, 
		   const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<DataMap *> & childDataMaps, 
		   const TemporalMapBase & temporalMap)
    : Map(Map::DATA_MAP, rankList, grid, dataDistDescription),
      m_numChildDataMaps(childDataMaps.size()),
      m_hasTemporalMap(true),
      m_temporalMapPtr(temporalMap.clone()),
      m_dataMapType(dataMapType)
  {
    copyChildDataMaps(childDataMaps);
  }
  
  // \brief Destructor
  inline
  DataMap::~DataMap() {
    freeDataMapMemory();
  }
  
  // \brief getDataMapType()
  inline
  DataMap::DataMapType DataMap::getDataMapType() const {
    return m_dataMapType;
  }
  
  // \brief getDistDescription
  // Return the requested DistDescription object.
  inline const DataDistDescription & 
  DataMap::getDistDescription() const {
    return *dynamic_cast<const DataDistDescription *>(m_distDescription);
  }
  
  // \brief hasTemporalMap
  // Return a boolean indicating whether or not the DataMap contains a
  // TemporalMap.
  inline
  bool DataMap::hasTemporalMap() const {
    return m_hasTemporalMap;
  }
  
  // \brief getTemporalMap
  // Return the TemporalMap.
  inline
  const TemporalMapBase & DataMap::getTemporalMap() const {
    return *m_temporalMapPtr;
  }
  
  // \brief getChildDataMap
  inline
  const DataMap & DataMap::getChildDataMap(int index) const {
    return *m_childDataMaps[index];
  }
  
  // \brief getNumChildDataMaps
  inline
  int DataMap::getNumChildDataMaps() const {
    return m_numChildDataMaps;
  }

  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  DataMap * DataMap::clone() const {
    return new DataMap(*this);
  }  

  // \brief Inequality operator
  inline
  bool DataMap::operator!=(const DataMap & other) const {
    return !(*this == other);
  }

} // namespace ipvtol


#endif // PVTOL_DATAMAP_INL_ not defined
