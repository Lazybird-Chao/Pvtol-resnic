/**
 *  File: DataMgmtPolicy.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the DataMgmtPolicy class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataMgmtPolicy.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATAMGMTPOLICY_CC_
#define PVTOL_DATAMGMTPOLICY_CC_


#include <DataMgmtPolicy.h>

namespace ipvtol
{

  // \brief setLazyAllocationFlag
  // Set the boolean describing whether or not storage for subarrays 
  // should be allocated when first accessed.  True directs PVTOL to use lazy
  // allocation, allocating the array when first accessed.  False directs 
  // PVTOL to NOT use lazy allocation, allocating storage during array 
  // construction.  The default value is false.  This should only be used
  // for DataMgmtPolicy objects that have an undefined lazyAllocation policy.
  // This should only be used for AutoMaps.
  void DataMgmtPolicy::setLazyAllocationFlag(bool value) {
    m_lazyAllocation = value;
    m_lazyAllocationDefined = true;
  }

  // \brief getMultiBufferSize 
  // Sets an unsigned integer describing the multi-buffering depth for 
  // subarrays from the parent subarray.  For multi-buffering, a value
  // of 1 is the default.  Therefore to "double-buffer," a value of 2 is 
  // required.
  void DataMgmtPolicy::setMultiBufferSize(unsigned int value) {
    m_multiBufferSize = value;
    m_multiBufferSizeDefined = true;
  }

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * DataMgmtPolicy::serialize(int * buffer) const {
    *(buffer++) = static_cast<int>(m_lazyAllocation);    
    *(buffer++) = static_cast<int>(m_lazyAllocationDefined);
    *(buffer++) = static_cast<int>(m_saveData);
    *(buffer++) = static_cast<int>(m_loadData);
    *(buffer++) = static_cast<int>(m_multiBufferSize);
    *(buffer++) = static_cast<int>(m_multiBufferSizeDefined);

    return buffer;
  }
  
  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available 
  // storage location.
  int * DataMgmtPolicy::deserialize(int * buffer) {
    m_lazyAllocation = static_cast<bool>(*(buffer++));
    m_lazyAllocationDefined = static_cast<bool>(*(buffer++));
    m_saveData = static_cast<bool>(*(buffer++));
    m_loadData = static_cast<bool>(*(buffer++));
    m_multiBufferSize = static_cast<unsigned int>(*(buffer++));
    m_multiBufferSizeDefined = static_cast<bool>(*(buffer++));
    
    return buffer;
  }

  // \brief Assignment operator
  const DataMgmtPolicy & 
  DataMgmtPolicy::operator=(const DataMgmtPolicy & other) {
    m_lazyAllocation = other.m_lazyAllocation;
    m_saveData = other.m_saveData;
    m_loadData = other.m_loadData;
    m_multiBufferSize = other.m_multiBufferSize;
    return *this;
  }
  
  // \brief Equality operator
  bool DataMgmtPolicy::operator==(const DataMgmtPolicy & other) const {
    return (m_lazyAllocation == other.m_lazyAllocation
	    && m_saveData == other.m_saveData
	    && m_loadData == other.m_loadData
	    && m_multiBufferSize == other.m_multiBufferSize);
  }
  
} // namespace ipvtol


#endif // PVTOL_DATAMGMTPOLICY_CC_ not defined
