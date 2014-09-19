/**
 *  File: CyclicDist.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    CyclicDist.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the CyclicDist class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: CyclicDist.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_CYCLICDIST_H_
#define PVTOL_CYCLICDIST_H_


namespace ipvtol
{

  /// \brief class DataDistBase
  /// The CyclicDist class inherits from the DataDistBase class.  It 
  /// specifies a data distribution for a single dimension.  It 
  /// defines a Block-cyclic distribution where the blockSize = 1.
  class CyclicDist : public DataDistBase {
    
  public:
    
    /// \brief Constructor
    CyclicDist(unsigned int overlap = 0);
    
    /// \brief Copy Constructor
    CyclicDist(const CyclicDist &);
    
    /// \brief Destructor
    virtual ~CyclicDist();
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual CyclicDist * clone() const;
    
  }; // class DataDistBase
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------    


  // \brief Constructor
  inline
  CyclicDist::CyclicDist(unsigned int overlap) 
    : DataDistBase(DataDistBase::CYCLIC, 0, overlap)
  {
  }
  
  // \brief Copy Constructor
  inline
  CyclicDist::CyclicDist(const CyclicDist & other)
    : DataDistBase(other)
  {
  }
  
  // \brief Destructor
  inline
  CyclicDist::~CyclicDist()
  {
  }
  
  // \brief clone
  // Create and return a copy of the object.  It is the user's responsibility
  // to delete the returned object.
  inline
  CyclicDist * CyclicDist::clone() const {
    return (new CyclicDist(*this));
  }

} // namespace ipvtol


#endif // PVTOL_CYCLICDIST_H_ not defined

