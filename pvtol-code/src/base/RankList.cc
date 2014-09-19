/**
 *  File: RankList.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    RankList.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Inline methods for the RankList class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: RankList.cc 938 2009-02-18 17:39:52Z ka21088 $
 */


#ifndef PVTOL_RANKLIST_CC_
#define PVTOL_RANKLIST_CC_


#include <RankList.h>
#include <Exception.h>
#include <vector>

namespace ipvtol
{

  /// \brief Default Constructor
  /// More initialization needs to be done before an object created with
  /// this constructor can be used.
  RankList::RankList()
  {
  }

  // \brief Constructor
  // Construct a RankList, a number of ranks.  A RankList containing
  // 0 to size-1 will be constructed.
  RankList::RankList(int size) {
    for(int i = 0; i < size; i++) {
      m_ranks.push_back(static_cast<RankId>(i));
    }
  }

  // \brief Constructor
  // Construct a RankList, given a size and C-style array of RankIds.
  RankList::RankList(int size, const RankId * ranks) {
    RankId * tmpPtr = const_cast<RankId *>(ranks);
    for(int i = 0; i < size; i++) {
      m_ranks.push_back(*(tmpPtr++));
    }
  }

  // \brief hasRank
  // Return a boolean indicating if the supplied rank is held in
  // the RankList.
  bool RankList::hasRank(int rank) const {
    for(unsigned int i = 0; i < m_ranks.size(); i++) {
      if(m_ranks[i] == rank) {
	return true;
      }
    }
    return false;
  }

  // \brief setRank
  // Set a rank value in the RankList.
  void RankList::setRank(unsigned int index, unsigned int value) {
    if(index < m_ranks.size()) {
      m_ranks[index] = value;
    }
    else {
      throw Exception("RankList::setRank: Invalid index supplied.",
		      __FILE__, __LINE__);
    }
  }

  // \brief serialize
  // Store all object information within the supplied integer
  // buffer.  Return the buffer, incremented to the next available storage
  // location.
  int * RankList::serialize(int * buffer) const {
    // Store the size of the RankList, along with all ranks.
    *(buffer++) = m_ranks.size();
    for(unsigned int i = 0; i < m_ranks.size(); i++) {
      *(buffer++) = static_cast<int>(m_ranks[i]);
    }
    return buffer;
  }

  // \brief deserialize
  // Initialize the object with the information supplied within
  // the buffer.  Return the buffer, incremented to the next available
  // storage location.
  int * RankList::deserialize(int * buffer) {
    m_ranks.clear();
    int numRanks = *(buffer++);
    for(int i = 0; i < numRanks; i++) {
      m_ranks.push_back(static_cast<RankId>(*(buffer++)));
    }
    return buffer;
  }


} // namespace ipvtol


#endif // PVTOL_RANKLIST_CC_ not defined
