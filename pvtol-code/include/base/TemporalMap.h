/**
 *  File: TemporalMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TemporalMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the TemporalMap class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TemporalMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TEMPORALMAP_H_
#define PVTOL_TEMPORALMAP_H_


#include <vector>
#include <DataMap.h>
#include <TemporalMapBase.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <DataMgmtPolicy.h>

namespace ipvtol
{

  /// \brief class TemporalMap
  /// The TemporalMap class is used to fully define out-of-core
  /// data distributions and policies.
  class TemporalMap : public TemporalMapBase {
    
  public:

    /// \brief Constructor
    /// Default constructor for a TemporalMap.  It is recommended that this 
    /// method is only used internal to PVTOL in combination with the 
    /// deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    TemporalMap();

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object.
    TemporalMap(const TemporalBlockingInfo &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo ojbect and
    /// a child TemporalMap.
    TemporalMap(const TemporalBlockingInfo &, const TemporalMap &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object and
    /// a DataMgmtPolicy.
    TemporalMap(const TemporalBlockingInfo &, const DataMgmtPolicy &);

    /// \brief Constructor
    /// Construct a TemporalMap, given a TemporalBlockingInfo object, a
    /// a DataMgmtPolicy, and a child TemporalMap.
    TemporalMap(const TemporalBlockingInfo &, const DataMgmtPolicy &,
		const TemporalMap &);
    
    /// \brief Copy contructor
    /// Construct a TemporalMap, given another TemporalMap.
    TemporalMap(const TemporalMap &);
    
    /// \brief Destructor
    virtual ~TemporalMap();
    
    /// \brief getChildTemporalMap
    /// Return the child TemporalMap.  This method should only be used if
    /// the hasChildTemporalMap method returns true.
    virtual const TemporalMap & getChildTemporalMap() const;
    
    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual TemporalMapBase * clone() const;
    
  }; // class TemporalMap
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Default constructor for a TemporalMap.  It is recommended that this 
  // method is only used internal to PVTOL in combination with the 
  // deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  TemporalMap::TemporalMap()
    : TemporalMapBase(TemporalMapBase::RUNTIME_MAP)
  {
  }

  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object.
  inline
  TemporalMap::TemporalMap(const TemporalBlockingInfo & blockingInfo)
    : TemporalMapBase(TemporalMapBase::RUNTIME_MAP,
		      blockingInfo)
  {
  }
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo ojbect and
  // a child TemporalMap.
  inline
  TemporalMap::TemporalMap(const TemporalBlockingInfo & blockingInfo, 
			   const TemporalMap & childTemporalMap)
    : TemporalMapBase(TemporalMapBase::RUNTIME_MAP,
		      blockingInfo, childTemporalMap)
  {
  }    
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object and
  // a DataMgmtPolicy.
  inline
  TemporalMap::TemporalMap(const TemporalBlockingInfo & blockingInfo, 
			   const DataMgmtPolicy & dataMgmtPolicy)
    : TemporalMapBase(TemporalMapBase::RUNTIME_MAP,
		      blockingInfo, dataMgmtPolicy)
  {
  }
  
  // \brief Constructor
  // Construct a TemporalMap, given a TemporalBlockingInfo object, a
  // a DataMgmtPolicy, and a child TemporalMap.
  inline
  TemporalMap::TemporalMap(const TemporalBlockingInfo & blockingInfo, 
			   const DataMgmtPolicy & dataMgmtPolicy,
			   const TemporalMap & childTemporalMap)
    : TemporalMapBase(TemporalMapBase::RUNTIME_MAP,
		      blockingInfo, dataMgmtPolicy, childTemporalMap)
  {
  }
  
  // \brief Copy contructor
  // Construct a Map, given another Map.
  inline
  TemporalMap::TemporalMap(const TemporalMap & other)
    : TemporalMapBase(other)
  {
  }
  
  // \brief Destructor
  inline
  TemporalMap:: ~TemporalMap() {
  }
  
  // \brief getChildTemporalMap
  // Return the child TemporalMap.  This method should only be used if
  // the hasChildTemporalMap method returns true.
  inline
  const TemporalMap & TemporalMap::getChildTemporalMap() const {
    return dynamic_cast<const TemporalMap &>(*m_childTemporalMapPtr);
  }
  
  // \brief clone
  // Allocate and create a copy of the current object.  The user is 
  // responsible for managing the returned memory.
  inline
  TemporalMapBase * TemporalMap::clone() const {
    return new TemporalMap(*this);
  }

} // namespace ipvtol


#endif // PVTOL_TEMPORALMAP_H_ not defined

