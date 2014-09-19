/**
 *  File: AutoMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    AutoMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the AutoMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: AutoMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_AUTOMAP_H_
#define PVTOL_AUTOMAP_H_


#include <vector>
#include <DataMap.h>
#include <RuntimeMap.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <TemporalMapBase.h>

namespace ipvtol
{

  /// \brief class AutoMap
  /// The AutoMap class is used to provide mapping information for 
  /// hierarchical arrays where the map is not fully defined at runtime.
  /// An automatic mapping utility is required to fill in any undefined
  /// portion of the map.
  class AutoMap : public DataMap {
    
  public:
    
    enum ParameterType {
      AUTO_MAPPABLE
    };

    /// \brief Constructor
    /// Default constructor for an AutoMap.  It is recommended that this 
    /// method is only used internal to PVTOL in combination with the 
    /// deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    AutoMap();

//   /// \brief Constructor
//   AutoMap(ParameterType type = AUTO_MAPPABLE);

//   /// \brief Constructor
//   AutoMap(ParameterType type, const DataMap &);
  
//   /// \brief Constructor
//   AutoMap(ParameterType type, const TemporalMapBase &);

//   /// \brief Constructor
//   AutoMap(ParameterType type, const DataMap &, const TemporalMapBase &);

    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, and 
    /// vector of DataDistDescription pointers.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, and a Temporal map vector.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const TemporalMapBase &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child DataMap vector.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<DataMap *> &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child AutoMap vector, 
    /// and a TemporalMapBase.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<DataMap *> &, const TemporalMapBase &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child DataMap.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<LocalMap *> &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child AutoMap vector, 
    /// and a TemporalMapBase.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<LocalMap *> &, const TemporalMapBase &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child RuntimeMap vector.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<RuntimeMap *> &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child RuntimeMap vector, 
    /// and a TemporalMapBase.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<RuntimeMap *> &, const TemporalMapBase &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers and a child AutoMap vector.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<AutoMap *> &);
    
    /// \brief Constructor
    /// Construct a AutoMap, given a RankList, Grid, 
    /// vector of DataDistDescription pointers, child AutoMap vector, 
    /// and a TemporalMapBase.
    AutoMap(const RankList &, const Grid &, 
	    const DataDistDescription &, 
	    const std::vector<AutoMap *> &, const TemporalMapBase &);
    
    /// \brief Copy contructor
    /// Construct a AutoMap, given another AutoMap.
    AutoMap(const AutoMap &);

    /// \brief Destructor
    virtual ~AutoMap();  
    
    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual DataMap * clone() const;
    
  }; // class AutoMap

  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Default constructor for an AutoMap.  It is recommended that this 
  // method is only used internal to PVTOL in combination with the 
  // deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  AutoMap::AutoMap()
    : DataMap(DataMap::AUTO_MAP)
  {
  }

//   // \brief Constructor
//   // \todo
//   inline
//   AutoMap::AutoMap(ParameterType type)
//   {
//   }
  
//   // \brief Constructor
//   // \todo
//   inline
//   AutoMap::AutoMap(ParameterType type, const DataMap &)
//   {
//   }

//   // \brief Constructor
//   // \todo
//   inline
//   AutoMap::AutoMap(ParameterType type, const TemporalMapBase &)
//   {
//   }
  
//   // \brief Constructor
//   // \todo
//   inline
//   AutoMap::AutoMap(ParameterType type, const DataMap &, 
// 		   const TemporalMapBase &)
//   {
//   }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, and 
  // vector of DataDistDescription pointers.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription)
    : DataMap(DataMap::AUTO_MAP, rankList, grid, dataDistDescription)
  {
  }
  
  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, and a Temporal map.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const TemporalMapBase & temporalMap)
    : DataMap(DataMap::AUTO_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
  }
  
  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child AutoMap.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<DataMap *> & childDataMaps)
    : DataMap(DataMap::AUTO_MAP, rankList, grid, 
	      dataDistDescription, childDataMaps)
  {
  }
  
  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child AutoMap, 
  // and a TemporalMapBase.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<DataMap *> & childDataMaps, 
		   const TemporalMapBase & temporalMap)
    : DataMap(DataMap::AUTO_MAP, rankList, grid,
	      dataDistDescription, childDataMaps, temporalMap)
  {
  }
  
  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child DataMap.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<LocalMap *> & childDataMaps)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, 
	      dataDistDescription)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child AutoMap vector, 
  // and a TemporalMapBase.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<LocalMap *> & childDataMaps, 
		   const TemporalMapBase & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child RuntimeMap vector.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<RuntimeMap *> & childDataMaps)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, 
	      dataDistDescription)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child RuntimeMap vector, 
  // and a TemporalMapBase.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<RuntimeMap *> & childDataMaps, 
		   const TemporalMapBase & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers and a child AutoMap vector.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<AutoMap *> & childDataMaps)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid, 
	      dataDistDescription)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }

  // \brief Constructor
  // Construct a AutoMap, given a RankList, Grid, 
  // vector of DataDistDescription pointers, child AutoMap vector, 
  // and a TemporalMapBase.
  inline
  AutoMap::AutoMap(const RankList & rankList, const Grid & grid, 
		   const DataDistDescription & dataDistDescription, 
		   const std::vector<AutoMap *> & childDataMaps, 
		   const TemporalMapBase & temporalMap)
    : DataMap(DataMap::RUNTIME_MAP, rankList, grid,
	      dataDistDescription, temporalMap)
  {
    copyChildDataMaps(childDataMaps);
    m_numChildDataMaps = childDataMaps.size();
  }
  

  // \brief Copy contructor
  // Construct a AutoMap, given another AutoMap.
  inline
  AutoMap::AutoMap(const AutoMap & other)
    : DataMap(other)
  {
  }
  
  // \brief Destructor
  inline
  AutoMap::~AutoMap() { 
  }
  
  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  DataMap * AutoMap::clone() const {
    return new AutoMap(*this);
  }

} // namespace ipvtol


#endif // PVTOL_AUTOMAP_H_ not defined

