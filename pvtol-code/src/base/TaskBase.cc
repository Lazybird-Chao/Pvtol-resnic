/**
 *    File: TaskBase.cc
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   non-inline methods of the TaskBase class.
 *
 *  $Id: TaskBase.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <TaskBase.h>
#include <PvtolProgram.h>
#include <TaskManager.h>

namespace ipvtol{


TaskBase::TaskBase()
   :m_Name("NotInitialized"),
    m_ParentTid(0),
    m_Tid(0),
    m_numProcesses(1),
    m_processRank(-1),
    m_commScopePtr(NULL),
    m_threadManagerPtr(NULL),
    m_numLocalThreads(0),
    m_keyDealers(NULL),
    m_cdtDealers(NULL),
    m_tagDealers(NULL),
    m_Cancelled(false),
    m_Completed(false),
    m_Replicated(false),
    m_numReplicas(1),
    m_replicaRank(0),
    m_taskBarrierDataPtr(NULL),
    m_ExceptionPtr(NULL),
    m_hMap() //m_hInfo()
{

    m_processList.clear();
    m_rankList.clear();
    m_globalRanks.clear();
    m_ThreadIds.clear();
    m_Threads.clear();
    m_ThreadRegistry.clear();
    pthread_mutex_init(&m_itxdDbMutex, NULL);
}

TaskBase::~TaskBase()
{
    delete m_commScopePtr;
    delete m_threadManagerPtr;
    delete m_taskBarrierDataPtr;
    delete m_ExceptionPtr;
    delete [] m_tagDealers;
    delete [] m_keyDealers;
    delete [] m_cdtDealers;


    m_processList.clear();
    m_rankList.clear();
    m_globalRanks.clear();
    m_ThreadIds.clear();
    m_Threads.clear();
    m_ThreadRegistry.clear();
}

TaskBase& TaskBase::getParentTask()
{
    TaskManager* mgr = PvtolProgram::getTaskManager();
	return mgr->getParentTask();
}

ostream& TaskBase::display (ostream& output) const
{

		output <<
		"Name: " << m_Name <<
		", ProcessRank,: "<< m_processRank <<
		", NumLocalThreads: " << m_numLocalThreads <<
		endl;

	return output;

}

}//end Namespace



