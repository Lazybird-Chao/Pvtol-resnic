/**
 *  File: ReplicatedDist.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    ReplicatedDist.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the ReplicatedDist class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: ReplicatedDist.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_REPLICATEDDIST_H_
#define PVTOL_REPLICATEDDIST_H_


namespace ipvtol
{

  /// \brief class DataDistBase
  /// The ReplicatedDist class inherits from the DataDistBase class.  
  /// It specifies a data distribution for a single dimension.  A full 
  /// copy of the data (in one dimension) is replicated over each 
  /// process/rank.
  class ReplicatedDist : public DataDistBase {
    
  public:
    
    /// \brief Constructor
    ReplicatedDist();
    
    /// \brief Copy Constructor
    ReplicatedDist(const ReplicatedDist &);
    
    /// \brief Destructor
    virtual ~ReplicatedDist();
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual ReplicatedDist * clone() const;
    
  }; // class ReplicatedDist
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  inline
  ReplicatedDist::ReplicatedDist() 
    : DataDistBase(DataDistBase::REPLICATED, 0, 0)
  {
  }
  
  // \brief Copy Constructor
  inline
  ReplicatedDist::ReplicatedDist(const ReplicatedDist & other)
    : DataDistBase(other)
  {
  }
  
  // \brief Destructor
  inline
  ReplicatedDist::~ReplicatedDist()
  {
  }
  
  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  inline
  ReplicatedDist * ReplicatedDist::clone() const {
    return (new ReplicatedDist(*this));
  }

} // namespace ipvtol


#endif // PVTOL_REPLICATEDDIST_H_ not defined

