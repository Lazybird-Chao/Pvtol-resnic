/**
 *  File: TemporalMapBase.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TemporalMapBase.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the TemporalMapBase abstract base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TemporalMapBase.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TEMPORALMAPBASE_CC_
#define PVTOL_TEMPORALMAPBASE_CC_


#include <PvtolBasics.h>
#include <vector>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>
#include <DataMap.h>
#include <TemporalMapBase.h>
#include <TemporalMap.h>
#include <AutoTemporalMap.h>

namespace ipvtol
{    
  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * TemporalMapBase::serialize(int * buffer) const {
    buffer = m_temporalBlockingInfo.serialize(buffer);
    buffer = m_dataMgmtPolicy.serialize(buffer);
    *(buffer++) = static_cast<int>(m_hasChildTemporalMap);
    if(m_hasChildTemporalMap) {
      *(buffer++) 
	= static_cast<int>(m_childTemporalMapPtr->getTemporalMapBaseType());
      buffer = m_childTemporalMapPtr->serialize(buffer);
    }

    return buffer;
  }
    
  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available 
  // storage location.
  int * TemporalMapBase::deserialize(int * buffer) {

    buffer = m_temporalBlockingInfo.deserialize(buffer);
    buffer = m_dataMgmtPolicy.deserialize(buffer);
    m_hasChildTemporalMap = static_cast<bool>(*buffer++);
    
    if(m_hasChildTemporalMap) {
      switch(static_cast<TemporalMapBase::TemporalMapBaseType>(*buffer++)) {
      case TemporalMapBase::AUTO_MAP:
	m_childTemporalMapPtr = new AutoTemporalMap;
	break;
      case TemporalMapBase::RUNTIME_MAP:
	m_childTemporalMapPtr = new TemporalMap;
	break;
      default:
	throw Exception("deserialize: Invalid TemporalMapBaseType.",
			__FILE__, __LINE__);
      }
      buffer = m_childTemporalMapPtr->deserialize(buffer);
    }
      
    return buffer;
  }

  // \brief Assignment operator
  const TemporalMapBase & 
  TemporalMapBase::operator=(const TemporalMapBase & other) {
    if(*this != other) {
      if(m_temporalMapType == other.m_temporalMapType) {
	freeTemporalMapBaseMemory();
	m_temporalBlockingInfo = other.m_temporalBlockingInfo;
	m_dataMgmtPolicy = other.m_dataMgmtPolicy;
	if(other.m_childTemporalMapPtr != NULL) {
	  m_childTemporalMapPtr = other.m_childTemporalMapPtr->clone();
	}
      }
      else {
	throw 
	  Exception("TemporalMapBase assignment operator: Types do not match.",
		    __FILE__, __LINE__);
      }
    }
    return *this;
  }
  
  // \brief Equality operator
  bool TemporalMapBase::operator==(const TemporalMapBase & other) const {
    if(m_temporalMapType == other.m_temporalMapType 
       && m_temporalBlockingInfo == other.m_temporalBlockingInfo
       && m_dataMgmtPolicy == other.m_dataMgmtPolicy) {
      if(m_childTemporalMapPtr == NULL 
	 || other.m_childTemporalMapPtr == NULL) {
	if(m_childTemporalMapPtr == other.m_childTemporalMapPtr) {
	  return true;
	}
      }
      else {
	if(*m_childTemporalMapPtr == *(other.m_childTemporalMapPtr)) {
	  return true;
	}
      }
    }
    return false;
  }

  // \brief freeTemporalMapBaseMemory
  // Free all allocated TemporalMapBase memory.
  void TemporalMapBase::freeTemporalMapBaseMemory() {
    if(m_childTemporalMapPtr != NULL) {
      delete m_childTemporalMapPtr;
      m_childTemporalMapPtr = NULL;
    }
  }
  
} // namespace ipvtol


#endif // PVTOL_TEMPORALMAPBASE_CC_ not defined
