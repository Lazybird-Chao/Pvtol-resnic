/**
 *  File: DataDistDescription.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods for the DataDistDescription class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataDistDescription.cc 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATADISTDESCRIPTION_CC_
#define PVTOL_DATADISTDESCRIPTION_CC_


#include <DataDistDescription.h>
#include <DistDescription.h>
#include <DataDistBase.h>
#include <BlockDist.h>
#include <BlockCyclicDist.h>
#include <CyclicDist.h>
#include <ReplicatedDist.h>

namespace ipvtol
{

  /// \brief Default Constructor
  /// More initialization needs to be done before an object created with
  /// this constructor can be used.
  DataDistDescription::DataDistDescription()
    : DistDescription(DistDescription::DATA_DIST_DESCRIPTION)
  {
  }

  // \brief Constructor
  // Construct a DataDistDescription, given a DataDistDescriptionType.
  DataDistDescription::DataDistDescription(const DataDistBase & distBase)
    : DistDescription(DistDescription::DATA_DIST_DESCRIPTION),
      m_numDimensions(1)
  {
    m_dataDistBases.push_back(distBase.clone());
  }

  // \brief Constructor
  // Construct a DataDistDescription, given a DataDistDescriptionType.
  DataDistDescription::DataDistDescription(const DataDistBase & distBase0,
					   const DataDistBase & distBase1)
    : DistDescription(DistDescription::DATA_DIST_DESCRIPTION),
      m_numDimensions(2)
  {
    m_dataDistBases.push_back(distBase0.clone());
    m_dataDistBases.push_back(distBase1.clone());
  }

  // \brief Constructor
  // Construct a DataDistDescription, given a DataDistDescriptionType.
  DataDistDescription::DataDistDescription(const DataDistBase & distBase0,
					   const DataDistBase & distBase1,
					   const DataDistBase & distBase2)
    : DistDescription(DistDescription::DATA_DIST_DESCRIPTION),
      m_numDimensions(3)
  {
    m_dataDistBases.push_back(distBase0.clone());
    m_dataDistBases.push_back(distBase1.clone());
    m_dataDistBases.push_back(distBase2.clone());
  }

  // \brief Copy contructor
  DataDistDescription::DataDistDescription(const DataDistDescription & other)
    : DistDescription(other),
      m_numDimensions(other.m_numDimensions)
  {
    copyDataDistDescription(other);
  }

  // \brief Destructor
  DataDistDescription::~DataDistDescription() {
    freeDataDistDescriptionMemory();
  }

  // \brief getDataDist
  // Return the DataDist at the specified dimension.  Returns a DataDistBase
  // reference that could be a Block, BlockCyclic, Cyclic, or Replicated
  // object.
  const DataDistBase &
  DataDistDescription::getDataDist(unsigned int dimension) const {
    if(dimension < m_numDimensions) {
      return *m_dataDistBases[dimension];
    }
    else {
      throw Exception("getDataDist: Invalid dimension.",
		      __FILE__, __LINE__);
    }
  }

  // \brief getDataDistType
  // Return the DataDistType at the specified dimension.  I.e. Block,
  // cyclic, etc.
  DataDistBase::DataDistType
  DataDistDescription::getDataDistType(unsigned int dimension) const {
    if(dimension < m_numDimensions) {
      return m_dataDistBases[dimension]->getDataDistType();
    }
    else {
      throw Exception("getDataDistType: Invalid dimension.",
		      __FILE__, __LINE__);
    }
  }

  // \brief getBlockSize
  unsigned int
  DataDistDescription::getBlockSize(unsigned int dimension) const {
    if(dimension < m_numDimensions) {
      return m_dataDistBases[dimension]->getBlockSize();
    }
    else {
      throw Exception("getBlockSize: Invalid dimension.",
		      __FILE__, __LINE__);
    }
  }

  // \brief getOverlapSize
  unsigned int
  DataDistDescription::getOverlapSize(unsigned int dimension) const {
    if(dimension < m_numDimensions) {
      return m_dataDistBases[dimension]->getOverlapSize();
    }
    else {
      throw Exception("getOverlapSize: Invalid dimension.",
		      __FILE__, __LINE__);
    }
  }

  // \brief serialize
  // Store all DataDistDescription information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * DataDistDescription::serialize(int * buffer) const {
    *(buffer++) = m_numDimensions;
    for(unsigned int i = 0; i < m_numDimensions; i++) {
      buffer = m_dataDistBases[i]->serialize(buffer);
    }
    return buffer;
  }

  // \brief deserialize
  // Initialize the DataDistDescription with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available
  // storage location.
  int * DataDistDescription::deserialize(int * buffer) {
    m_numDimensions = *(buffer++);
    for(unsigned int i = 0; i < m_numDimensions; i++) {
      switch(static_cast<DataDistBase::DataDistType>(*buffer++)) {
      case DataDistBase::BLOCK:
	m_dataDistBases.push_back(new BlockDist(*(buffer+1)));
	break;
      case DataDistBase::BLOCK_CYCLIC:
	m_dataDistBases.push_back(new BlockCyclicDist(*buffer, *(buffer+1)));
	break;
      case DataDistBase::CYCLIC:
	m_dataDistBases.push_back(new CyclicDist(*(buffer+1)));
	break;
      case DataDistBase::REPLICATED:
	m_dataDistBases.push_back(new ReplicatedDist);
	break;
      default:
	throw Exception("deserialize: Invalid DataDistBase::DataDistType.",
			__FILE__, __LINE__);
      }
      // Increment the buffer.
      buffer += 2;
    }

    return buffer;
  }

  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  DataDistDescription * DataDistDescription::clone() const {
    return (new DataDistDescription(*this));
  }

  // \brief Assignment operator
  const DataDistDescription &
  DataDistDescription::operator=(const DataDistDescription & other) {
    if(m_distDescriptionType != other.m_distDescriptionType) {
      throw Exception("DistDesc assignment operator: Types do not match.",
		      __FILE__, __LINE__);
    }
    else {
      freeDataDistDescriptionMemory();
      m_numDimensions = m_numDimensions;
      copyDataDistDescription(other);
    }

    return (*this);
  }

  // \brief Equality operator
  bool
  DataDistDescription::operator==(const DataDistDescription & other) const {
    if(m_distDescriptionType == other.m_distDescriptionType
       && m_numDimensions == other.m_numDimensions) {
      for(unsigned int i = 0; i < other.m_dataDistBases.size(); i++) {
	if(*m_dataDistBases[i] != *other.m_dataDistBases[i]) {
	  return false;
	}
      }
    }
    else {
      return false;
    }

    return true;
  }

  // \brief Inequality operator
  bool
  DataDistDescription::operator!=(const DataDistDescription & other) const {
    return !(*this == other);
  }

  // \brief freeDataDistDescriptionMemory()
  void DataDistDescription::freeDataDistDescriptionMemory() {
    std::vector<DataDistBase *>::iterator it;
    for(it = m_dataDistBases.begin(); it != m_dataDistBases.end(); it++) {
      if(*it != NULL) {
	delete (*it);
	*it = NULL;
      }
    }
    m_dataDistBases.clear();
  }

  // \brief copyDataDistDescription()
  void
  DataDistDescription::copyDataDistDescription(const DataDistDescription &
					       other) {
    for(unsigned int i = 0; i < other.m_dataDistBases.size(); i++) {
      m_dataDistBases.push_back(other.m_dataDistBases[i]->clone());
    }
  }

} // namespace ipvtol


#endif // PVTOL_DATADISTDESCRIPTION_CC_ not defined
