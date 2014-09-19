/**
 *  File: Grid.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    Grid.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition and inline methods for the Grid class.
 *
 *  Author: Ryan Haney
 *
 *  $Id: Grid.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_GRID_H_
#define PVTOL_GRID_H_


#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

namespace ipvtol
{

  /// \brief class Grid
  /// The Grid class is used by the DataMap class to relate a 
  /// list of ranks to the dimensionality of the distributed object.
  class Grid {
     friend class boost::serialization::access;
    
  public:
    
    /// \brief Default Constructor
    /// This constructor is used to build an incomplete Grid.  
    /// This should only be used internal to PVTOL.  It should be 
    /// used in combination with the deserialization methods.
    /// More initialization needs to be done before an object 
    /// created with this constructor can be used.
    Grid(); 

    /// \brief Constructor
    /// Construct a 1d Grid, given the size of the dimension.
    Grid(int dim0Size); 
    
    /// \brief Constructor
    /// Construct a 2d Grid, given the size of the dimensions.
    Grid(int dim0Size, int dim1Size);
    
    /// \brief Constructor
    /// Construct a 3d Grid, given the size of the dimensions.
    Grid(int dim0Size, int dim1Size, int dim2Size);
    
    /// \brief Copy Constructor
    /// Construct a Grid, given another grid.
    Grid(const Grid &);
    
    /// \brief getNumGridDimensions
    /// Return the number of dimensions in the grid.
    int getNumDimensions() const;
    
    /// \brief getDimension
    /// Return the value for the requested dimension of the grid.
    int getDimension(int dimension) const;
    
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
    int * deserialize(int *);

    /// \brief serialize
    /// This method is needed by the Boost::serialize methods for Atlas.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);    

    /// \brief Assignment operator
    const Grid & operator=(const Grid & other);
    
    /// \brief Equality operator
    bool operator==(const Grid & other) const;
    
    /// \brief Inequality operator
    bool operator!=(const Grid & other) const;
    
    /// \brief Destructor
    /// Destruct the object.
    ~Grid();
    
  private:
    std::vector<int> m_dimensions;

  }; // class Grid

  
  //------------------
  // INLINE FUNCTIONS
  //------------------


  // \brief Default Constructor
  // More initialization needs to be done before an object created with
  // this constructor can be used.
  inline
  Grid::Grid()
  {
  }

  // \brief Constructor
  // Construct a 1d Grid, given the size of the dimension.
  inline
  Grid::Grid(int dim0Size) {
    m_dimensions.push_back(dim0Size);
  }
  
  // \brief Constructor
  // Construct a 2d Grid, given the size of the dimensions.
  inline
  Grid::Grid(int dim0Size, int dim1Size) {
    m_dimensions.push_back(dim0Size);
    m_dimensions.push_back(dim1Size);
  }
  
  // \brief Constructor
  // Construct a 3d Grid, given the size of the dimensions.
  inline
  Grid::Grid(int dim0Size, int dim1Size, int dim2Size) {
    m_dimensions.push_back(dim0Size);
    m_dimensions.push_back(dim1Size);
    m_dimensions.push_back(dim2Size);
  }
  
  // \brief Copy Constructor
  // Construct a Grid, given another grid.
  inline
  Grid::Grid(const Grid & other) {
    m_dimensions = other.m_dimensions;
  }

  // \brief Destructor
  // Destruct the object.
  inline
  Grid::~Grid() {
  }  

  // \brief getNumGridDimensions
  // Return the number of dimensions in the grid.
  inline
  int Grid::getNumDimensions() const {
    return m_dimensions.size();
  }
  
  // \brief getSerializedSize
  // Return size of the serialized object.
  inline
  int Grid::getSerializedSize() const {
    // Return the number of dimensions + 1 buffer space to store the 
    // number of dimensions.
    return m_dimensions.size() + 1;
  }

  // \brief serialize
  // This method is needed by the Boost::serialize methods for Atlas.
  template<class Archive>
  inline
  void Grid::serialize(Archive & ar, const unsigned int version) {
    ar & m_dimensions;
  }

  // \brief Assignment operator
  inline
  const Grid & Grid::operator=(const Grid & other) {
    m_dimensions = other.m_dimensions;
    return (*this);
  }
  
  // \brief Equality operator
  inline
  bool Grid::operator==(const Grid & other) const {
    return (m_dimensions == other.m_dimensions);
  }
  
  // \brief Inequality operator
  inline
  bool Grid::operator!=(const Grid & other) const {
    return (m_dimensions != other.m_dimensions);
  }

} // namespace ipvtol


#endif // PVTOL_GRID_H_ not defined

