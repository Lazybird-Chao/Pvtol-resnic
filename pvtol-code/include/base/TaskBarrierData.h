/**
 *    File: TaskBarrierData.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TaskBarierData class.
 *           TaskBarrierData is a data structure used by Task wide barrier synchronizations
 *
 *  $Id: TaskBarrierData.h 938 2009-02-18 17:39:52Z ka21088 $
 * 
 *
 */
#ifndef TASKBARRIERDATA_H_
#define TASKBARRIERDATA_H_

#include <pthread.h>
#include <vector>
 
namespace ipvtol
{

///TaskBarrierData is a data structure used by Task wide barrier synchronizations
class TaskBarrierData
{
public:
	///Constructor
	TaskBarrierData();
	
	///Destructor
	virtual ~TaskBarrierData();

    ///Mutex
	pthread_mutex_t            tbdMutex;
	///Condition variable
	pthread_cond_t             barrCond;
	///A vector of mutexes
    std::vector<pthread_mutex_t *> condMutex;
    ///Vector of ranks
    std::vector<int>               rankInit;
    ///Ready? 
    int                        readyIndicator;
};

}

#endif /*TASKBARRIERDATA_H_*/
