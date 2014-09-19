/**
 *  File: DataMgmtPolicy.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DataMgmtPolicy.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the DataMgmtPolicy class.
 * 
 *  Author: Ryan Haney
 *
 *  $Id: DataMgmtPolicy.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATAMGMTPOLICY_H_
#define PVTOL_DATAMGMTPOLICY_H_


#include <PvtolBasics.h>

namespace ipvtol
{

  /// \brief class DataMgmtPolicy
  /// The Data Management Policy class contains policy information on 
  /// how temporal data should be loaded/stored.  A data management
  /// policy object is typically contained within a temporal map.
  /// Undefined policies are only allowed within AutoMaps.
  class DataMgmtPolicy {
    
  public:
    
    /// \brief enum ParameterType
    /// The ParameterType enumeration allows the user to leave some of the 
    /// data management policies undefined to be set by the automated
    /// mapping software.
    enum ParameterType {
      UNDEFINED
    };
    
    /// \brief Constructor
    /// Construct a DataMgmtPolicy with default values:
    /// lazy allocation (false), save data (true), load data (true),
    /// multi buffer size (1).
    DataMgmtPolicy();
    
    /// \brief Constructor
    /// Construct a DataMgmtPolicy given the parameter values.  For 
    /// multi-buffering, a value of 1 is the default.  Therefore to 
    /// "double-buffer," a value of 2 is required.
    DataMgmtPolicy(bool lazyAllocation, bool saveData, 
		   bool loadData, unsigned int multiBufferSize);
    
    /// \brief Constructor
    /// Construct a DataMgmtPolicy given the parameter values.  The
    /// lazyAllocation flag is left UNDEFINED.  The values will be 
    /// automatically set by the automatic mapper.  This DataMgmtPolicy 
    /// can only be used with AutoMaps!  For multi-buffering, a value of 1 
    /// is the default.  Therefore to "double-buffer," a value of 2 is 
    /// required.
    DataMgmtPolicy(ParameterType lazyAllocation, bool saveData, 
		   bool loadData, unsigned int multiBufferSize);
    
    /// \brief Constructor
    /// Construct a DataMgmtPolicy given the parameter values.  The
    /// lazyAllocation flag and multiBufferSize are left UNDEFINED and will
    /// be automatically set by the automatic mapper.  This DataMgmtPolicy can 
    /// only be used with AutoMaps!
    DataMgmtPolicy(ParameterType lazyAllocation, bool saveData, 
		   bool loadData, ParameterType multiBufferSize);
    
    /// \brief Constructor
    /// Construct a DataMgmtPolicy given the parameter values.  The
    /// multiBufferSize is left UNDEFINED and will be automatically set by the 
    /// automatic mapper.  This DataMgmtPolicy can only be used with AutoMaps!
    DataMgmtPolicy(bool lazyAllocation, bool saveData, 
		   bool loadData, ParameterType multiBufferSize);
    
    /// \brief Copy constructor
    DataMgmtPolicy(const DataMgmtPolicy & other);
    
    /// \brief Destructor
    ~DataMgmtPolicy();
    
    /// \brief getLazyAllocationFlag
    /// Returns a boolean describing whether or not storage for subarrays 
    /// should be allocated when first accessed.  True directs PVTOL to use 
    /// lazy allocation, allocating the array when first accessed.  False 
    /// directs PVTOL to NOT use lazy allocation, allocating storage during 
    /// array construction.  The default value is false.  For AutoMaps, the 
    /// method isMultiBufferSizeDefined method should be used prior to this 
    /// method to see if the value has been defined.  This method will return 
    /// the default value (false) if the undefined value has not been set.
    bool getLazyAllocationFlag() const;  
    
    /// \brief isLazyAllocationDefined 
    /// Returns a boolean indicating if the lazyAllocation flag has been 
    /// defined. For AutoMaps, this should be used prior to using 
    /// getLazyAllocationFlag().
    bool lazyAllocationDefined() const;
    
    /// \brief getSaveDataFlag
    /// Returns a boolean describing whether or not contents of subarray 
    /// should be saved to the parent subarray when a new subarray is loaded.  
    /// True directs PVTOL to save data when a new subarray is loaded.  
    /// False directs PVTOL to NOT save data when a new subarray is loaded.  
    /// The default value is true.
    bool getSaveDataFlag() const;
    
    /// \brief getLoadDataFlag
    /// Returns a boolean describing whether or not contents of a subarray 
    /// should be loaded when a new subarray is accessed.  True directs PVTOL 
    /// to load subarray contents upon access.  False directs PVTOL to NOT 
    /// load subarray contents upon access.  The default value is true.
    bool getLoadDataFlag() const;

    /// \brief getMultiBufferSize 
    /// Returns an unsigned integer describing the mult-buffering depth for 
    /// subarrays from the parent subarray.  The method 
    /// isMultiBufferSizeDefined should be used prior to this method for 
    /// AutoMaps to see if the value has been set.  This method will return 
    /// the default value (1) if the undefined value has not been set.  For 
    /// multi-buffering, a value of 1 is the default.  Therefore to 
    /// "double-buffer," a value of 2 is required.
    unsigned int getMultiBufferSize() const;
    
    /// \brief isMultiBufferSizeDefined
    /// Returns a boolean indicating if the mutliBufferSize has been defined.
    /// For AutoMaps, this should be used prior to using getMultiBufferSize().
    bool multiBufferSizeDefined() const;
    
    /// \brief setLazyAllocationFlag
    /// Set the boolean describing whether or not storage for subarrays 
    /// should be allocated when first accessed.  True directs PVTOL to use 
    /// lazy allocation, allocating the array when first accessed.  
    /// False directs PVTOL to NOT use lazy allocation, allocating storage 
    /// during array construction.  The default value is false.  This should 
    /// only be used for DataMgmtPolicy objects that have an undefined 
    /// lazyAllocation policy. This should only be used for AutoMaps.
    void setLazyAllocationFlag(bool);
    
    /// \brief getMultiBufferSize 
    /// Sets an unsigned integer describing the multi-buffering depth for 
    /// subarrays from the parent subarray.  For multi-buffering, a value
    /// of 1 is the default.  Therefore to "double-buffer," a value of 2 is 
    /// required.
    void setMultiBufferSize(unsigned int);

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
    const DataMgmtPolicy & operator=(const DataMgmtPolicy & other);
    
    /// \brief Equality operator
    bool operator==(const DataMgmtPolicy & other) const;
    
    /// \brief Inequality operator
    bool operator!=(const DataMgmtPolicy & other) const;
    
  private:
    
    bool          m_lazyAllocation;
    bool          m_lazyAllocationDefined;
    bool          m_saveData;
    bool          m_loadData;
    unsigned int  m_multiBufferSize;
    bool          m_multiBufferSizeDefined;
    
  }; // class DataMgmtPolicy
  

  //------------------
  // INLINE FUNCTIONS
  //------------------
  

  // \brief Constructor
  // Construct a DataMgmtPolicy with default values:
  // lazy allocation (false), save data (true), load data (true),
  // multi buffer size (1).
  inline
  DataMgmtPolicy::DataMgmtPolicy() 
    : m_lazyAllocation(false),
      m_lazyAllocationDefined(true),
      m_saveData(true),
      m_loadData(true),
      m_multiBufferSize(1),
      m_multiBufferSizeDefined(false)
  {
  }
  
  // \brief Constructor
  // Construct a DataMgmtPolicy given the parameter values.
  inline
  DataMgmtPolicy::DataMgmtPolicy(bool lazyAllocation, bool saveData, 
				 bool loadData, unsigned int multiBufferSize)
    : m_lazyAllocation(lazyAllocation),
      m_lazyAllocationDefined(true),
      m_saveData(saveData),
      m_loadData(loadData),
      m_multiBufferSize(multiBufferSize),
      m_multiBufferSizeDefined(true)
  {
  }
  
  // \brief Constructor
  // Construct a DataMgmtPolicy given the parameter values.  The
  // lazyAllocation flag is left UNDEFINED.  The values will be automatically
  // set by the automatic mapper.  This DataMgmtPolicy can only be used with 
  // AutoMaps!
  inline
  DataMgmtPolicy::DataMgmtPolicy(ParameterType lazyAllocation, bool saveData, 
				 bool loadData, unsigned int multiBufferSize)
    : m_lazyAllocation(false),
      m_lazyAllocationDefined(false),
      m_saveData(saveData),
      m_loadData(loadData),
      m_multiBufferSize(multiBufferSize),
      m_multiBufferSizeDefined(true)
  {
  }  
  
  // \brief Constructor
  // Construct a DataMgmtPolicy given the parameter values.  The
  // lazyAllocation flag and multiBufferSize are left UNDEFINED and will
  // be automatically set by the automatic mapper.  This DataMgmtPolicy can 
  // only be used with AutoMaps!
  inline
  DataMgmtPolicy::DataMgmtPolicy(ParameterType lazyAllocation, bool saveData, 
				 bool loadData, ParameterType multiBufferSize)
    : m_lazyAllocation(false),
      m_lazyAllocationDefined(false),
      m_saveData(saveData),
      m_loadData(loadData),
      m_multiBufferSize(1),
      m_multiBufferSizeDefined(false)
  {
  }  
  
  // \brief Constructor
  // Construct a DataMgmtPolicy given the parameter values.  The
  // multiBufferSize is left UNDEFINED and will be automatically set by the 
  // automatic mapper.  This DataMgmtPolicy can only be used with AutoMaps!
  inline
  DataMgmtPolicy::DataMgmtPolicy(bool lazyAllocation, bool saveData, 
				 bool loadData, ParameterType multiBufferSize)
    : m_lazyAllocation(false),
      m_lazyAllocationDefined(true),
      m_saveData(saveData),
      m_loadData(loadData),
      m_multiBufferSize(1),
      m_multiBufferSizeDefined(false)
  {
  }   

  // \brief Copy constructor
  inline
  DataMgmtPolicy::DataMgmtPolicy(const DataMgmtPolicy & other)
  : m_lazyAllocation(other.m_lazyAllocation),
    m_lazyAllocationDefined(other.m_lazyAllocationDefined),
    m_saveData(other.m_saveData),
    m_loadData(other.m_loadData),
    m_multiBufferSize(other.m_multiBufferSize),
    m_multiBufferSizeDefined(other.m_multiBufferSizeDefined)
  {
  }
  
  // \brief Destructor
  inline
  DataMgmtPolicy::~DataMgmtPolicy() {
  }

  // \brief getLazyAllocationFlag
  // Returns a boolean describing whether or not storage for subarrays 
  // should be allocated when first accessed.  True directs PVTOL to use lazy
  // allocation, allocating the array when first accessed.  False directs 
  // PVTOL to NOT use lazy allocation, allocating storage during array 
  // construction.  The default value is false.
  inline
  bool DataMgmtPolicy::getLazyAllocationFlag() const {
    return m_lazyAllocation;
  }
  
  // \brief isLazyAllocationDefined 
  // Returns a boolean indicating if the lazyAllocation flag has been defined.
  // For AutoMaps, this should be used prior to using getLazyAllocationFlag().
  inline
  bool DataMgmtPolicy::lazyAllocationDefined() const {
    return m_lazyAllocationDefined;
  }
  
  // \brief getSaveDataFlag
  // Returns a boolean describing whether or not contents of subarray 
  // should be saved to the parent subarray when a new subarray is loaded.  
  // True directs PVTOL to save data when a new subarray is loaded.  
  // False directs PVTOL to NOT save data when a new subarray is loaded.  
  // The default value is true.
  inline
  bool DataMgmtPolicy::getSaveDataFlag() const {
    return m_saveData;
  }
  
  // \brief getLoadDataFlag
  // Returns a boolean describing whether or not contents of a subarray 
  // should be loaded when a new subarray is accessed.  True directs PVTOL 
  // to load subarray contents upon access.  False directs PVTOL to NOT load 
  // subarray contents upon access.  The default value is true.
  inline
  bool DataMgmtPolicy::getLoadDataFlag() const {
    return m_loadData;
  }
  
  // \brief getMultiBufferSize 
  // Returns an unsigned integer describing the mult-buffering depth for 
  // subarrays from the parent subarray.
  inline
  unsigned int DataMgmtPolicy::getMultiBufferSize() const {
    return m_multiBufferSize;
  }
  
  // \brief getMultiBufferSize 
  // Returns a boolean indicating if the mutliBufferSize has been defined.
  // For AutoMaps, this should be used prior to using getMultiBufferSize().
  inline
  bool DataMgmtPolicy::multiBufferSizeDefined() const {
    return m_multiBufferSizeDefined;
  }

  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int DataMgmtPolicy::getSerializedSize() const {
    return 6;
  }
  
  // \brief Inequality operator
  inline
  bool DataMgmtPolicy::operator!=(const DataMgmtPolicy & other) const {
    return !(*this == other);
  }

} // namespace ipvtol


#endif // PVTOL_DATAMGMTPOLICY_H_ not defined

