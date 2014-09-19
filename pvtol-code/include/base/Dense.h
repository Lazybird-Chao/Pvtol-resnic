/**
 *  File: Dense.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the Dense class template.
 *           The Dense class is PVTOL's dense block type.
 *
 *  Author: Eddie Rutledge
 *
 *  $Id: Dense.h 938 2009-02-18 17:39:52Z ka21088 $
 */

#ifndef PVTOL_DENSE_H_
#define PVTOL_DENSE_H_

#include <vsip/dense.hpp>

namespace ipvtol
{
  // Just use VSIP Dense class for the PVTOL Dense implementation.  Make sure
  // we include the definition inside the ipvtol namespace so that use of
  // VSIPL is hidden from the user.
  using vsip::Dense;
}

#endif // PVTOL_DENSE_H_ not defined

