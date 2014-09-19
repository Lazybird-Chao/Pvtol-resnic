/**
 *  File: VsipView.inl
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Definition of the VsipView class template methods.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: VsipView.inl 945 2009-02-24 21:28:21Z jav5880 $
 */

namespace ipvtol
{
  /// 1D constructors
  template<class T, class Block>
  VsipView<1, T, Block>::VsipView(Length<1> len) 
    : vsip::Vector<T, Block>(len[0])
  {
  }
  
  template<class T, class Block>
  VsipView<1, T, Block>::VsipView(Length<1> len, const T& val)
    : vsip::Vector<T, Block>(len[0], val)
  {
  }

  template<class T, class Block>
  VsipView<1, T, Block>::VsipView(Block& localIncoreStorage)
    : vsip::Vector<T, Block>(localIncoreStorage)
  {
  }

  /// 1D element access
  /*
template<class T, class Block>
  typename vsip::Vector<T, Block>::const_reference_type 
    VsipView<1, T, Block>::operator()(const Index<1>& idx) const
  {
    return base::operator()(idx[0]);
  }

  template<class T, class Block>
  typename vsip::Vector<T, Block>::reference_type 
    VsipView<1, T, Block>::operator()(const Index<1>& idx)
  {
    return base::operator()(idx[0]);
  }
*/
  /// 1D subarray
  template<class T, class Block>
  typename VsipView<1, T, Block>::const_subview_type
  VsipView<1, T, Block>::operator()(const Domain<1>& dom) const
  {
    return const_subview_type(base::operator()(dom).block());
  }

  template<class T, class Block>
  typename VsipView<1, T, Block>::subview_type
  VsipView<1, T, Block>::operator()(const Domain<1>& dom)
  {
    return subview_type(base::operator()(dom).block());
  }

  /// 1D assignment
  template<class T, class Block>
  template<class RHS>
  VsipView<1, T, Block>& VsipView<1, T, Block>::operator=(const RHS& rhs)
  {
    base::operator=(rhs);
    return *this;
  }

  /// 2D constructors
  template<class T, class Block>
  VsipView<2, T, Block>::VsipView(Length<2> len) : base(len[0], len[1])
  {
  }
  
  template<class T, class Block>
  VsipView<2, T, Block>::VsipView(Length<2> len, const T& val)
    : base(len[0], len[1], val)
  {
  }

  template<class T, class Block>
  VsipView<2, T, Block>::VsipView(Block& localIncoreStorage)
    : base(localIncoreStorage)
  {
  }

  /// 2D element access
  template<class T, class Block>
  typename VsipView<2, T, Block>::const_reference_type 
  VsipView<2, T, Block>::operator()(const Index<2>& idx) const
  {
    return base::operator()(idx[0], idx[1]);
  }

  template<class T, class Block>
  typename VsipView<2, T, Block>::reference_type 
  VsipView<2, T, Block>::operator()(const Index<2>& idx)
  {
    return base::operator()(idx[0], idx[1]);
  }

  /// 2D subarray
  template<class T, class Block>
  typename VsipView<2, T, Block>::const_subview_type
  VsipView<2, T, Block>::operator()(const Domain<2>& dom) const
  {
    return const_subview_type(base::operator()(dom).block());
  }

  template<class T, class Block>
  typename VsipView<2, T, Block>::subview_type
  VsipView<2, T, Block>::operator()(const Domain<2>& dom)
  {
    return subview_type(base::operator()(dom).block());
  }

  /// 2D assignment
  template<class T, class Block>
  template<class RHS>
  VsipView<2, T, Block>& VsipView<2, T, Block>::operator=(const RHS& rhs)
  {
    base::operator=(rhs);
    return *this;
  }

  /// 3D constructors
  template<class T, class Block>
  VsipView<3, T, Block>::VsipView(Length<3> len) 
    : vsip::Tensor<T, Block>(len[0], len[1], len[2])
  {
  }
  
  template<class T, class Block>
  VsipView<3, T, Block>::VsipView(Length<3> len, const T& val)
    : vsip::Tensor<T, Block>(len[0], len[1], len[2], val)
  {
  }

  template<class T, class Block>
  VsipView<3, T, Block>::VsipView(Block& localIncoreStorage)
    : vsip::Tensor<T, Block>(localIncoreStorage)
  {
  }

  /// 3D element access
/*
  template<class T, class Block>
  typename vsip::Tensor<T, Block>::const_reference_type 
  VsipView<3, T, Block>::operator()(const Index<3>& idx) const
  {
    return base::operator()(idx[0], idx[1], idx[2]);
  }

  template<class T, class Block>
  typename vsip::Tensor<T, Block>::reference_type 
  VsipView<3, T, Block>::operator()(const Index<3>& idx)
  {
    return base::operator()(idx[0], idx[1], idx[2]);
  }
*/
  /// 3D subarray
  template<class T, class Block>
  typename VsipView<3, T, Block>::const_subview_type
  VsipView<3, T, Block>::operator()(const Domain<3>& dom) const
  {
    return const_subview_type(base::operator()(dom).block());
  }

  template<class T, class Block>
  typename VsipView<3, T, Block>::subview_type
  VsipView<3, T, Block>::operator()(const Domain<3>& dom)
  {
    return subview_type(base::operator()(dom).block());
  }

  /// 3D assignment
  template<class T, class Block>
  template<class RHS>
  VsipView<3, T, Block>& VsipView<3, T, Block>::operator=(const RHS& rhs)
  {
    base::operator=(rhs);
    return *this;
  }
}

