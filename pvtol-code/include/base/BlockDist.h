/**
 *  File: BlockDist.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    BlockDist.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the BlockDist class.
 *           
 *  Author: Ryan Haney
 *
 *  $Id: BlockDist.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_BLOCKDIST_H_
#define PVTOL_BLOCKDIST_H_


namespace ipvtol
{
  /// \brief class DataDistBase
  /// The BlockDist class inherits from the DataDistBase class.  It 
  /// specifies a data distribution for a single dimension.  The data 
  /// is evenly divided over the specified processes/ranks, in order.
  class BlockDist : public DataDistBase {
    
  public:
    
    /// \brief Constructor
    BlockDist(unsigned int overlap = 0);
    
    /// \brief Copy Constructor
    BlockDist(const BlockDist &);
    
    /// \brief Destructor
    virtual ~BlockDist();
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual BlockDist * clone() const;
    
  }; // class DataDistBase


  //------------------
  // INLINE FUNCTIONS
  //------------------    


  // \brief Constructor
  inline
  BlockDist::BlockDist(unsigned int overlap) 
    : DataDistBase(DataDistBase::BLOCK, 0, overlap)
  {
  }
  
  // \brief Copy Constructor
  inline
  BlockDist::BlockDist(const BlockDist & other)
    : DataDistBase(other)
  {
  }
  
  // \brief Destructor
  inline
  BlockDist::~BlockDist()
  {
  }
  
  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  inline
  BlockDist * BlockDist::clone() const {
    return (new BlockDist(*this));
  }

} // namespace ipvtol


#endif // PVTOL_BLOCKDIST_H_ not defined

