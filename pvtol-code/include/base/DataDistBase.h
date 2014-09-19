/**
 *  File: DataDistBase.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DataDistBase.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the DataDistBase abstract base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataDistBase.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATADISTBASE_H_
#define PVTOL_DATADISTBASE_H_


namespace ipvtol
{

  /// \brief class DataDistBase
  /// The DataDistBase class is used to describe how
  /// data is distributed for a single dimension.  The choices
  /// are:
  ///    - Block: The data is evenly divided over the specified
  ///             processes/ranks, in order.
  ///    - Block-cyclic: Data is divided over the specified processes
  ///             or ranks.  It is done so in blockSize blocks of data, 
  ///             round-robin over the ranks until all data is distributed.
  ///    - Cyclic: Defines a Block-cyclic distribution where the 
  ///             blockSize = 1.
  ///    - Replicated: A full copy of the data (in one dimension) is 
  ///             replicated over each process/rank.
  ///    - Mappable: The distribution type and sizes are left up to
  ///             the automatic mapping utility.  This option can only
  ///             be used in AutoMaps.
  class DataDistBase {
    
  public:
  
    /// \brief DataDescriptionType
    /// Enumeration to give more information as to what the Distribution 
    /// Description is applied to.
    enum DataDistType {
      BLOCK,
      BLOCK_CYCLIC,
      CYCLIC,
      REPLICATED
    };
    
    /// \brief Constructor
    DataDistBase(DataDistType dataDistType,
		 unsigned int blockSize, unsigned int overlapSize);
    
    /// \brief Copy constructor
    DataDistBase(const DataDistBase & other);
    
    /// \brief Destructor
    virtual ~DataDistBase() = 0;
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual DataDistBase * clone() const = 0;
    
    /// \brief getDataDistType
    /// Return the DataDistType at the specified dimension.  I.e. Block, 
    /// cyclic, etc.
    DataDistType getDataDistType() const;
    
    /// \brief getBlockSize
    unsigned int getBlockSize() const;
    
    /// \brief getOverlapSize
    unsigned int getOverlapSize() const;
    
    /// \brief
    /// getSerializedSize
    /// Return size of the serialized object.
    int getSerializedSize() const;
    
    /// \brief serialize
    /// Store all object information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    int * serialize(int * buffer) const;
        
    /// \brief Assignment operator
    const DataDistBase & operator=(const DataDistBase & other);
    
    /// \brief Equality operator
    bool operator==(const DataDistBase & other) const;
    
    /// \brief Inequality operator
    bool operator!=(const DataDistBase & other) const;
    
  protected:
    DataDistType m_dataDistType;
    unsigned int m_blockSize;
    unsigned int m_overlapSize;
    
  }; // class DataDistBase

  
  //------------------
  // INLINE FUNCTIONS
  //------------------

  // \brief Constructor
  inline
  DataDistBase::DataDistBase(DataDistType dataDistType,
			     unsigned int blockSize, unsigned int overlapSize)
    : m_dataDistType(dataDistType),
      m_blockSize(blockSize),
      m_overlapSize(overlapSize)
  {
  }
  
  // \brief Copy constructor
  inline
  DataDistBase::DataDistBase(const DataDistBase & other)
    : m_dataDistType(other.m_dataDistType),
      m_blockSize(other.m_blockSize),
      m_overlapSize(other.m_overlapSize)
  {
  }
  
  // \brief Destructor
  inline
  DataDistBase::~DataDistBase() {
  }
  
  // \brief getDataDistType
  // Return the DataDistType at the specified dimension.  I.e. Block, 
  // cyclic, etc.
  inline 
  DataDistBase::DataDistType DataDistBase::getDataDistType() const {
    return m_dataDistType;
  }
  
  // \brief getBlockSize
  inline
  unsigned int DataDistBase::getBlockSize() const {
    return m_blockSize;
  }
  
  // \brief getOverlapSize
  inline
  unsigned int DataDistBase::getOverlapSize() const {
    return m_overlapSize;
  }

  // \brief
  // getSerializedSize
  // Return size of the serialized object.
  inline
  int DataDistBase::getSerializedSize() const {
    return 3;
  }  

  // \brief Inequality operator
  inline
  bool DataDistBase::operator!=(const DataDistBase & other) const {
    return !(*this == other);
  }
  
} // namespace ipvtol


#endif // PVTOL_DATADISTBASE_H_ not defined

