/**
 *  File: DataMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    DataMap.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the DataMap base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: DataMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DATAMAP_H_
#define PVTOL_DATAMAP_H_


#include <vector>
#include <Map.h>
#include <RankList.h>
#include <Grid.h>
#include <DataDistDescription.h>

namespace ipvtol
{

  class TemporalMapBase;
  class RuntimeMap;
  class AutoMap;
  class LocalMap;

  /// \brief class DataMap
  /// The DataMap class is used to provide mapping information for
  /// hierarchical arrays.  It inherits from the abstract base Map class.
  class DataMap : public Map {

  public:

    /// \brief enum DataMapType
    /// A DataMap can be a LocalMap (specifies data is local to a single
    /// process/thread), RuntimeMap (fully specifies a data distribution),
    /// or AutoMap (partially specifies a data distribution, relying on
    /// automatic mapping software to fill in optimal mapping information where
    /// the map is undefined.
    enum DataMapType {
      LOCAL_MAP,
      RUNTIME_MAP,
      AUTO_MAP
    };

    /// \brief Constructor
    /// This constructor is used to build an incomplete DataMap.
    /// This should only be used internal to PVTOL.  It should be
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object
    /// created with this constructor can be used.
    DataMap();

    /// \brief Constructor
    /// Construct a DataMap, given a DataMapType.  This constructor is
    /// used to build incomplete DataMaps.  This should only
    /// be used internal to PVTOL, and should be used only by the
    /// DataMap child objects.  It should be used in combination with the
    /// deserialization methods.
    DataMap(const DataMapType);

    /// \brief Constructor
    /// Construct a DataMap, given a DataMapType, RankList, Grid, and
    /// DataDistDescription.
    DataMap(const DataMapType, const RankList &, const Grid &,
	    const DataDistDescription &);

    /// \brief Constructor
    /// Construct a DataMap, given a DataMapType, RankList, Grid,
    /// DataDistDescription, and child data map.
    DataMap(const DataMapType, const RankList &, const Grid &,
	    const DataDistDescription &,
	    const std::vector<DataMap *> &);

    /// \brief Constructor
    /// Construct a DataMap, given a DataMapType, RankList, Grid,
    /// DataDistDescription, and temporal map.
    DataMap(const DataMapType, const RankList &, const Grid &,
	    const DataDistDescription &, const TemporalMapBase &);

    /// \brief Constructor
    /// Construct a DataMap, given a DataMapType, RankList, Grid, vector of
    /// DataDistDescription pointers, child data map, and temporal map.
    DataMap(const DataMapType, const RankList &, const Grid &,
	    const DataDistDescription &, const std::vector<DataMap *> &,
	    const TemporalMapBase &);

    /// \brief Copy contructor
    /// Construct a DataMap, given another DataMap.
    DataMap(const DataMap &);

    /// \brief Destructor
    virtual ~DataMap();

    /// \brief getDataMapType()
    DataMapType getDataMapType() const;

    /// \brief getDistDescription
    /// Return the requested DistDescription object.
    virtual const DataDistDescription & getDistDescription() const;

    /// \brief hasTemporalMap
    /// Return a boolean indicating whether or not the DataMap contains a
    /// TemporalMap.
    bool hasTemporalMap() const;

    /// \brief getTemporalMap
    /// Return the TemporalMap.  Use hasTemporalMap prior to this method
    /// to be sure the DataMap contains a TemporalMap.
    virtual const TemporalMapBase & getTemporalMap() const;

    /// \brief getChildDataMap
    /// Returns the specified child map.  Use getNumChildDataMaps prior to
    /// obtain the number of child maps present.
    const DataMap & getChildDataMap(int) const;

    /// \brief getNumChildDataMaps
    int getNumChildDataMaps() const;

    /// \brief getSerializedSize
    /// Return size of the serialized object.
    int getSerializedSize() const;

    /// \brief serialize
    /// Store all object information within the supplied integer
    /// buffer.  Return the buffer, incremented to the next available storage
    /// location.
    int * serialize(int * buffer) const;

    //// \brief deserialize
    /// Initialize the object with the information supplied within
    /// the buffer.  Return the buffer, incremented to the next available
    /// storage location.
    int * deserialize(int *);

    /// \brief clone
    /// Allocate and create a copy of the current object.  The user is
    /// responsible for managing the returned memory.
    virtual DataMap * clone() const;

    /// \brief Assignment operator
    virtual const DataMap & operator=(const DataMap & other);

    /// \brief Equality operator
    virtual bool operator==(const DataMap & other) const;

    /// \brief Inequality operator
    virtual bool operator!=(const DataMap & other) const;

  protected:

    std::vector<DataMap *> m_childDataMaps;
    int                    m_numChildDataMaps;
    bool                   m_hasTemporalMap;
    TemporalMapBase *      m_temporalMapPtr;
    DataMapType            m_dataMapType;

    /// \brief copyChildDataMaps
    void copyChildDataMaps(const std::vector<RuntimeMap *> &);

    /// \brief copyChildDataMaps
    void copyChildDataMaps(const std::vector<LocalMap *> &);

    /// \brief copyChildDataMaps
    void copyChildDataMaps(const std::vector<AutoMap *> &);

    /// \brief copyChildDataMaps
    void copyChildDataMaps(const std::vector<DataMap *> &);

  private:

    /// \brief freeDataMapMemory()
    void freeDataMapMemory();

  }; // class DataMap

} // namespace ipvtol

// Include inline definitions
#include <DataMap.inl>


#endif // PVTOL_DATAMAP_H_ not defined

