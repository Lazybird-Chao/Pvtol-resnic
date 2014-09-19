/**
 *  File: BlockCyclicDist.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    BlockCyclicDist.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the BlockCyclicDist class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: BlockCyclicDist.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_BLOCKCYCLICDIST_H_
#define PVTOL_BLOCKCYCLICDIST_H_


namespace ipvtol
{

  /// \brief class DataDistBase
  /// The BlockCyclicDist class inherits from the DataDistBase class.  
  /// It specifies a data distribution for a single dimension.  The 
  /// data is divided over the specified processes or ranks.  It is 
  /// done so in blockSize blocks of data, round-robin over the ranks 
  /// until all data is distributed.
  class BlockCyclicDist : public DataDistBase {
    
  public:
    
    /// \brief Constructor
    BlockCyclicDist(unsigned int blockSize, unsigned int overlap = 0);
    
    /// \brief Copy Constructor
    BlockCyclicDist(const BlockCyclicDist &);
    
    /// \brief Destructor
    virtual ~BlockCyclicDist();
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual BlockCyclicDist * clone() const;
    
  }; // class BlockCyclicDist
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------    


  // \brief Constructor
  inline
  BlockCyclicDist::BlockCyclicDist(unsigned int blockSize, 
				   unsigned int overlap) 
    : DataDistBase(DataDistBase::BLOCK_CYCLIC, blockSize, overlap)
  {
  }
  
  // \brief Copy Constructor
  inline
  BlockCyclicDist::BlockCyclicDist(const BlockCyclicDist & other)
    : DataDistBase(other)
  {
  }

  // \brief Destructor
  inline
  BlockCyclicDist::~BlockCyclicDist()
  {
  }
  
  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  inline
  BlockCyclicDist * BlockCyclicDist::clone() const {
    return (new BlockCyclicDist(*this));
  }

} // namespace ipvtol


#endif // PVTOL_BLOCKCYCLICDIST_H_ not defined

