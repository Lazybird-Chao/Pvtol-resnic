/**
 *  File: RuntimeMap.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Inline methods for the RuntimeMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: RuntimeMap.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_RUNTIMEMAP_INL_
#define PVTOL_RUNTIMEMAP_INL_


#include <vector>
#include <DataMap.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <TemporalMap.h>

namespace ipvtol
{

  // \brief Constructor
  // Default constructor for a RuntimeMap.  It is recommended that this 
  // method is only used internal to PVTOL in combination with the 
  // deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  RuntimeMap::RuntimeMap()
    : DataMap(DataMap::RUNTIME_MAP)
  {
  }

  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, and 
  // vector of DataDistDescription pointers.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, dataDistDescription)
  {
  }
  
  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child RuntimeMap.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription, 
			 const std::vector<RuntimeMap *> & childDataMaps)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, dataDistDescription)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }
  
  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child RuntimeMap.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription, 
			 const std::vector<LocalMap *> & childDataMaps)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, 
	      dataDistDescription)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }
  
  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, and a RuntimeTemporal map.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription, 
			 const TemporalMap & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
  }
  
  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child RuntimeMap, 
  // and a TemporalMap.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription, 
			 const std::vector<RuntimeMap *> & childDataMaps, 
			 const TemporalMap & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }
  
  // \brief Constructor
  // Construct a RuntimeMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child LocalMap, 
  // and a TemporalMap.
  inline
  RuntimeMap::RuntimeMap(const RankList & rankList, const Grid & grid, 
			 const DataDistDescription & dataDistDescription, 
			 const std::vector<LocalMap *> & childDataMaps, 
			 const TemporalMap & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }
  
  // \brief Copy contructor
  // Construct a RuntimeMap, given another RuntimeMap.
  inline
  RuntimeMap::RuntimeMap(const RuntimeMap & other)
    : DataMap(other)
  {
  }
  
  // \brief Destructor
  inline
  RuntimeMap::~RuntimeMap() { 
  }
  
  // \brief getTemporalMap
  // Return the TemporalMap.  Use hasTemporalMap prior to this method
  // to be sure the RuntimeMap contains a TemporalMap.
  inline
  const TemporalMapBase & RuntimeMap::getTemporalMap() const {
    return *dynamic_cast<const TemporalMap *>(m_temporalMapPtr);
  }
  
  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  DataMap * RuntimeMap::clone() const {
    return new RuntimeMap(*this);
  }
  
} // namespace ipvtol


#endif // PVTOL_RUNTIMEMAP_INL_ not defined

