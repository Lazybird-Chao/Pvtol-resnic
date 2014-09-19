/**
 *    File: TaskManager.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the TaskManager class.
 *  
 *  $Id: TaskManager.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 *  Author: Sanjeev Mohindra
 */
#ifndef TASKMANAGER_
#define TASKMANAGER_

#include <PvtolBasics.h>
#include <TaskBase.h>
#include <TaskInfo.h>
#include <ThreadManager.h>
#include <map>

//This is only in the base namespace
namespace ipvtol {

/**
 * \brief TaskManager is a singleton that holds all the tasks in a process
 *
 */
class TaskManager {
public:

	///The sole instance of the class			
	static TaskManager* Instance();

	///Register a task with the Taskmanager
	static TaskId registerTask(TaskBase* t);

	///Unregister a task with the Taskmanager
	static void unregisterTask(TaskBase* t);

	///Get the key for thread local storage
	static pthread_key_t getTaskInfoKey(void);
	
	///Get the TaskInfo from thread local storage
	static TaskInfo getTaskInfo(void);
	
	///Get the local rank from thread local storage
	static RankId getLocalRank(void);
	
	///Get the global rank from thread local storage. This global rank is the rank in the parent comScope
	static RankId getGlobalRank(void);

	//Task related convenience items
	///Get a new Task Id
	static void getNewTaskId(TaskId& id);
	
	///Get the Current Task Id
	static TaskId getCurrentTaskId();

	///Get the Parent task Id
	static TaskId getParentTaskId();

	///get the current Task
	static TaskBase& getCurrentTask();

	///Get the parent Task
	static TaskBase& getParentTask();

	///Get the ThreadId of the current thread
	static ThreadId getThreadId();

	///Destructor
	~TaskManager();

protected:

	///The Task registry 
	static std::map<TaskId,TaskBase*> m_TaskRegistry;

	///The named constructor of this object
	static void init();

	///No constructor
	TaskManager();
	///No copy constructor	
	TaskManager(const TaskManager&);
	///No assignment	
	TaskManager& operator=(const TaskManager&);

private:

	///The pointer to the sole instance
	static TaskManager* m_InstancePtr;

	///Identity key for thread local storage of Taskinfo structure
	static pthread_key_t m_taskInfoKey;
	
	///pthread once variable to construct the singleton only once
	static pthread_once_t m_taskOnce;
	
	///Mutex to update the Task Registry
	static pthread_mutex_t m_Mutex;
	
	///Mutex to update the Task Id dealer
	static pthread_mutex_t m_mutexTaskIdDealer;

	///Task Id dealer
	static TaskId m_TaskIdDealer;
	
	///Variable to hold error condition if a key cannot be created
	static int m_rc;
	
	///Create a key
	static int createKey();
	
	///Delete the data that is stored at the key location (TaskInfo)
	static void freeTaskInfo(void* pInfo);
};

} //end namespace ipvtol

#endif /*TASKMANAGER_*/

