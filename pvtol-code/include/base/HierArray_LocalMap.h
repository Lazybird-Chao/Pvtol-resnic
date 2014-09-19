/**
 *  File: HierArray_LocalMap.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the HierArray class template specialized for 
 *           LocalMap.  The HierArray class is the PVTOL hierarchical, 
 *           distributed array class.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: HierArray_LocalMap.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_HIER_ARRAY_LOCAL_MAP_H_
#define PVTOL_HIER_ARRAY_LOCAL_MAP_H_

#include <VsipView.h>
#include <DistributedLayout.h>
#include <LocalMap.h>

namespace ipvtol
{
  /// \brief class HierArray<N,T,Block,LocalMap>
  /// Specialization of HierArray class template when Map type is LocalMap.
  template<dimension_type N, typename T, class Block>
  class HierArray<N, T, Block, LocalMap>
  {

  public:

    /////////////////////////////////////////////////////////////////////////
    // PUBLIC CONSTANTS
    /////////////////////////////////////////////////////////////////////////
    static const int dim = N;

    /////////////////////////////////////////////////////////////////////////
    // PUBLIC TYPES
    /////////////////////////////////////////////////////////////////////////

    // why do we need this?
    typedef T ElType;

    /// convenience typedef
    typedef HierArray<N, T, Block, LocalMap> this_type;

    /// block typedef
    typedef Block block_type;

    /// element value and reference typedefs
    typedef typename block_type::value_type value_type;
    typedef typename block_type::reference_type reference_type;
    typedef typename block_type::const_reference_type const_reference_type;
    typedef T* local_pointer_type;
    typedef const T* const_local_pointer_type;

    /// subarray typedefs
    typedef typename VsipView<N,T,Block>::subview_type::block_type 
    subarray_block_type;
    typedef typename VsipView<N,T,Block>::const_subview_type::block_type 
    const_subarray_block_type;
    typedef HierArray<N, T, subarray_block_type, LocalMap> subarray_type;
    typedef HierArray<N, T, const_subarray_block_type, LocalMap> 
    const_subarray_type;

    /// local array typedefs
    typedef this_type local_array_type;
    typedef const this_type const_local_array_type;
    typedef this_type local_incore_array_type;
    typedef const this_type const_local_incore_array_type;

    /// map typedef
    typedef LocalMap map_type;

    
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    /////////////////////////////////////////////////////////////////////////

    /// \brief Constructor
    /// Construct an array of the given size
    HierArray(const Length<N>& len, const LocalMap& map = LocalMap()); 

    /// \brief Constructor
    /// Construct an array of the given size with all elements set to the 
    /// given element value
    HierArray(const Length<N>& len, 
	      const T& val, 
	      const LocalMap& map = LocalMap()); 

    /// \brief Constructor
    /// Construct an array of the given size using the given Block for 
    /// local incore storage
    HierArray(const Length<N>& len,
	      Block& localIncoreStorage, 
	      const LocalMap& map = LocalMap()); 

    /// \brief Copy Constructor
    /// Construct a shallow copy of the given HierArray.
    HierArray(const HierArray<N, T, Block, LocalMap>& other);

    /// \brief Destructor
    /// Deallocate internally allocated heap objects
    virtual ~HierArray(void);

    /// \brief Assignment
	HierArray<N, T, Block, LocalMap>&
    operator=(const HierArray<N, T, Block, LocalMap>& rhs);
    template<typename T2, typename Block2>
    HierArray<N, T, Block, LocalMap>& 
    operator=(const HierArray<N, T2, Block2, LocalMap>& rhs);
    template<typename T2>
    HierArray<N, T, Block, LocalMap>& operator=(const T2& rhs);

    /// \brief Element-accessor
    /// Return a reference to an element
    const_reference_type operator()(const Index<N>& idx) const;
    reference_type operator()(const Index<N>& idx);

    /// \brief Subarray operator
    /// Return a new subarray
    const_subarray_type operator()(const Domain<N>& dom) const;
    subarray_type operator()(const Domain<N>& dom);

    /// \brief isLocal accessor.  Always true for this specialization.
    bool isLocal(void) const;

    /// \brief numLocalLevels accessor.  Always 1 for this specialization.
    int numLocalLevels(void) const;

    /// \brief local view accessor.  Equal to 'this' pointer for this
    /// specialization.
    const_local_array_type* local(int level = 0) const;
    local_array_type* local(int level = 0);
    
    /// \brief isIncore accessor.  Always true for this specialization.
    bool isIncore(int level = 0) const;

    /// \brief localIncore array accessor.  Equal to 'this' pointer for this
    /// specialization
    const_local_incore_array_type* localIncore(int level = 0) const;
    local_incore_array_type* localIncore(int level = 0);

    /// \brief numLocalCoreBlocks accessor.  Always 1 for this specialization.
    int numLocalCoreBlocks(dimension_type dim, int level = 0) const;

    /// \brief move the specified core block into core memory.  Has no effect
    /// for this specialization
    void getLocalCoreBlock(const Index<N>& idx, int level = 0) const;

    /// \brief write the specified core block from core memory to the next
    /// level of memory up.  Has no effect for this specialization
    void putLocalCoreBlock(const Index<N>& idx, int level = 0);

    /// \brief get the DistributedLayout for this array.  Always returns a
    /// DistributedLayout containing only the local thread for this 
    /// specialization.
    DistributedLayout<N> getDistributedLayout(void) const;

    /// \brief get the local incore block for this array.
    Block& localIncoreBlock(int level = 0);
    const Block& localIncoreBlock(int level = 0) const;

    /// \brief number of elements
    length_type size(void) const;
    
    /// \brief size in specified dimension
    length_type size(dimension_type dim) const;

    /// \brief memory stride in specified dimension
    length_type stride(dimension_type dim, int level = 0) const;
    
    /// \brief pointer to local memory
    const_local_pointer_type localPointer(int level = 0) const;
    local_pointer_type localPointer(int level = 0);
    
    /// \brief return LocalMap object
    map_type map(void) const;
    
  private:
    /////////////////////////////////////////////////////////////////////////
    // PRIVATE CONSTRUCTOR
    /////////////////////////////////////////////////////////////////////////
    HierArray(const VsipView<N, T, Block>& vsipView);

    ////////////////////////////////////////////////////////////////////////
    // PRIVATE DATA
    /////////////////////////////////////////////////////////////////////////
    
    /// VSIP view to store the local incore data
#ifdef _NOT_TILL_VSIP_WORKS
    VsipView<N, T, Block> m_vsipView;
#else
    mutable VsipView<N, T, Block> m_vsipView;
#endif

    // this lets HierArrays of other types use the private constructor
    template<dimension_type N1, typename T1, class Block1, class Map1>
    friend class HierArray;

  }; // class HierArray
  
} // namespace ipvtol

#include <HierArray_LocalMap.inl>

#endif // PVTOL_HIER_ARRAY_LOCAL_MAP_H_ not defined

