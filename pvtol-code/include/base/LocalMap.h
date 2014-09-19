/**
 *  File: LocalMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    LocalMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the LocalMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: LocalMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_LOCALMAP_H_
#define PVTOL_LOCALMAP_H_


#include <DataMap.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <BlockDist.h>
#include <TaskManager.h>
#include <TaskBase.h>

namespace ipvtol
{

  /// \brief class LocalMap
  /// The LocalMap class is used to designate a non-distributed, local
  /// data distribution.
  class LocalMap : public DataMap {
    
  public:
    
    /// \brief Constructor
    /// Construct a LocalMap.
    LocalMap();
    
    /// \brief Copy contructor
    /// Construct a LocalMap, given another LocalMap.
    LocalMap(const LocalMap &);
    
    /// \brief Destructor
    virtual ~LocalMap();
    
    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual DataMap * clone() const;
    
  }; // class LocalMap
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Construct a LocalMap.
  inline
  LocalMap::LocalMap()
    : DataMap(DataMap::LOCAL_MAP, RankList(1), Grid(1,1,1),
	      DataDistDescription(BlockDist(), BlockDist(), BlockDist()))
  {
    m_rankList.setRank(0, TaskManager::getCurrentTask().getGlobalThreadRank());
  }
  
  // \brief Copy contructor
  // Construct a LocalMap, given another LocalMap.
  inline
  LocalMap::LocalMap(const LocalMap & other)
    : DataMap(other)
  {
  }
  
  // \brief Destructor
  inline
  LocalMap::~LocalMap() {
  }

  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  DataMap * LocalMap::clone() const {
    return new LocalMap(*this);
  }
  
} // namespace ipvtol


#endif // PVTOL_LOCALMAP_H_ not defined

