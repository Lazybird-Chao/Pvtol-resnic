/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  smohindra, $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Implementation of TaskManager class
 *            
 *
 *  $Id: TaskManager.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 *  Author: Sanjeev Mohindra
 */
#include <TaskManager.h>
#include <iostream>
#include <Exception.h>



namespace ipvtol{


	//Iniialize Static variables (These could be moved into Instance() )
		
	TaskManager* TaskManager::m_InstancePtr = 0;// initialize pointer
	//std::vector<TaskBase*> TaskManager::TaskRegistry;
	std::map<TaskId,TaskBase*> TaskManager::m_TaskRegistry;	

	int TaskManager::m_rc = 0;
	
	pthread_key_t   TaskManager::m_taskInfoKey = 0;
	pthread_once_t  TaskManager::m_taskOnce =  PTHREAD_ONCE_INIT;
	pthread_mutex_t TaskManager::m_Mutex =  PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t TaskManager::m_mutexTaskIdDealer =  PTHREAD_MUTEX_INITIALIZER;

	///Task Id dealer
	TaskId TaskManager::m_TaskIdDealer = 0;
	
	TaskManager::TaskManager() {}
	
	TaskManager::~TaskManager() 
	{
		m_TaskRegistry.clear();

		m_InstancePtr = 0;
		
	    pthread_key_delete(m_taskInfoKey);
	    pthread_mutex_destroy(&m_Mutex);
	    pthread_mutex_destroy(&m_mutexTaskIdDealer);
	}
	
	TaskManager* TaskManager::Instance () 
	{
    	pthread_once(&m_taskOnce, &TaskManager::init);
    	if (m_rc)
    	{
    		
    	}
		return m_InstancePtr;
	}
	
	void TaskManager::init () 
	{
		static TaskManager taskMgr;
		m_InstancePtr = &taskMgr;
		m_rc = createKey();//pthread_key_create(&m_taskInfoKey,NULL);
		
//		if (rc)
//		{
//			std::cerr << "Unable to create key" <<std::endl;
//		}
//		else
//		{
//			std::cerr << "Created Key: " << m_taskInfoKey << std::endl;
//		}
	}
	

	//This is thread safe
	TaskId TaskManager::registerTask(TaskBase* t)
	{
		pthread_mutex_lock(&m_Mutex);
		TaskId id = t->getTaskID();//TaskRegistry.size();
		m_TaskRegistry.insert(std::pair<TaskId,TaskBase*>(id,t));
		pthread_mutex_unlock(&m_Mutex);
		return id;
	}
	
	void TaskManager::unregisterTask(TaskBase* t)
	{
		pthread_mutex_lock(&m_Mutex);
		TaskId id = t->getTaskID();//TaskRegistry.size();
		if (id)
		{
			m_TaskRegistry.erase(id);
//			std::map<TaskId,TaskBase*>::iterator iter ;
//			for(iter = m_TaskRegistry.begin() ; iter != m_TaskRegistry.end() ; )
//			{
//				if((*iter).first == id)
//					m_TaskRegistry.erase( iter++ );
//				else
//					++iter;
//			}
		}
		pthread_mutex_unlock(&m_Mutex);
	}
	

	
	pthread_key_t TaskManager::getTaskInfoKey(void)
	{
		return m_taskInfoKey;
	}
	
	TaskInfo TaskManager::getTaskInfo(void)
	{
		TaskInfo info;
		void* pInfo = pthread_getspecific(m_taskInfoKey);

		if (pInfo) {
			info = * reinterpret_cast<TaskInfo*>(pInfo);
		}
		else
		{
			std::cerr << "Unable to get TaskInfo from TSS" <<std::endl;
		}

		return info;
	}

	RankId TaskManager::getLocalRank(void)
	{
		TaskInfo info;
		info.localRank = -1;
		void* pInfo = pthread_getspecific(m_taskInfoKey);

		if (pInfo) {
			info = * reinterpret_cast<TaskInfo*>(pInfo);
		}
		else
		{
			std::cerr << "Unable to get TaskInfo from TSS" <<std::endl;
		}

		return info.localRank;
	}
	
	RankId TaskManager::getGlobalRank(void)
	{
		TaskInfo info;
		info.globalRank = -1;
		void* pInfo = pthread_getspecific(m_taskInfoKey);

		if (pInfo) {
			info = * reinterpret_cast<TaskInfo*>(pInfo);
		}
		else
		{
			std::cerr << "Unable to get TaskInfo from TSS" <<std::endl;
		}

		return info.globalRank;
	}
	

	ThreadId TaskManager::getThreadId()
	{
		return ThreadManager::getCurrentThreadId();
	}
	
	//Task related convenience items
	
	
	///Get a new Task Id
	void TaskManager::getNewTaskId(TaskId& id)
	{
		pthread_mutex_lock(&m_mutexTaskIdDealer);
		id = m_TaskIdDealer++;
		pthread_mutex_unlock (&m_mutexTaskIdDealer);
	}
	
	///Get the Current Task Id
	TaskId TaskManager::getCurrentTaskId()
	{	
		TaskId taskId = 0;

		void* pInfo = pthread_getspecific(m_taskInfoKey);
		//std::cout<<"in taskmanager::getcurrenttaskid:  "<<pInfo<<endl;

		if (pInfo) {
			TaskInfo info = * reinterpret_cast<TaskInfo*>(pInfo);
			taskId = info.taskId;
		}
		else{
			std::cerr << "Unable to get TaskId from TSS" <<std::endl;
			taskId = static_cast<TaskId> (getThreadId());
		}

		return taskId;
	}
	
	///Get the Parent Task Id
	TaskId TaskManager::getParentTaskId()
	{	
		TaskId parentTaskId = 0;

		void* pInfo = pthread_getspecific(m_taskInfoKey);
		
		if (pInfo) {
			TaskInfo info = * reinterpret_cast<TaskInfo*>(pInfo);
			parentTaskId = info.parentTaskId;
		}
		else{
			std::cerr << "Unable to get TaskId from TSS" <<std::endl;
			TaskBase* currTask = m_TaskRegistry[getCurrentTaskId()];
			parentTaskId = currTask->getParentTaskID();
		}

		return parentTaskId;
	}
	
	
	TaskBase& TaskManager::getCurrentTask()
	{	
		return *m_TaskRegistry[getCurrentTaskId()];
	}
	
	TaskBase& TaskManager::getParentTask()
	{
		return *m_TaskRegistry[getParentTaskId()];
	}
	
	int TaskManager::createKey()
	{
		return pthread_key_create(&m_taskInfoKey,&TaskManager::freeTaskInfo);
	}
	
    void TaskManager::freeTaskInfo(void* pInfo)
    {
		TaskInfo * taskInfoPtr =  reinterpret_cast<TaskInfo*>(pInfo);
    	delete taskInfoPtr;
    }
    


}
