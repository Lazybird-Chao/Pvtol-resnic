/**
 *   RootTask.h
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of RootTask class
 *             RootTask is a container of the info for the main SPMD Task
 *
 *  $Id: RootTask.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *
 *  All rights reserved.
 */

#ifndef RootTask_H
#define RootTask_H

#include <PvtolBasics.h>
//#include <PvtolProgram.h> //Do we need this?
#include <TaskBase.h>
#include <TaskInfo.h>
#include <TaskManager.h>
#include <CommScope.h>
#include <vector>

namespace ipvtol
{

/**
 * \brief RootTask is a container of the information for the main SPMD Task 
 * This is not meant to be instantiated directly by the user
 */
class RootTask : public TaskBase
  {
  public:
	  
	/**
	 * \brief  Constructor for the RootTask.
	 * 
	 * \param  worldSize: The number of processes in the world  
	 */
    RootTask(int worldSize);

    ///Destructor
    ~RootTask();
//  private:
//    void        init() throw(Exception);
//    int         run() throw(Exception);
//    void        waitTillDone();

    
  private:
    RootTask();// default constructor disallowed

  };//end RootTask


}//end namespace ipvtol

#endif // RootTask_H not defined

