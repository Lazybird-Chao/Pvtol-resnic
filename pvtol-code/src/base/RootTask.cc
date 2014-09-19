/**
 * File: RootTask.cc
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of RootTask
 *
 *  $Id: RootTask.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <RootTask.h>



namespace ipvtol
{

/**
 *  Method:     Constructor
 *    RootTask(int size)
 *
 *  \brief RootTask Constructor
 *  \param int worldSize: total number of processes
 *
 */
 RootTask::RootTask(int worldSize)
 :TaskBase()
  {
    m_commScopePtr    = new CommScope(worldSize);
    m_numProcesses    = worldSize;
    m_numLocalThreads = 1;
    TaskBase::buildXferDealers();
    m_Name            = "RootTask";
    int threadRank = 0;

    ThreadId id = ThreadManager::getThreadId();
	
    TaskManager::getNewTaskId(m_Tid); //0; //static_cast<TaskId> (id);
    m_ThreadIds.push_back(id);
    m_Threads.push_back(pthread_self());
    m_ThreadRegistry.insert(std::pair<ThreadId,RankId>(id,threadRank));
    
    m_ParentTid       = m_Tid;//Note: parent id == current id for Root Task

    m_processRank     = m_commScopePtr->getProcId();

	//Create a TaskInfo object to place in Thread local storage
	TaskInfo* taskInfoPtr = new TaskInfo();	
	taskInfoPtr->taskId = m_Tid;
	taskInfoPtr->parentTaskId = m_ParentTid;
	taskInfoPtr->self = pthread_self();
	taskInfoPtr->threadId = id;
	taskInfoPtr->localRank = 0;
	taskInfoPtr->globalRank = m_processRank;

	//Place it in TSS
	int rc = pthread_setspecific(TaskManager::getTaskInfoKey(),(void *) taskInfoPtr);
	if (rc)
		std::cerr << "Error in setting the TSS: rc=" <<rc <<std::endl;

    m_globalRanks.push_back(m_processRank); 

    for (int i=0; i<worldSize; i++) {
      m_processList.push_back(i);
      m_rankList.push_back(i);
    }//endFor each process

    RankList            rl(m_rankList);
    TaskDistDescription tdd;
    m_mapPtr = new TaskMap(rl, tdd);

    return;
  }//end Construct


RootTask::~RootTask()
 {return;}



}//end Namspace

