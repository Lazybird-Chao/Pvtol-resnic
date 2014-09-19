/**
 *  File: DistDescription.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DistDescription.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the DistDescription abstract 
 *           base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DistDescription.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DISTDESCRIPTION_H_
#define PVTOL_DISTDESCRIPTION_H_


#include <Exception.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

namespace ipvtol
{

  /// \brief class DistDescription
  /// The DistDescription class is used as an abstract base for 
  /// the DataDistDescription, FunctorDistDescription, and 
  /// TaskDistDescription child classes.
  class DistDescription {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief DistDescriptionType
    /// Enumeration to give more information as to what the Distribution 
    /// Description is applied to.
    enum DistDescriptionType {
      DATA_DIST_DESCRIPTION,
      TASK_DIST_DESCRIPTION,
      FUNCTOR_DIST_DESCRIPTION 
    };
    
    /// \brief Default Constructor
    /// This constructor is used to build an incomplete DistDescription.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    DistDescription(); 

    /// \brief Constructor
    /// Construct a DistDescription, given a DistDescriptionType.
    DistDescription(const DistDescriptionType &);
    
    /// \brief Copy contructor
    DistDescription(const DistDescription &);
    
    /// \brief Destructor
    virtual ~DistDescription() = 0;
    
    /// \brief getDistDescriptionType
    /// Return the DistDescription's type.
    DistDescriptionType getDistDescriptionType() const;
    
    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual DistDescription * clone() const = 0;
    
    /// \brief getSerializedSize
    /// Return size of the serialized object.
    virtual int getSerializedSize() const = 0;
    
    /// \brief serialize
    /// Store all object information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    virtual int * serialize(int * buffer) const = 0;
    
    /// \brief deserialize
    /// Initialize the object with the information supplied within
    /// the buffer.  Return the buffer, incremented to the next available 
    /// storage location.
    virtual int * deserialize(int * buffer) = 0;

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

    /// \brief Assignment operator
    virtual const DistDescription & operator=(const DistDescription & other);
    
    /// \brief Equality operator
    virtual bool operator==(const DistDescription & other) const;
    
    /// \brief Inequality operator
    virtual bool operator!=(const DistDescription & other) const;

  protected:
    
    DistDescriptionType m_distDescriptionType;
    
  }; // class DistDescription
  
  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Construct a DistDescription, given a DistDescriptionType.
  inline
  DistDescription::DistDescription(const DistDescriptionType & distType)
    : m_distDescriptionType(distType)
  {
  }
  
  // \brief Copy contructor
  inline
  DistDescription::DistDescription(const DistDescription & other)
    : m_distDescriptionType(other.m_distDescriptionType)
  {
  }
  
  // \brief Destructor
  inline
  DistDescription::~DistDescription() {
  }
  
  // \brief getDistDescriptionType
  // Return the DistDescription's type.
  inline DistDescription::DistDescriptionType 
  DistDescription::getDistDescriptionType() const {
    return m_distDescriptionType;
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void DistDescription::serialize(Archive & ar, 
				  const unsigned int version) {
    ar & m_distDescriptionType;
  }
  
  // \brief Assignment operator
  inline
  const DistDescription & 
  DistDescription::operator=(const DistDescription & other) {
    
    if(m_distDescriptionType != other.m_distDescriptionType) {
      throw Exception("DistDesc assignment operator: Types do not match.",
		      __FILE__, __LINE__);
    }
    
    return (*this);
  }
  
  // \brief Equality operator
  inline
  bool DistDescription::operator==(const DistDescription & other) const {
    return (m_distDescriptionType == other.m_distDescriptionType);
  }
  
  // \brief Inequality operator
  inline
  bool DistDescription::operator!=(const DistDescription & other) const {
    return (m_distDescriptionType != other.m_distDescriptionType);
  }

} // namespace ipvtol


#endif // PVTOL_DISTDESCRIPTION_H_ not defined

