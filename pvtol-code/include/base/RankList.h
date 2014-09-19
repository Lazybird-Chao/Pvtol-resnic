/**
 *  File: RankList.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    RankList.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the RankList class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: RankList.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_RANKLIST_H_
#define PVTOL_RANKLIST_H_


#include <PvtolBasics.h> //must be first (for now...)
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

namespace ipvtol
{

  /// \brief class RankList
  /// The RankList class provides a list of ranks to be used by the Map or 
  /// related classes.
  class RankList {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief Default Constructor
    /// This constructor is used to build an incomplete RankList.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    RankList(); 

    /// \brief Constructor
    /// Construct a RankList, given an STL vector of ranks.
    RankList(const std::vector<RankId> &);
    
    /// \brief Constructor
    /// Construct a RankList, a number of ranks.  A RankList containing 
    /// 0 to size-1 will be constructed.
    RankList(int size);
    
    /// \brief Constructor
    /// Construct a RankList, given a size and C-style array of RankIds.
    RankList(int size, const RankId * ranks);
    
    /// \brief Copy constructor
    /// Create a RankList, given another RankList.
    RankList(const RankList &);  
    
    /// \brief Destructor
    ~RankList();
    
    /// \brief getElement
    /// Return the rank at the requested position.
    RankId getRank(int) const;
    
    /// \brief getNumRanks
    /// Return the size of the RankList.
    int getNumRanks() const;
    
    /// \brief getRankListVector
    /// Return the rank at the requested position.
    void getRankListVector(std::vector<RankId> &) const;
    
    /// \brief hasRank
    /// Return a boolean indicating if the supplied rank is held in 
    /// the RankList.
    bool hasRank(int rank) const;
    
    /// \brief setRank
    /// Set a rank value in the RankList.
    void setRank(unsigned int index, unsigned int value);
    
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

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

    /// \brief Assignment operator
    const RankList & operator=(const RankList & other);
    
    /// \brief Equality operator
    bool operator==(const RankList & other) const;
    
    /// \brief Inequality operator
    bool operator!=(const RankList & other) const;
    
  private:
    
    /// \brief m_ranks
    /// Container for the ranks.
    std::vector<RankId> m_ranks;
    
  }; // class RankList
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Construct a RankList, given an STL vector of ranks.
  inline
  RankList::RankList(const std::vector<RankId> & ranks)
    : m_ranks(ranks)
  {
  }
  
  // \brief Copy constructor
  // Create a RankList, given another RankList.
  inline
  RankList::RankList(const RankList & other)
    : m_ranks(other.m_ranks)
  {
  }
  
  // \brief getElement
  // Return the rank at the requested position.
  inline
  RankId RankList::getRank(int index) const {
    return m_ranks[index];
  }
  
  // \brief getNumRanks
  // Return the size of the RankList.
  inline
  int RankList::getNumRanks() const {
    return m_ranks.size();
  }
  
  // \brief getRankListVector
  // Return the rank at the requested position.
  inline
  void RankList::getRankListVector(std::vector<RankId> & rankListCopy) const {
    rankListCopy = m_ranks;
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int RankList::getSerializedSize() const {
    // Return the number of ranks + 1 buffer space to store the number of 
    // ranks.
    return m_ranks.size() + 1;
  }

  /// \brief serialize
  /// This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void RankList::serialize(Archive & ar, const unsigned int version) {
    ar & m_ranks;
  }

  // \brief Assignment operator
  inline
  const RankList & RankList::operator=(const RankList & other) {
    m_ranks = other.m_ranks;
    return (*this);
  }
  
  // \brief Equality operator
  inline
  bool RankList::operator==(const RankList & other) const {
    return (m_ranks == other.m_ranks);
  }
  
  // \brief Inequality operator
  inline
  bool RankList::operator!=(const RankList & other) const {
    return (m_ranks != other.m_ranks);
  }
  
  // \brief Destructor
  inline
  RankList::~RankList() {
  }

} // namespace ipvtol


#endif // PVTOL_RANKLIST_H_ not defined
