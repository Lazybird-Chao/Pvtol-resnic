/**
 *  File: DataDistBase.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DataDistBase.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the DataDistBase abstract base class.

 *  Author: Ryan Haney
 *
 *  $Id: DataDistBase.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATADISTBASE_CC_
#define PVTOL_DATADISTBASE_CC_


#include <DataDistBase.h>
#include <Exception.h>

namespace ipvtol
{

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * DataDistBase::serialize(int * buffer) const {
    *(buffer++) = static_cast<int>(m_dataDistType);
    *(buffer++) = m_blockSize;
    *(buffer++) = m_overlapSize;

    return buffer;
  }

  // \brief Assignment operator
  const DataDistBase & DataDistBase::operator=(const DataDistBase & other) {
    if(*this != other) {
      if(m_dataDistType == other.m_dataDistType) {
	m_blockSize = other.m_blockSize;
	m_overlapSize = other.m_overlapSize;
      }
      else {
	throw 
	  Exception("DataDistBase assignment operator: Types do not match.",
		    __FILE__, __LINE__);
      }    
    }
    return (*this);
  }
  
  // \brief Equality operator
  bool DataDistBase::operator==(const DataDistBase & other) const {
    return (m_dataDistType == other.m_dataDistType
	    && m_blockSize == other.m_blockSize
	    && m_overlapSize == other.m_overlapSize);
  }
  
} // namespace ipvtol


#endif // PVTOL_DATADISTBASE_CC_ not defined

