/**
 *  File: RuntimeMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    RuntimeMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the RuntimeMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: RuntimeMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_RUNTIMEMAP_H_
#define PVTOL_RUNTIMEMAP_H_


#include <vector>
#include <DataMap.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <LocalMap.h>

namespace ipvtol
{

  class TemporalMap;
  
  /// \brief class RuntimeMap
  /// The RuntimeMap class is used to provide mapping information for 
  /// hierarchical arrays where the entire map is known at runtime.
  class RuntimeMap : public DataMap {
    
  public:
    /// \brief Constructor
    /// Default constructor for a RuntimeMap.  It is recommended that this 
    /// method is only used internal to PVTOL in combination with the 
    /// deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    RuntimeMap();
        
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, and 
    /// vector of DataDistDescription pointers.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &);
    
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child RuntimeMap.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &, 
	       const std::vector<RuntimeMap *> &);
    
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child RuntimeMap.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &, 
	       const std::vector<LocalMap *> &);
    
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, and a RuntimeTemporal map.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &, 
	       const TemporalMap &);
    
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child RuntimeMap, 
    /// and a TemporalMap.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &, 
	       const std::vector<RuntimeMap *> &, const TemporalMap &);
    
    /// \brief Constructor
    /// Construct a RuntimeMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child LocalMap, 
    /// and a TemporalMap.
    RuntimeMap(const RankList &, const Grid &, 
	       const DataDistDescription &, 
	       const std::vector<LocalMap *> &, const TemporalMap &);
    
    /// \brief Copy contructor
    /// Construct a RuntimeMap, given another RuntimeMap.
    RuntimeMap(const RuntimeMap &);
    
    /// \brief Destructor
    virtual ~RuntimeMap();
    
    /// \brief getTemporalMap
    /// Return the TemporalMap.  Use hasTemporalMap prior to this method
    /// to be sure the RuntimeMap contains a TemporalMap.
    virtual const TemporalMapBase & getTemporalMap() const;
    
    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual DataMap * clone() const;
    
  }; // class RuntimeMap

} // namespace ipvtol

// Include inline definitions
#include <RuntimeMap.inl>


#endif // PVTOL_RUNTIMEMAP_H_ not defined

