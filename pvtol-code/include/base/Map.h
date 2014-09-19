/**
 *  File: Map.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    Map.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the Map abstract base class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: Map.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_MAP_H_
#define PVTOL_MAP_H_



#include <RankList.h>
#include <Grid.h>
#include <TaskDistDescription.h>
#include <DataDistDescription.h>

#include <vector>

namespace ipvtol
{

  /// \brief class Map
  /// The Map class is used as an abstract base for the DataMap, TaskMap, and 
  /// FunctorMap child classes.
  class Map {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief enum MapType
    enum MapType {
      DATA_MAP,
      TASK_MAP,
      FUNCTOR_MAP
    };

    /// \brief Default Constructor
    /// This constructor is used to build an incomplete Map.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    Map();

    /// \brief Constructor
    /// Construct a Map, given a MapType.  This constructor is 
    /// used to build incomplete Maps.  This should only
    /// be used internal to PVTOL, and should be used only by the
    /// Map child objects.  It should be used in combination with the
    /// deserialization methods.
    Map(const MapType);

    /// \brief Constructor
    /// Construct a Map, given a MapType, RankList, and Grid.
    Map(const MapType, const RankList &, const Grid &, 
	const DistDescription &);
    
    /// \brief Copy contructor
    /// Construct a Map, given another Map.
    Map(const Map &);
    
    /// \brief Destructor
    virtual ~Map() = 0;
    
    /// \brief getMapType
    MapType getMapType() const;
    
    /// \brief getNumRanks
    /// Return the # of ranks in the Map
    inline int getNumRanks() const;
    
    /// \brief getRankList
    /// Return the Map's RankList.
    const RankList & getRankList() const;
    
    /// \brief getRankList
    /// Return the Map's RankList.
    void getRankList(std::vector<RankId> &) const;
    
    /// \brief getGrid
    /// Return the Map's Grid.
    const Grid & getGrid() const;
    
    /// \brief getNumDimensions
    /// Return the number of dimensions for the Map.
    int getNumDimensions() const;
    
    /// \brief isLocal
    /// Return a boolean indicating if the calling thread/process taskRank 
    /// id is in the map's RankList.
    bool isLocal() const;

    /// \brief getDistDescription
    /// Return the requested dimension's DistDescription object.
    virtual const DistDescription & getDistDescription() const;

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

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
    
    /// \brief Assignment operator
    virtual const Map & operator=(const Map & other);
    
    /// \brief Equality operator
    virtual bool operator==(const Map & other) const;
    
    /// \brief Inequality operator
    virtual bool operator!=(const Map & other) const;

  protected:

    MapType           m_mapType;
    RankList          m_rankList;
    Grid              m_grid;
    DistDescription * m_distDescription;
    
  private:
    
    /// \brief freeMemory
    /// Free all allocated Map memory.
    void freeMapMemory();  
    
  }; // class Map

  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Default Constructor
  // This constructor is used to build an incomplete Map.  
  // This should only be used internal to PVTOL.  It should be 
  // used in combination with the deserialization methods.
  // More initialization needs to be done before an object 
  // created with this constructor can be used.
  inline
  Map::Map()
  {
  }
  
  // \brief Constructor
  // Construct a Map, given a MapType.  This constructor is 
  // used to build incomplete Maps.  This should only
  // be used internal to PVTOL, and should be used only by the
  // Map child objects.  It should be used in combination with the
  // deserialization methods.
  inline
  Map::Map(const MapType mapType)
    : m_mapType(mapType),
      m_distDescription(NULL)
  {
  }
  
  // \brief Constructor
  // Construct a Map, given a MapType, RankList, and Grid.
  inline
  Map::Map(const MapType mapType, const RankList & rankList, 
	   const Grid & grid, const DistDescription & distDescription)
    : m_mapType(mapType), 
      m_rankList(rankList), 
      m_grid(grid),
      m_distDescription(distDescription.clone())
  {
  }
  
  // \brief Copy contructor
  // Construct a Map, given another Map.
  inline
  Map::Map(const Map & other)
    : m_mapType(other.m_mapType),
      m_rankList(other.m_rankList),
      m_grid(other.m_grid),
      m_distDescription(other.m_distDescription->clone())
  {
  }
  
  // \brief Destructor
  inline
  Map::~Map() {
    freeMapMemory();
  }
  
  // \brief getMapType
  inline
  Map::MapType Map::getMapType() const {
    return m_mapType;
  }
  
  // \brief getRankList
  // Return the Map's RankList.
  inline
  const RankList & Map::getRankList() const {
    return m_rankList;
  }
  
  // \brief getRankList
  // Return the Map's RankList.
  inline
  void Map::getRankList(std::vector<RankId> & rankList) const {
    return m_rankList.getRankListVector(rankList);
  }
  
inline int Map::getNumRanks() const
{
  return(m_rankList.getNumRanks());
}

  // \brief getGrid
  // Return the Map's Grid.
  inline
  const Grid & Map::getGrid() const {
    return m_grid;
  }
  
  // \brief getNumDimensions
  // Return the number of dimensions for the Map.
  inline
  int Map::getNumDimensions() const {
    int val = m_grid.getNumDimensions();
    return val;
  }

  // \brief getDistDescription
  // Return the requested dimension's DistDescription object.
  inline
  const DistDescription & Map::getDistDescription() const {
    return *m_distDescription;
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void Map::serialize(Archive & ar, const unsigned int version) {
    ar & m_mapType;
    ar & m_rankList;
    ar & m_grid;
    /// We register TaskDistDescription and DataDistDescription with
    /// Boost because m_distDescription is a pointer to the base class
    /// of both of those classes. This is so that Boost knows about the
    /// derived classes and will therefore properly serialize/deserialize
    /// them.
    ar.template register_type<ipvtol::TaskDistDescription>();
    ar.template register_type<ipvtol::DataDistDescription>();
    ar & m_distDescription;
  }

} // namespace ipvtol


#endif // PVTOL_MAP_H_ not defined

