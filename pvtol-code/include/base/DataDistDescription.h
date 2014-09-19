/**
 *  File: DataDistDescription.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DataDistDescription.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the DataDistDescription class.
 *           
 *  Author: Ryan Haney
 *
 *  $Id: DataDistDescription.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATADISTDESCRIPTION_H_
#define PVTOL_DATADISTDESCRIPTION_H_



#include <DistDescription.h>
#include <DataDistBase.h>
#include <vector>

namespace ipvtol
{

  /// \brief class DataDistDescription
  /// The DataDistDescription class is used to describe how
  /// data is distributed for each dimension.  Each dimension must
  /// be specified when constructing the object.  The choices are:
  ///    - Block: The data is evenly divided over the specified
  ///             processes/ranks, in order.
  ///    - Block-cyclic: Data is divided over the specified processes
  ///             or ranks.  It is done so in blockSize blocks of data, 
  ///             round-robin over the ranks until all data is distributed.
  ///    - Cyclic: Defines a Block-cyclic distribution where the 
  ///             blockSize = 1.
  ///    - Replicated: A full copy of the data (in one dimension) is 
  ///             replicated over each process/rank.
  ///    - Mappable: The distribution type and sizes are left up to
  ///             the automatic mapping utility.  This option can only
  ///             be used in AutoMaps.
  class DataDistDescription : public DistDescription {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief Default Constructor
    /// This constructor is used to build an incomplete DataDistDescription.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    DataDistDescription(); 

    /// \brief Constructor
    /// Construct a DataDistDescription, given a DataDistDescriptionType.
    DataDistDescription(const DataDistBase &);
    
    /// \brief Constructor
    /// Construct a DataDistDescription, given a DataDistDescriptionType.
    DataDistDescription(const DataDistBase &, const DataDistBase &);
    
    /// \brief Constructor
    /// Construct a DataDistDescription, given a DataDistDescriptionType.
    DataDistDescription(const DataDistBase &, const DataDistBase &,
			const DataDistBase &);
    
    /// \brief Copy contructor
    DataDistDescription(const DataDistDescription &);
    
    /// \brief Destructor
    virtual ~DataDistDescription();
    
    /// \brief getNumDimensions
    /// Return the number of data dimensions that the distDescription
    /// describes.
    unsigned int getNumDimensions() const;
    
    /// \brief getDataDist
    /// Return the DataDist at the specified dimension.  Returns a DataDistBase
    /// reference that could be a Block, BlockCyclic, Cyclic, or Replicated 
    /// object.
    const DataDistBase & getDataDist(unsigned int dimension) const;
    
    /// \brief getDataDistType
    /// Return the DataDistType at the specified dimension.  I.e. Block, 
    /// cyclic, etc.
    DataDistBase::DataDistType getDataDistType(unsigned int dimension) const;
    
    /// \brief getBlockSize
    unsigned int getBlockSize(unsigned int dimension) const;
    
    /// \brief getOverlapSize
    unsigned int getOverlapSize(unsigned int dimension) const;
    
    /// \brief
    /// getSerializedSize
    /// Return size of the serialized DataDistDescription object.
    virtual int getSerializedSize() const;
    
    /// \brief serialize
    /// Store all DataDistDescription information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    virtual int * serialize(int * buffer) const;
    
    /// \brief deserialize
    /// Initialize the DataDistDescription with the information supplied within
    /// the buffer.  Return the buffer, incremented to the next available 
    /// storage location.
    virtual int * deserialize(int *);

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

    /// \brief clone
    /// Create and return a copy of the object.  It is the user's 
    /// responsibility to delete the returned object.
    virtual DataDistDescription * clone() const;
    
    /// \brief Assignment operator
    virtual const DataDistDescription & operator=(const DataDistDescription & 
						  other);
    
    /// \brief Equality operator
    virtual bool operator==(const DataDistDescription & other) const;
    
    /// \brief Inequality operator
    virtual bool operator!=(const DataDistDescription & other) const;
    
  private:
    
    unsigned int                m_numDimensions;
    std::vector<DataDistBase *> m_dataDistBases;
    
    /// \brief freeDataDistDescriptionMemory
    void freeDataDistDescriptionMemory();
    
    /// \brief copyDataDistDescription
    void copyDataDistDescription(const DataDistDescription &);
    
  }; // class DataDistDescription


  //------------------
  // INLINE FUNCTIONS
  //------------------
  
  
  // \brief getNumDimensions
  // Return the number of data dimensions that the distDescription
  // describes.
  inline
  unsigned int DataDistDescription::getNumDimensions() const {
    return m_numDimensions;
  }

  // \brief
  // getSerializedSize
  // Return size of the serialized DataDistDescription object.
  inline
  int DataDistDescription::getSerializedSize() const {
    // Return the size of the DataDistBase objects multiplied by the number
    // of dimensions + 1 to hold the number of dimensions.
    return (m_numDimensions * m_dataDistBases[0]->getSerializedSize() + 1);
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void DataDistDescription::serialize(Archive & ar, 
				      const unsigned int version) {
    /// This line tells Boost::serialize that it also needs to serialize
    /// the base class
    ar & boost::serialization::base_object<DistDescription>(*this);
    ar & m_numDimensions;
    ar & m_dataDistBases;
  }

} // namespace ipvtol


#endif // PVTOL_DATADISTDESCRIPTION_H_ not defined

