/**
 *  File: HierArray_LocalMap.inl
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of methods for the HierArray class template 
 *           specialization for LocalMaps.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: HierArray_LocalMap.inl 938 2009-02-18 17:39:52Z ka21088 $
 */

#include <DistributedLayout.h>

namespace ipvtol
{
  // constructors - just build the VSIPL view
  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>::HierArray(const Length<N>& len, 
					      const LocalMap& map) 
    : m_vsipView(len)
  {
  }

  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>::HierArray(const Length<N>& len, 
					      const T& val, 
					      const LocalMap& map)
    : m_vsipView(len, val)
  {
  }

  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>::HierArray(const Length<N>& len, 
					      Block& localIncoreStorage, 
					      const LocalMap& map)
    : m_vsipView(localIncoreStorage)
  {
#ifdef PVTOL_DEVELOP
    int prod = 1;
    for (unsigned int i = 0; i < N; ++i)
      {
	prod *= len[i];
      }
      unsigned int tmpprod=static_cast<unsigned int>(prod);
    assert(tmpprod == localIncoreStorage.size());
#endif
  }

  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>
  ::HierArray(const HierArray<N,T,Block,LocalMap>& other)
    : m_vsipView(other.m_vsipView)
  {
  }
  
  // destructor - no need to do anything
  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>::~HierArray(void)
  {
  }

  // assignment - just forward to VSIP view
  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>& 
  HierArray<N, T, Block, LocalMap>
  ::operator=(const HierArray<N, T, Block, LocalMap>& rhs)
  {
    m_vsipView = rhs.m_vsipView;
    return *this;
  }

  // assignment - just forward to VSIP view
  template<dimension_type N, typename T, class Block>
  template<typename T2, class Block2>
  HierArray<N, T, Block, LocalMap>& 
  HierArray<N, T, Block, LocalMap>
  ::operator=(const HierArray<N, T2, Block2, LocalMap>& rhs)
  {
    m_vsipView = rhs.m_vsipView;
    return *this;
  }

  // assignment - just forward to VSIP view
  template<dimension_type N, typename T, class Block>
  template<class T2>
  HierArray<N, T, Block, LocalMap>& 
  HierArray<N, T, Block, LocalMap>::operator=(const T2& rhs)
  {
    m_vsipView = rhs;
    return *this;
  }

  // element accessors and subview operators - just forward to VSIP view
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::const_reference_type 
  HierArray<N, T, Block, LocalMap>::operator()(const Index<N>& idx) const
  {
    return m_vsipView(idx);
  }

  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::reference_type 
  HierArray<N, T, Block, LocalMap>::operator()(const Index<N>& idx)
  {
    return m_vsipView(idx);
  }

  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::const_subarray_type 
  HierArray<N, T, Block, LocalMap>::operator()(const Domain<N>& dom) const
  {
    return typename HierArray<N, T, Block, LocalMap>::
      const_subarray_type(m_vsipView(dom));
  }

  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::subarray_type 
  HierArray<N, T, Block, LocalMap>::operator()(const Domain<N>& dom)
  {
    return typename HierArray<N, T, Block, LocalMap>::
      subarray_type(m_vsipView(dom));
  }
  
  // isLocal - always true for this specialization
  template<dimension_type N, typename T, class Block>
  bool 
  HierArray<N, T, Block, LocalMap>::isLocal(void) const
  {
    return true;
  }

  // numLocalLevels - always 1 for this specialization
  template<dimension_type N, typename T, class Block>
  int 
  HierArray<N, T, Block, LocalMap>::numLocalLevels(void) const
  {
    return 1;
  }

  // local view - equal to 'this' for this specialization
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::const_local_array_type* 
  HierArray<N, T, Block, LocalMap>::local(int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return true;
  }

  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::local_array_type* 
  HierArray<N, T, Block, LocalMap>::local(int level)
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return this;
  }

  // isIncore - always true for this specialization
  template<dimension_type N, typename T, class Block>
  bool 
  HierArray<N, T, Block, LocalMap>::isIncore(int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return true;
  }

  // localIncore array - equal to 'this' for this specialization
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::const_local_incore_array_type* 
  HierArray<N, T, Block, LocalMap>::localIncore(int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return this;
  }

  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::local_incore_array_type* 
  HierArray<N, T, Block, LocalMap>::localIncore(int level)
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return this;
  }

  // numLocalCoreBlocks - always equal to 1 for this specialization
  template<dimension_type N, typename T, class Block>
  int 
  HierArray<N, T, Block, LocalMap>::numLocalCoreBlocks(dimension_type dim, 
						       int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return 1;
  }

  // getLocalCoreBlock - no effect for this specialization
  template<dimension_type N, typename T, class Block>
  void 
  HierArray<N, T, Block, LocalMap>::getLocalCoreBlock(const Index<N>& idx, 
						      int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
  }

  // putLocalCoreBlock - no effect for this specialization
  template<dimension_type N, typename T, class Block>
  void 
  HierArray<N, T, Block, LocalMap>::putLocalCoreBlock(const Index<N>& idx, 
						      int level)
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
  }

  // getDistributedLayout - just return a layout saying everything is on 
  // the local node for this specialization
  template<dimension_type N, typename T, class Block>
  DistributedLayout<N> 
  HierArray<N, T, Block, LocalMap>::getDistributedLayout(void) const
  {
    return DistributedLayout<N>();
  }

  // localIncoreBlock - return the block contained by the VSIPL view providing
  // local incore storage.
  template<dimension_type N, typename T, class Block>
  Block&
  HierArray<N, T, Block, LocalMap>::localIncoreBlock(int level)
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return m_vsipView.block();
  }
  template<dimension_type N, typename T, class Block>
  const Block&
  HierArray<N, T, Block, LocalMap>::localIncoreBlock(int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    return m_vsipView.block();
  }

  // size - forward to VSIP view size method
  template<dimension_type N, typename T, class Block>
  length_type
  HierArray<N, T, Block, LocalMap>::size(void) const
  {
    return m_vsipView.size();
  }
  template<dimension_type N, typename T, class Block>
  length_type
  HierArray<N, T, Block, LocalMap>::size(dimension_type dim) const
  {
#ifdef PVTOL_DEVELOP
    assert(dim < N);
#endif
    return m_vsipView.size(dim);
  }

  // stride - forward to block
  template<dimension_type N, typename T, class Block>
  length_type
  HierArray<N, T, Block, LocalMap>::stride(dimension_type dim, int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(dim < N);
    assert(level == 0);
#endif
    // ### NOTE: this is specific to Sourcery VSIPL++ ###
    return m_vsipView.block().impl_stride(N, dim);
  }

  // local data pointer - return reference to first VSIP view element
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::const_local_pointer_type
  HierArray<N, T, Block, LocalMap>::localPointer(int level) const
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    // ### NOTE: this is specific to Sourcery VSIPL++ ###
    return m_vsipView.block().impl_data();
  }
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::local_pointer_type
  HierArray<N, T, Block, LocalMap>::localPointer(int level) 
  {
#ifdef PVTOL_DEVELOP
    assert(level == 0);
#endif
    // ### NOTE: this is specific to Sourcery VSIPL++ ###
    return m_vsipView.block().impl_data();
  }

  // map - just return LocalMap object
  template<dimension_type N, typename T, class Block>
  typename HierArray<N, T, Block, LocalMap>::map_type
  HierArray<N, T, Block, LocalMap>::map(void) const
  {
    return map_type();
  }
    
  template<dimension_type N, typename T, class Block>
  HierArray<N, T, Block, LocalMap>
  ::HierArray(const VsipView<N, T, Block>& vsipView)
    : m_vsipView(vsipView)
  {
  }
  
} // end namespace ipvtol
