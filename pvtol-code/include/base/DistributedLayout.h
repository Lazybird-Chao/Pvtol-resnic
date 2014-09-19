/**
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   DistributedLayout class template definition
 *           DistributedLayout provides information about the distribution of
 *           data among processors (spatial distribution).  It adds to the
 *           information contained in the map by also containing array size.
 *           DistributedLayout stores PITFALLS information for each dimension.
 *          
 *           Initial implementation is stub for LocalMap Arrays.
 *
 *  $Id: DistributedLayout.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef DISTRIBUTED_LAYOUT_H
#define DISTRIBUTED_LAYOUT_H

namespace ipvtol 
{

  /**
   * \brief DistributedLayout provides information about the distribution of
   * data among processors (spatial distribution).  It adds to the
   * information contained in the map by also containing array size.
   * DistributedLayout stores PITFALLS information for each dimension.
   */
  template <dimension_type N=1>
  class DistributedLayout
  {
  public:
    DistributedLayout(void) {};
  };

} // namespace ipvtol

#endif // CLASSNAME_H
