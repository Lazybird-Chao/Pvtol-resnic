/**
 *  File: VsipView.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the VsipView class.  VsipView presents a common
 *           constructor interface for VsipViews, regardless of the number of 
 *           dimensions.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: VsipView.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_VSIP_VIEW_H_
#define PVTOL_VSIP_VIEW_H_

#include <vsip/vector.hpp>
#include <vsip/matrix.hpp>
#include <vsip/tensor.hpp>
#include <vsip/parallel.hpp> // needed as workaround to VSIPL++ problem
#include <BasicTypes.h>

namespace ipvtol
{
  // generic version is empty - only define the class for 1D, 2D, 3D
  template<dimension_type N, class T, class Block>
  class VsipView;

  // specialization for 1D
  template<class T, class Block>
  class VsipView<1, T, Block> : public vsip::Vector<T, Block>
  {
  public:
    /// convenience typedefs
    typedef vsip::Vector<T, Block> base;
    typedef typename base::const_reference_type const_reference_type;
    typedef typename base::reference_type reference_type;
    typedef typename base::subview_type::block_type subblock_type;
    typedef typename base::const_subview_type::block_type const_subblock_type;
    typedef VsipView<1, T, subblock_type> subview_type;
    typedef VsipView<1, T, const_subblock_type> const_subview_type;

    /// constructors
    VsipView(Length<1> len);
    VsipView(Length<1> len, const T& val);
    VsipView(Block& localIncoreStorage);
    
    /// element access
    const_reference_type operator()(const Index<1>& idx) const;
    reference_type operator()(const Index<1>& idx);

    /// subarray
    const_subview_type operator()(const Domain<1>& dom) const;
    subview_type operator()(const Domain<1>& dom);

    /// assignment
    template<class RHS>
    VsipView<1, T, Block>& operator=(const RHS& rhs);
 };

  // specialization for 2D
  template<class T, class Block>
  class VsipView<2, T, Block> : public vsip::Matrix<T, Block>
  {
  public:
    /// convenience typedefs
    typedef vsip::Matrix<T, Block> base;
    typedef typename base::const_reference_type const_reference_type;
    typedef typename base::reference_type reference_type;
    typedef typename base::subview_type::block_type subblock_type;
    typedef typename base::const_subview_type::block_type const_subblock_type;
    typedef VsipView<2, T, subblock_type> subview_type;
    typedef VsipView<2, T, const_subblock_type> const_subview_type;

    /// constructors
    VsipView(Length<2> len);
    VsipView(Length<2> len, const T& val);
    VsipView(Block& localIncoreStorage);
    
    /// element access
    const_reference_type operator()(const Index<2>& idx) const;
    reference_type operator()(const Index<2>& idx);

    /// subarray
    const_subview_type operator()(const Domain<2>& dom) const;
    subview_type operator()(const Domain<2>& dom);

    /// assignment
    template<class RHS>
    VsipView<2, T, Block>& operator=(const RHS& rhs);
 };

  // specialization for 3D
  template<class T, class Block>
  class VsipView<3, T, Block> : public vsip::Tensor<T, Block>
  {
  public:
    /// convenience typedefs
    typedef vsip::Tensor<T, Block> base;
    typedef typename base::const_reference_type const_reference_type;
    typedef typename base::reference_type reference_type;
    typedef typename base::subview_type::block_type subblock_type;
    typedef typename base::const_subview_type::block_type const_subblock_type;
    typedef VsipView<3, T, subblock_type> subview_type;
    typedef VsipView<3, T, const_subblock_type> const_subview_type;

    /// constructors
    VsipView(Length<3> len);
    VsipView(Length<3> len, const T& val);
    VsipView(Block& localIncoreStorage);
    
    /// element access
    const_reference_type operator()(const Index<3>& idx) const;
    reference_type operator()(const Index<3>& idx);

    /// subarray
    const_subview_type operator()(const Domain<3>& dom) const;
    subview_type operator()(const Domain<3>& dom);

    /// assignment
    template<class RHS>
    VsipView<3, T, Block>& operator=(const RHS& rhs);
 };

}

// inline definitions
#include <VsipView.inl>

#endif // VSIP_VIEW_H_ not defined

