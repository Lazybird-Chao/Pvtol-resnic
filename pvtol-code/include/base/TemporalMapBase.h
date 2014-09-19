/**
 *  File: TemporalMapBase.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TemporalMapBase.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TemporalMapBase abstract base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TemporalMapBase.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TEMPORALMAPBASE_H_
#define PVTOL_TEMPORALMAPBASE_H_


#include <vector>
#include <TemporalBlockingInfo.h>
#include <DataMgmtPolicy.h>

namespace ipvtol
{
  class TemporalMapBase;

  /// \brief class TemporalMapBase
  /// The TemporalMapBase class is used as an abstract base for the 
  /// TemporalMap and AutoTemporalMap child classes.
  /// The set of TemporalMapBase classes is used to define out-of-core
  /// data distributions and policies.
  class TemporalMapBase {
    
  public:
    
    /// \brief enum TemporalMapBaseType
    enum TemporalMapBaseType {
      AUTO_MAP,
      RUNTIME_MAP,
    };

    /// \brief Constructor
    /// Default constructor for a TemporalMapBase.  It is recommended that 
    /// this method is only used internal to PVTOL in combination with the 
    /// deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    TemporalMapBase(const TemporalMapBaseType);		

    /// \brief Constructor
    /// Construct a TemporalMapBasee, given a TemporalMapBaseType and a
    /// TemporalBlockingInfo object.
    TemporalMapBase(const TemporalMapBaseType, 
		    const TemporalBlockingInfo &);
    
    /// \brief Constructor
    /// Construct a TemporalMapBasee, given a TemporalMapBaseType, a
    /// TemporalBlockingInfo object, and a child TemporalMapBase.
    TemporalMapBase(const TemporalMapBaseType, 
		    const TemporalBlockingInfo &,
		    const TemporalMapBase &);
    
    /// \brief Constructor
    /// Construct a TemporalMapBasee, given a TemporalMapBaseType and a
    /// TemporalBlockingInfo object.
    TemporalMapBase(const TemporalMapBaseType, const TemporalBlockingInfo &,
		    const DataMgmtPolicy &);
    
    /// \brief Constructor
    /// Construct a TemporalMapBasee, given a TemporalMapBaseType, a
    /// TemporalBlockingInfo object, and a child TemporalMapBase.
    TemporalMapBase(const TemporalMapBaseType, const TemporalBlockingInfo &,
		    const DataMgmtPolicy &, const TemporalMapBase &);
    
    /// \brief Copy contructor
    /// Construct a TemporalMapBase, given another TemporalMapBase.
    TemporalMapBase(const TemporalMapBase &);
    
    /// \brief Destructor
    virtual ~TemporalMapBase() = 0;
    
    /// \brief getMapType
    TemporalMapBaseType getTemporalMapBaseType() const;
    
    /// \brief hasChildTemporalMapBase
    /// Return a flag indicating whether or not the TemporalMapBase has a child
    /// TemporalMapBase.
    bool hasChildTemporalMap() const;
    
    /// \brief getChildTemporalMap
    /// Return the child TemporalMapBase.  This method should only be used if
    /// the hasChildTemporalMapBase method returns true.
    virtual const TemporalMapBase & getChildTemporalMap() const;

    /// \brief getNumDimensions
    /// Return the number of dimensions in the temporal map.
    unsigned int getNumDimensions() const;
    
    /// \brief getTemporalBlockingInfo()
    const TemporalBlockingInfo & getTemporalBlockingInfo() const;

    /// \brief getDataMgmtPolicy()
    const DataMgmtPolicy & getDataMgmtPolicy() const;

    /// \brief getSerializedSize
    /// Return size of the serialized object.
    int getSerializedSize() const;
    
    /// \brief serialize
    /// Store all object information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    int * serialize(int * buffer) const;
    
    /// \brief deserialize
    /// Initialize the object with the information supplied within
    /// the buffer.  Return the buffer, incremented to the next available 
    /// storage location.
    int * deserialize(int * buffer);    

    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is 
    /// responsible for managing the returned memory.
    virtual TemporalMapBase * clone() const = 0;
    
    /// \brief Assignment operator
    const TemporalMapBase & operator=(const TemporalMapBase & other);

    /// \brief Equality operator
    bool operator==(const TemporalMapBase & other) const;
    
    /// \brief Inequality operator
    bool operator!=(const TemporalMapBase & other) const;
    
  protected:

    TemporalMapBaseType  m_temporalMapType;
    TemporalBlockingInfo m_temporalBlockingInfo;
    DataMgmtPolicy       m_dataMgmtPolicy;
    bool                 m_hasChildTemporalMap;
    TemporalMapBase *    m_childTemporalMapPtr;
    
  private:
    
    /// \brief freeTemporalMapBaseMemory
    /// Free all allocated TemporalMapBase memory.
    void freeTemporalMapBaseMemory();
    
  }; // class TemporalMapBase
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------
  

  // \brief Constructor
  // Default constructor for a TemporalMapBase.  It is recommended that 
  // this method is only used internal to PVTOL in combination with the 
  // deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBaseType type)
    : m_temporalMapType(type),
      m_hasChildTemporalMap(false),
      m_childTemporalMapPtr(NULL)
  {
  }

  // \brief Constructor
  // Construct a TemporalMapBasee, given a TemporalMapBaseType and a
  // TemporalBlockingInfo object.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBaseType type, 
				   const TemporalBlockingInfo & blockingInfo)
    : m_temporalMapType(type),
      m_temporalBlockingInfo(blockingInfo),
      m_dataMgmtPolicy(DataMgmtPolicy()),
      m_hasChildTemporalMap(false),
      m_childTemporalMapPtr(NULL)
  {
  }
  
  // \brief Constructor
  // Construct a TemporalMapBasee, given a TemporalMapBaseType, a
  // TemporalBlockingInfo object, and a child TemporalMapBase.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBaseType type, 
				   const TemporalBlockingInfo & blockingInfo,
				   const TemporalMapBase & childMap)
    : m_temporalMapType(type),
      m_temporalBlockingInfo(blockingInfo),
      m_dataMgmtPolicy(DataMgmtPolicy()),
      m_hasChildTemporalMap(true),
      m_childTemporalMapPtr(childMap.clone())
  {
  }
    
  // \brief Constructor
  // Construct a TemporalMapBasee, given a TemporalMapBaseType and a
  // TemporalBlockingInfo object.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBaseType type, 
				   const TemporalBlockingInfo & blockingInfo,
				   const DataMgmtPolicy & dataMgmtPolicy)
    : m_temporalMapType(type),
      m_temporalBlockingInfo(blockingInfo),
      m_dataMgmtPolicy(dataMgmtPolicy),
      m_hasChildTemporalMap(false),
      m_childTemporalMapPtr(NULL)
  {
  }
      
  
  // \brief Constructor
  // Construct a TemporalMapBasee, given a TemporalMapBaseType, a
  // TemporalBlockingInfo object, and a child TemporalMapBase.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBaseType type,
				   const TemporalBlockingInfo & blockingInfo,
				   const DataMgmtPolicy & dataMgmtPolicy, 
				   const TemporalMapBase & childMap)
    : m_temporalMapType(type),
      m_temporalBlockingInfo(blockingInfo),
      m_dataMgmtPolicy(dataMgmtPolicy),
      m_hasChildTemporalMap(true),
      m_childTemporalMapPtr(childMap.clone())
  {
  }
    
  
  // \brief Copy contructor
  // Construct a TemporalMapBase, given another TemporalMapBase.
  inline
  TemporalMapBase::TemporalMapBase(const TemporalMapBase & other)
    : m_temporalMapType(other.m_temporalMapType),
      m_temporalBlockingInfo(other.m_temporalBlockingInfo),
      m_dataMgmtPolicy(other.m_dataMgmtPolicy),
      m_hasChildTemporalMap(other.m_hasChildTemporalMap)
  {
    if(other.m_hasChildTemporalMap) {
      m_childTemporalMapPtr = other.m_childTemporalMapPtr->clone();
    }
    else {
      m_childTemporalMapPtr = NULL;
    }
  }

  // \brief Destructor
  inline
  TemporalMapBase::~TemporalMapBase() {
    freeTemporalMapBaseMemory();
  }
  
  // \brief getMapType
  inline TemporalMapBase::TemporalMapBaseType
  TemporalMapBase::getTemporalMapBaseType() const {
    return m_temporalMapType;
  }

  // \brief getNumDimensions
  // Return the number of dimensions in the temporal map.
  inline
  unsigned int TemporalMapBase::getNumDimensions() const {
    return m_temporalBlockingInfo.getNumDimensions();
  }
  
  // \brief hasChildTemporalMap
  // Return a flag indicating whether or not the TemporalMap has a child
  // TemporalMap.
  inline
  bool TemporalMapBase::hasChildTemporalMap() const {
    return m_hasChildTemporalMap;
  }
  
  // \brief getChildTemporalMap
  // Return the child TemporalMap.  This method should only be used if
  // the hasChildTemporalMap method returns true.
  inline
  const TemporalMapBase & TemporalMapBase::getChildTemporalMap() const {
    return *m_childTemporalMapPtr;
  }  

  // \brief getTemporalBlockingInfo()
  inline
  const TemporalBlockingInfo & 
  TemporalMapBase::getTemporalBlockingInfo() const {
    return m_temporalBlockingInfo;
  }

  // \brief getDataMgmtPolicy()
  inline
  const DataMgmtPolicy & TemporalMapBase::getDataMgmtPolicy() const {
    return m_dataMgmtPolicy;
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int TemporalMapBase::getSerializedSize() const {

    // Size of the blockingInfo + dataMgmtPolicy + 1 for m_hasChildTemporalMap.
    int size 
      = m_temporalBlockingInfo.getSerializedSize()
      + m_dataMgmtPolicy.getSerializedSize() + 1;

    if(m_hasChildTemporalMap) {
      // Add the size of the child TemporalMap + 1 for the type of TemporalMap.
      return size + m_childTemporalMapPtr->getSerializedSize() + 1;
    }
    else {
      return size;
    }
  }
  
  // \brief Inequality operator
  inline
  bool TemporalMapBase::operator!=(const TemporalMapBase & other) const {
    return !(*this == other);
  }

} // namespace ipvtol


#endif // PVTOL_TEMPORALMAPBASE_H_ not defined

