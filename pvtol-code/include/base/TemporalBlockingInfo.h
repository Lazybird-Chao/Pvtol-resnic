/**
 *  File: TemporalBlockingInfo.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    TemporalBlockingInfo.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the TemporalBlockingInfo class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: TemporalBlockingInfo.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_TEMPORALBLOCKINGINFO_H_
#define PVTOL_TEMPORALBLOCKINGINFO_H_


#include <Exception.h>
#include <vector>

namespace ipvtol
{

  /// \brief class TemporalBlockingInfo
  /// The TemporalBlockingInfo class is given to a TemporalMap or
  /// AutoTemporalMap to specify block sizes of temporal data, rather than
  /// have block sizes be computed as a function of the local data size
  /// and the Grid.
  ///
  /// For instance, if a user knew that the optimal data size to read into
  /// to memory was X elements for dimension Y, a TemporalBlockingInfo object
  /// would be created containing X in dimension Y, and supplied to the
  /// TemporalMap.  For dimension Y, the data would then be broken up into
  /// NumYElements/X number of blocks to be read in, each of size X.
  ///
  /// This can be extended in the future to add stride and block stride
  /// information in order to bring in more complex data patterns.  At
  /// the time of implementation, no complex uses were known, so the simple
  /// case has been implemented.
  class TemporalBlockingInfo {

  public:

    /// \brief Constructor
    /// Default constructor for a TemporalBlockingInfo.  It is recommended
    /// that this method is only used internal to PVTOL in combination with
    /// the deserialization method.  The method produces an object that is
    /// not completely formed/initialized.
    TemporalBlockingInfo();

    /// \brief Constructor
    /// Construct a TemporalBlockingInfo, given an array of block lengths (in
    /// number of elements).  The block length must be given for each
    /// dimension of the data object.
    TemporalBlockingInfo(unsigned int numDims, unsigned int lengths[]);

    // \brief Constructor
    /// Construct a TemporalBlockingInfo, given an array of block lengths and
    /// an array of overlap values (both in specified in terms of the number
    /// of elements).  The block length and overlap values must be given for
    /// each dimension of the data object.
    TemporalBlockingInfo(unsigned int numDims, unsigned int lengths[],
			 unsigned int overlaps[]);

    /// \brief Copy constructor
    /// Construct a TemporalBlockingInfo, given another TemporalBlockingInfo.
    TemporalBlockingInfo(const TemporalBlockingInfo &);

    /// \brief Destructor
    ~TemporalBlockingInfo();

    /// \brief getNumDimensions
    /// Return the number of dimensions in the object.
    unsigned int getNumDimensions() const;

    /// \brief getLength
    /// Return the block length for the requested dimension.
    unsigned int getLength(unsigned int dim) const;

    /// \brief getOverlap
    /// Return the overlap size for the requested dimension.
    unsigned int getOverlap(unsigned int dim) const;

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

    /// \brief Assignment operator
    const TemporalBlockingInfo & operator=(const TemporalBlockingInfo &);

    /// \brief Equality operator
    bool operator==(const TemporalBlockingInfo &) const;

    /// \brief Inequality operator
    bool operator!=(const TemporalBlockingInfo &) const;

  private:

    /// \todo Replace the use of vectors with C-style arrays of size T,
    /// where the class is templated on the number of dimensions.
    std::vector<unsigned int> m_lengths;
    std::vector<unsigned int> m_overlaps;

  }; // class TemporalBlockingInfo


  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Constructor
  // Default constructor for a TemporalBlockingInfo.  It is recommended
  // that this method is only used internal to PVTOL in combination with
  // the deserialization method.  The method produces an object that is
  // not completely formed/initialized.
  inline
  TemporalBlockingInfo::TemporalBlockingInfo()
  {
  }

  // \brief Constructor
  // Construct a TemporalBlockingInfo, given an array of block lengths (in
  // number of elements).  The block length must be given for each
  // dimension of the data object.
  inline
  TemporalBlockingInfo::TemporalBlockingInfo(unsigned int numDims,
					     unsigned int lengths[]) {
    for(unsigned int i = 0; i < numDims; i++) {
      m_lengths.push_back(lengths[i]);
      m_overlaps.push_back(static_cast<unsigned int>(0));
    }
  }

  // \brief Constructor
  // Construct a TemporalBlockingInfo, given an array of block lengths and
  // an array of overlap values (both in specified in terms of the number
  // of elements).  The block length and overlap values must be given for
  // each dimension of the data object.
  inline
  TemporalBlockingInfo::TemporalBlockingInfo(unsigned int numDims,
					     unsigned int lengths[],
					     unsigned int overlaps[]) {
    for(unsigned int i = 0; i < numDims; i++) {
      m_lengths.push_back(lengths[i]);
      m_overlaps.push_back(overlaps[i]);
    }
  }

  // \brief Copy constructor
  // Construct a TemporalBlockingInfo, given another TemporalBlockingInfo.
  inline
  TemporalBlockingInfo::TemporalBlockingInfo(const TemporalBlockingInfo &
					     other) {
    for(unsigned int i = 0; i < other.m_lengths.size(); i++) {
      m_lengths.push_back(other.m_lengths[i]);
      m_overlaps.push_back(other.m_overlaps[i]);

    }
  }

  // \brief Destructor
  inline
  TemporalBlockingInfo::~TemporalBlockingInfo() {
  }

  // \brief getNumDimensions
  // Return the number of dimensions in the object.
  inline
  unsigned int TemporalBlockingInfo::getNumDimensions() const {
    return m_lengths.size();
  }

  // \brief getLength
  // Return the block length for the requested dimension.
  inline
  unsigned int TemporalBlockingInfo::getLength(unsigned int dim) const {
    if(dim >= m_lengths.size()) {
      throw Exception("Invalid dimension.",
		      __FILE__, __LINE__);
    }
    return m_lengths[dim];
  }

  // \brief getOverlap
  // Return the overlap size for the requested dimension.
  inline
  unsigned int TemporalBlockingInfo::getOverlap(unsigned int dim) const {
    if(dim >= m_overlaps.size()) {
      throw Exception("Invalid dimension.",
		      __FILE__, __LINE__);
    }
    return m_overlaps[dim];
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int TemporalBlockingInfo::getSerializedSize() const {
    return m_lengths.size() * 2 + 1;
  }

  // \brief Inequality operator
  inline
  bool
  TemporalBlockingInfo::operator!=(const TemporalBlockingInfo & rhs) const {
    return (!(*this == rhs));
  }

} // namespace ipvtol


#endif // PVTOL_TEMPORALBLOCKINGINFO_H_ not defined

