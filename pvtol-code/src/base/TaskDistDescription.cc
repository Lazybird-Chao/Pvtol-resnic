 /**
 *  File: TaskDistDescription.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TaskDistDescription.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the TaskDistDescription class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TaskDistDescription.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TASKDISTDESCRIPTION_CC_
#define PVTOL_TASKDISTDESCRIPTION_CC_


#include <TaskDistDescription.h>

namespace ipvtol
{

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * TaskDistDescription::serialize(int * buffer) const {
    *(buffer++) = static_cast<int>(m_distType);
    *(buffer++) = m_numReplicas;
    *(buffer++) = m_replicaSize;
    *(buffer++) = m_overlap;

    return buffer;
  }
  
  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available 
  // storage location.
  int * TaskDistDescription::deserialize(int * buffer) {
    m_distType = static_cast<TaskDistType>(*(buffer++));
    m_numReplicas = *(buffer++);
    m_replicaSize = *(buffer++);
    m_overlap = *(buffer++);
    
    return buffer;
  }
    
  // \brief Assignment operator
  const TaskDistDescription & 
  TaskDistDescription::operator=(const TaskDistDescription & other) {
    
    if(m_distDescriptionType != other.m_distDescriptionType) {
      throw Exception("DistDesc assignment operator: Types do not match.",
		      __FILE__, __LINE__);
    }
    else {
      m_distType = other.m_distType;
      m_numReplicas = other.m_numReplicas;
      m_replicaSize = other.m_replicaSize;
      m_overlap = other.m_replicaSize;
    }
    
    return (*this);
  }
  
  // \brief Equality operator
  bool TaskDistDescription::operator==(const TaskDistDescription & 
				       other) const {
    if(m_distDescriptionType == other.m_distDescriptionType) {
      if(m_distType == other.m_distType
       && m_numReplicas == other.m_numReplicas
	 && m_replicaSize == other.m_replicaSize 
	 && m_overlap == other.m_overlap) {
	return true;
      }
    }
    return false;
  }

} // namespace ipvtol


#endif // PVTOL_TASKDISTDESCRIPTION_CC_ not defined

