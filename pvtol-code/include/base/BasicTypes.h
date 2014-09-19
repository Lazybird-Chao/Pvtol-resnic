/**
 *  File: BasicTypes.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of some basic types that can be shared by many
 *           different modules.  Examples are index_type and dimension_type.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: BasicTypes.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_BASIC_TYPES_H_
#define PVTOL_BASIC_TYPES_H_

#include <vsip/support.hpp>
#include <vsip/domain.hpp>
#include <boost/array.hpp>

namespace ipvtol
{
  typedef vsip::length_type length_type;
  typedef vsip::dimension_type dimension_type;
  typedef vsip::index_type index_type;
  using vsip::Index;
  using vsip::Domain;
  using vsip::tuple;

  // Use boost::array for non-distributed, fixed-size arrays of arbitrary type

  // Implement Length class template using array
  template<dimension_type N>
  class Length : public boost::array<length_type, N> 
  {
  public:
    /// Conversion to VSIPL Domain<N> supports use of Length<N> class in place
    /// of Domain<N>
    operator Domain<N>() const;
  };

  // Define a few functions for converting length_type values to Length<N>
  // objects
  Length<1> buildLengthArr(length_type);
  Length<2> buildLengthArr(length_type, length_type);
  Length<3> buildLengthArr(length_type, length_type, length_type);
  template<dimension_type N>
  Length<N> buildLengthArr(const Domain<N>& dom);

} // namespace ipvtol

#include <BasicTypes.inl>

#endif // PVTOL_ARRAY_H_ not defined

