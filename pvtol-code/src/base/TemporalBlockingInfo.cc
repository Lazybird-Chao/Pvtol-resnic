/**
 *  File: TemporalBlockingInfo.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TemporalBlockingInfo.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the TemporalBlockingInfo class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TemporalBlockingInfo.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TEMPORALBLOCKINGINFO_CC_
#define PVTOL_TEMPORALBLOCKINGINFO_CC_


#include <TemporalBlockingInfo.h>
#include <vector>

namespace ipvtol
{

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * TemporalBlockingInfo::serialize(int * buffer) const {
    *(buffer++) = m_lengths.size();
    for(unsigned int i = 0; i < m_lengths.size(); i++) {
      *(buffer++) = static_cast<int>(m_lengths[i]);
    }
    for(unsigned int i = 0; i < m_overlaps.size(); i++) {
      *(buffer++) = static_cast<int>(m_overlaps[i]);
    }
    return buffer;
  }

  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available
  // storage location.
  int * TemporalBlockingInfo::deserialize(int * buffer) {
    m_lengths.clear();
    m_overlaps.clear();
    int numDims = *(buffer++);
    for(int i = 0; i < numDims; i++) {
      m_lengths.push_back(static_cast<unsigned int>(*(buffer++)));
    }
    for(int i = 0; i < numDims; i++) {
      m_overlaps.push_back(static_cast<unsigned int>(*(buffer++)));
    }
    return buffer;
  }

  // \brief Assignment operator
  const TemporalBlockingInfo &
  TemporalBlockingInfo::operator=(const TemporalBlockingInfo & rhs) {
    m_lengths.clear();
    m_overlaps.clear();
    for(unsigned int i = 0; i < rhs.m_lengths.size(); i++) {
      m_lengths.push_back(rhs.m_lengths[i]);
      m_overlaps.push_back(rhs.m_overlaps[i]);
    }
    return (*this);
  }

  // \brief Equality operator
  bool
  TemporalBlockingInfo::operator==(const TemporalBlockingInfo & rhs) const {
    for(unsigned int i = 0; i < m_lengths.size(); i++) {
      if(m_lengths[i] != rhs.m_lengths[i]
	 || m_overlaps[i] != rhs.m_overlaps[i]) {
	return false;
      }
    }
    return true;
  }

}


#endif // PVTOL_TEMPORALBLOCKINGINFO_CC
