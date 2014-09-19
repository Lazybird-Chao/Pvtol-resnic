/**
 *  File: TaskMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TaskMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TaskMap class.

 *
 *  Author: Ryan Haney
 *
 *  $Id: TaskMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TASKMAP_H_
#define PVTOL_TASKMAP_H_

#include <RankList.h>
#include <TaskDistDescription.h>
#include <Map.h>
#include <Grid.h>


namespace ipvtol
{

  /// \brief class TaskMap
  /// The TaskMap class is used to provide mapping information for 
  /// Tasks.  It inherits from the abstract base Map class.
  class TaskMap : public Map {
     friend class boost::serialization::access;
    
  public:

    /// \brief Default Constructor
    /// This constructor is used to build an incomplete TaskMap.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    TaskMap();    

    /// \brief Constructor
    /// Construct a TaskMap, given a RankList, Grid, and 
    /// TaskDistDescription object.
    TaskMap(const RankList &, const TaskDistDescription &); 
    
    /// \brief Constructor
    /// Construct a TaskMap, given a size.  A task map will be constructed
    /// with a standard distribution, with ranks 0 to taskSize-1.
    TaskMap(int taskSize); 
    
    /// \brief Constructor
    /// Construct a TaskMap, given a RankList.
    TaskMap(const RankList &);
    
    /// \brief Constructor
    /// Construct a TaskMap, given a size and C-style array of RankIds.
    TaskMap(int size, const RankId *);
    
    /// \brief Copy contructor
    /// Construct a Map, given another Map.
    TaskMap(const TaskMap &);
    
    /// \brief Destructor
    virtual ~TaskMap();
    
    /// \brief getDistDescription
    /// Return the requested DistDescription object.
    virtual const TaskDistDescription & getDistDescription() const;
    
    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
   
  }; // class TaskMap
  

  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Construct a TaskMap, given a RankList, Grid, and 
  // TaskDistDescription object.
  inline
  TaskMap::TaskMap(const RankList & rankList, 
		   const TaskDistDescription & distDescription)
    : Map(Map::TASK_MAP, rankList, Grid(rankList.getNumRanks()), 
	  distDescription)
  {
  }
  
  // \brief Constructor
  // Construct a TaskMap, given a size.  A task map will be constructed
  // with a standard distribution, with ranks 0 to taskSize-1.
  inline
  TaskMap::TaskMap(int taskSize) 
    : Map(Map::TASK_MAP, RankList(taskSize), Grid(taskSize), 
	  TaskDistDescription())
  {
  }
  
  /// \brief Constructor
  /// Construct a TaskMap, given a RankList.
  inline
  TaskMap::TaskMap(const RankList & rankList)
    : Map(Map::TASK_MAP, rankList, Grid(rankList.getNumRanks()),
	  TaskDistDescription())
  {
  }
  
  /// \brief Constructor
  /// Construct a TaskMap, given a size and C-style array of RankIds.
  inline
  TaskMap::TaskMap(int size, const RankId * ranks)
    : Map(Map::TASK_MAP, RankList(size, ranks), Grid(size),
	  TaskDistDescription())
  {
  }

  // \brief Copy contructor
  // Construct a Map, given another Map.
  inline
  TaskMap::TaskMap(const TaskMap & other)
    : Map(other)
  {
  }

  // \brief Destructor
  inline
  TaskMap::~TaskMap() {
  }
  
  // \brief getDistDescription
  // Return the requested dimension's TaskDistDescription object.
  inline
  const TaskDistDescription & TaskMap::getDistDescription() const {
    return *dynamic_cast<const TaskDistDescription *>(m_distDescription);
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void TaskMap::serialize(Archive & ar, const unsigned int version) {
    // This line tells Boost::serialize that it also needs to serialize
    // the base class
    ar & boost::serialization::base_object<Map>(*this);
  }  
  
} // namespace ipvtol


#endif // PVTOL_TASKMAP_H_ not defined

