/**
 *    File: TaskBase.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TaskBase class.
 *
 *  $Id: TaskBase.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 *  Author: Sanjeev Mohindra
 *
 *   Also see TaskBase.inl for all TaskBase's inline methods
 */
#ifndef TASKBASE_H_
#define TASKBASE_H_

#include <PvtolBasics.h>
#include <CommScope.h>
#include <TaskMap.h>
#include <ThreadManager.h>
#include <TaskBarrierData.h>
#include <InterThreadXferData.h>
#include <CdtLocalXferData.h>
#include <XferDealer.h>
//#include <HTaskInfo.h>
#include <HeterogeneousMap.h>

#include <string>
#include <vector>
#include <map>

namespace ipvtol {

///TaskBase is the base class of all the Task objects
class TaskBase
    {
    public:
       // Heterogeneous Task Info
       //HTaskInfo * getHInfo();
       HeterogeneousMap * getHMap();

	    ///Get the Name of the Task
	    std::string getName() const;

	    ///Get the Taskmap
	    TaskMap& getMap() const;

	    //TASK INFORMATION

	    ///Get the Task ID
	    TaskId getTaskID() const;


	    ///Get the Task ID of the parent
	    TaskId getParentTaskID() const;

	    //Get the Parent Task
	    TaskBase& getParentTask();

	//Get the Commscope associated with the current Task
    	CommScope& getCommScope();

    	///Get the number of processes assigned to this task
	    int getNumProcesses() const;

	    ///Get the list of processes assigned to this task
	    void getProcesses(std::vector<ProcId>& pList) const;

	    ///Get the process rank associated with the current task
	    int getProcessRank() const;

	    ///Get the number of ranks in the Task
	    int getNumRanks() const;

	    ///Get the list of ranks assigned to this task
    	void getRankList(std::vector<ProcId>& rankList) const;


    	//THREAD INFORMATION

	    ///Get the number of local threads on the current processor
	    int getNumLocalThreads()  const;

	    ///Get the Thread manager
	    ThreadManager& getThreadManager();

	    ///Get the local thread rank of the current thread
	    int getLocalThreadRank();

	    ///Get the global thread rank of the current thread
	    int getGlobalThreadRank();



	    //SYNCHRONIZATION PRIMITIVES
	    ///Get the Barrier Data structure
	    TaskBarrierData& getTaskBarrierData();

	    //DATA MOVEMENT PRIMITIVES
        int getTransferKey(int localRank);
        int getCdtLocalTransferKey(int localRank);
        int getTransferTag(int localRank);

            ///InterThread transfer Mutex
	    pthread_mutex_t& getItxdDbMutex();

	    ///Interthread Transfer Database
	    std::map<int, InterThreadXferData>& getItxdDb();

            ///Conduit Local transfer Mutex
	    pthread_mutex_t& getClxdDbMutex();

	    ///Conduit Local Transfer Database
	    std::map<int, CdtLocalXferData>& getClxdDb();

	    ///STATE INFORMATION

		///Has th task ben cancelled  : not used
	    bool isCanceled() const;
	    ///Has the task completed
	    bool isCompleted() const;

	    ///Is the task replicated? LOCAL REPLICATION???  : not used
	    bool isReplicated() const;

	    ///Number of replicas : not used
	    int numReplicas() const;
	    ///Replica Rank  : not used
	    int replicaRank() const;

	    ///Destructor
	    virtual ~TaskBase();

	    ///Output operator
	    friend ostream& operator<< (ostream& output, const TaskBase& taskBase);


    protected:   
      // Heterogeneous Task Information
//      HTaskInfo m_hInfo;
      HeterogeneousMap m_hMap;

    	//Identity information
    	///Name of the Task
	    std::string m_Name;

	    ///Task Map
	    TaskMap* m_mapPtr;

	    ///Task ID of the parent
	    TaskId m_ParentTid;

	    ///Task ID
	    TaskId m_Tid;

	    ///Number of processes assigned to this task
	    int m_numProcesses;

	    ///The process rank associated with the current Task
	    int m_processRank;

	    ///Commscope
	    CommScope   *m_commScopePtr;

	    ///List of processes assigned to this task
	    std::vector<ProcId> m_processList;

	    ///Task RankList
	    std::vector<ProcId> m_rankList;


	    ///Thread manager
	    ThreadManager* m_threadManagerPtr;

	    ///Number of local threads on the current processor
	    int m_numLocalThreads;

    	///LocalRank -> globalRank
    	std::vector<ProcId> m_globalRanks;

    	///LocalRank -> ThreadID
    	std::vector<ThreadId> m_ThreadIds;

    	///LocalRank -> Thread
    	std::vector<pthread_t> m_Threads;

    	///ThreadId -> LocalRank
    	std::map<ThreadId, RankId> m_ThreadRegistry;
        /////

        //Adds the current task to the registry, and recieve a TaskID
        //     from TaskManager
        TaskId RegisterTask();

        void buildXferDealers();

        // inter Thread Transfer Data Databases
        //       & mutex guarding them
        //       & key dealers
        pthread_mutex_t                    m_itxdDbMutex;
        pthread_mutex_t                    m_clxdDbMutex;
        std::map<int, InterThreadXferData> m_itxdDb;
        std::map<int, CdtLocalXferData>    m_clxdDb;
        XferDealer*                        m_keyDealers;
        XferDealer*                        m_cdtDealers;
        XferDealer*                        m_tagDealers;

		//The following fields for state information
	        ///Has the task been cancelled
		bool m_Cancelled;
		///Has the task completed
		bool m_Completed;

	    //The following fields for replicated tasks
	    bool m_Replicated;
	    int m_numReplicas;
	    int m_replicaRank;


	    //Synchronization
	    ///Task Barrier structure
        TaskBarrierData* m_taskBarrierDataPtr;

	    //Exception Handling
	    Exception* m_ExceptionPtr;

	    //Constructor

	    ///Constructor not for public use
		TaskBase();

		///Display information about the task
		virtual ostream& display (ostream& output) const;

    };//end TaskBase


//Implementation
//inline HTaskInfo * TaskBase::getHInfo() { return &m_hInfo; }
inline HeterogeneousMap * TaskBase::getHMap() { return &m_hMap; }

inline
std::string TaskBase::getName() const
 {return(m_Name);}

inline
TaskMap& TaskBase::getMap() const
 { return(*m_mapPtr);}


///Get the Task ID
inline
TaskId TaskBase::getTaskID() const
{
	return m_Tid;
}

///Get the Task ID of the parent
inline
TaskId TaskBase::getParentTaskID() const
{
	return m_ParentTid;
}


///Get the Commscope associated with the current Task
inline
CommScope& TaskBase::getCommScope()
{
	return(*m_commScopePtr);
}

///Get the number of processes assigned to this task
inline
int TaskBase::getNumProcesses() const
{
	return(m_numProcesses);
}

///Get the list of processes assigned to this task
inline
void TaskBase::getProcesses(std::vector<ProcId>& pList) const
{
	pList = m_processList;
}

///Get the process rank associated with the current task
inline
int TaskBase::getProcessRank() const
{

	return(m_processRank);
}


///Get the list of ranks assigned to this task. This is relative to parent Task
inline
void TaskBase::getRankList(std::vector<ProcId>& rankList) const
{
	rankList = m_rankList;
}
//THREAD INFORMATION

inline
ThreadManager& TaskBase::getThreadManager()
{
	return *m_threadManagerPtr;
}



///Get the number of local threads on the current processor
inline
int TaskBase::getNumRanks()  const
{
	return(m_rankList.size());
}

inline
int TaskBase::getNumLocalThreads()  const
{
	return(m_numLocalThreads);
}

///Get the local thread rank of the current thread
inline
int TaskBase::getLocalThreadRank()
{
	if (m_numLocalThreads == 0)
		return(-1);

	if (m_numLocalThreads == 1)
		return(0);

	int localRank = 0;
	ThreadId th = ThreadManager::getThreadId();
	try {
		localRank = m_ThreadRegistry[th];
	}
	catch(...) {return 0;}

	return(localRank);
}

///Get the global thread rank of the current thread
inline
int TaskBase::getGlobalThreadRank()
{
	if (m_numLocalThreads == 0)
	        return(-1);
	else
		return(m_globalRanks[getLocalThreadRank()]);
}

inline
pthread_mutex_t& TaskBase::getItxdDbMutex()
  {
    return(m_itxdDbMutex);
  }

inline
pthread_mutex_t& TaskBase::getClxdDbMutex()
  {
    return(m_clxdDbMutex);
  }

inline
std::map<int, InterThreadXferData>& TaskBase::getItxdDb()
  {
     return(m_itxdDb);
  }

inline
std::map<int, CdtLocalXferData>& TaskBase::getClxdDb()
  {
     return(m_clxdDb);
  }


//State Information

inline
bool TaskBase::isCanceled() const
{
	return m_Cancelled;
}

inline
bool TaskBase::isCompleted() const
{
	return m_Completed;
}

inline
bool TaskBase::isReplicated() const
{
	return m_Replicated;
}

inline
int TaskBase::numReplicas() const
{
	return m_numReplicas;
}

inline
int TaskBase::replicaRank() const
{
	return m_replicaRank;
}

//Synchronisation and Communication
inline
TaskBarrierData& TaskBase::getTaskBarrierData()
{
   return(*m_taskBarrierDataPtr);
}

inline
void TaskBase::buildXferDealers()
{
	m_keyDealers = new  XferDealer[m_numLocalThreads];
	m_cdtDealers = new  XferDealer[m_numLocalThreads];
	m_tagDealers = new  XferDealer[m_numLocalThreads];
}

inline
int TaskBase::getTransferKey(int localRank)
{
	return (m_keyDealers+localRank)->getDealtValue();
}

inline
int TaskBase::getCdtLocalTransferKey(int localRank)
{
	return (m_cdtDealers+localRank)->getDealtValue();
}

inline
int TaskBase::getTransferTag(int localRank)
{
	return (m_tagDealers+localRank)->getDealtValue();
}

inline
ostream& operator<< (ostream& output, const TaskBase& taskBase)
{
	return taskBase.display(output);
}



}//namespace ipvtol

#endif /*TASKBASE_H_*/

