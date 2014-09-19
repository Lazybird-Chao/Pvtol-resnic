/**
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline implementations of the TaskMap class.
 *
 *  $Id: TaskMap.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#include <TaskMap.h>

namespace ipvtol
{
   /// \brief Default Constructor
   /// More initialization needs to be done before an object created with
   /// this constructor can be used.
   /// This constructor does the minimum initialization needed to allow the
   /// object created to be overwritten.
   TaskMap::TaskMap() : Map()
   {
      m_mapType = Map::TASK_MAP;
      m_distDescription = new TaskDistDescription();
   }
}
