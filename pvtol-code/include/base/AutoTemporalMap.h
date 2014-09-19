/**
 *  File: AutoTemporalMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    AutoTemporalMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the AutoTemporalMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: AutoTemporalMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_AUTOTEMPORALMAP_H_
#define PVTOL_AUTOTEMPORALMAP_H_


#include <vector>
#include <TemporalMapBase.h>
#include <DataMap.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>

namespace ipvtol
{

  /// \brief class AutoTemporalMap 
  /// The AutoTemporalMap class is used to partially define out-of-core data 
  /// distributions and policies to be filled in by an automatic mapping 
  /// utility.
  class AutoTemporalMap : public TemporalMapBase {

  public:
    
    enum ParameterType {
      AUTO_MAPPABLE
    };

    /// \brief Constructor
    /// Default constructor for an AutoTemporalMap.  It is recommended that 
    /// this method is only used internal to PVTOL in combination with the 
    /// deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    AutoTemporalMap();

    /// \brief Constructor
    // AutoTemporalMap(ParameterType type = AUTO_MAPPABLE);
    
    /// \brief Constructor
    // AutoTemporalMap(ParameterType type, const TemporalMapBase &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object.
    AutoTemporalMap(const TemporalBlockingInfo &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo ojbect and
    /// a child TemporalMap.
    AutoTemporalMap(const TemporalBlockingInfo &, const TemporalMapBase &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object and
    /// a DataMgmtPolicy.
    AutoTemporalMap(const TemporalBlockingInfo &, const DataMgmtPolicy &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object, a
    /// a DataMgmtPolicy, and a child TemporalMap.
    AutoTemporalMap(const TemporalBlockingInfo &, const DataMgmtPolicy &,
		    const TemporalMapBase &);
    
    /// \brief Copy contructor
    /// Construct a Map, given another Map.
    AutoTemporalMap(const AutoTemporalMap &);
    
    /// \brief Destructor
    virtual ~AutoTemporalMap();
    
    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual TemporalMapBase * clone() const;
    
  }; // class AutoTemporalMap
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Default constructor for an AutoTemporalMap.  It is recommended that this 
  // method is only used internal to PVTOL in combination with the 
  // deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  AutoTemporalMap::AutoTemporalMap()
    : TemporalMapBase(TemporalMapBase::AUTO_MAP)
  {
  }

  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object.
  inline
  AutoTemporalMap::AutoTemporalMap(const TemporalBlockingInfo & blockingInfo)
    : TemporalMapBase(TemporalMapBase::AUTO_MAP, blockingInfo)
  {
  }
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo ojbect and
  // a child TemporalMap.
  inline
  AutoTemporalMap::AutoTemporalMap(const TemporalBlockingInfo & blockingInfo, 
				   const TemporalMapBase & childTemporalMap)
    : TemporalMapBase(TemporalMapBase::AUTO_MAP, blockingInfo, 
		      childTemporalMap)
  {
  }    
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object and
  // a DataMgmtPolicy.
  inline
  AutoTemporalMap::AutoTemporalMap(const TemporalBlockingInfo & blockingInfo, 
				   const DataMgmtPolicy & dataMgmtPolicy)
    : TemporalMapBase(TemporalMapBase::AUTO_MAP, blockingInfo, 
		      dataMgmtPolicy)
  {
  }
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object, a
  // a DataMgmtPolicy, and a child TemporalMap.
  inline
  AutoTemporalMap::AutoTemporalMap(const TemporalBlockingInfo & blockingInfo, 
				   const DataMgmtPolicy & dataMgmtPolicy,
				   const TemporalMapBase & childTemporalMap)
    : TemporalMapBase(TemporalMapBase::AUTO_MAP, blockingInfo, 
		      dataMgmtPolicy, childTemporalMap)
  {
  }
  
  // \brief Copy contructor
  // Construct a Map, given another Map.
  inline
  AutoTemporalMap::AutoTemporalMap(const AutoTemporalMap & other)
    : TemporalMapBase(other)
  {
  }
  
  // \brief Destructor
  inline
  AutoTemporalMap::~AutoTemporalMap() {
  }
  
  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  TemporalMapBase * AutoTemporalMap::clone() const {
    return new AutoTemporalMap(*this);
  }

} // namespace ipvtol


#endif // PVTOL_AUTOTEMPORALMAP_H_ not defined

