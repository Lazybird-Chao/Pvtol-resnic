/**
 *    File: TaskInfo.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TaskInfo struct.
 *  
 *  $Id: TaskInfo.h 938 2009-02-18 17:39:52Z ka21088 $
 * 
 *  Author: Sanjeev Mohindra
 *
 */
#ifndef TASKINFO_H_
#define TASKINFO_H_

#include <PvtolBasics.h>
#include <pthread.h>
#include <iostream>

namespace ipvtol
{

/**
 * \brief TaskInfo is a struct that is used for Thread Local storage within the Task Class
 * 
 * \remarks This class is not designed to be used outside of the Task class.
 * 
 */
struct TaskInfo
{
	///The task id that the current thread belongs to
	TaskId taskId;
	//The Task Id of the parent task
	TaskId parentTaskId;	
	///ThreadId of the current thread
	ThreadId threadId;
	///Local rank of the thread
	int localRank;
	///global rank of the thread
	int globalRank;
	///The identity of the thread
	pthread_t self;
};


/**
 * \brief    Output operator  to output the state of TaskInfo
 *
 */
std::ostream& operator<< (std::ostream& output, const TaskInfo& taskInfo);


}

#endif /*TASKINFO_H_*/
