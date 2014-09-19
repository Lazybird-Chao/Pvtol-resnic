/**
 *    File: Task.inl
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: 2009-08-12 13:26:09 -0400 (Wed, 12 Aug 2009) $
 *  \version $LastChangedRevision: 1144 $
 *  \brief   Implementation of the Task class.
 *  
 *  $Id: Task.inl 1144 2009-08-12 17:26:09Z  $
 *
 */

//A little utility function
//template<typename T> 
//void stlClear(T& container)
//{
//	//get the first iterator
//	T::iterator iter = container.begin();
//	//iterate to the end and delete items (should be pointers)
//	for(; iter !=container.end();++iter) 
//		delete(*iter);
//
//	//clear the collection (now full with dead pointers)
//	container.clear();
//} 

// template<class T>
// struct del_fun_t 
// {
// 	del_fun_t& operator()(T* p)
// 	{ 
// 		delete p;
// 		return *this;
// 	}
// };
// 
// template<class T>
// del_fun_t<T> del_fun() 
// { 
// 	return del_fun_t<T>();
// }


template<typename T>
Task<T>::Task(const std::string name, const TaskMap& map, const HeterogeneousMap& hMap ) throw(Exception)
:TaskBase()
{
   m_hMap = hMap;
	CreateTask(name,map);	
}

template<typename T>
Task<T>::Task(T& functionObject , const std::string name, const TaskMap& map, const HeterogeneousMap& hMap) throw(Exception)
:TaskBase()
{
   m_hMap = hMap;
	CreateTask(name,map,&functionObject);	
}

template<typename T>
inline 
void Task<T>::CreateTask(const std::string name, const TaskMap& map, T* functionObjectPtr ) throw(Exception)
{
	try {
	pthread_mutex_init(&m_CreationMutex,NULL);	
	pthread_mutex_init(&m_DestructionMutex,NULL);

	pthread_mutex_init(&m_Mutex,NULL);
	pthread_mutex_init(&m_InitMutex,NULL);
	pthread_mutex_init(&m_CountMutex,NULL);
	
#if PVTOL_DEVELOP
	pthread_mutex_init(&m_DebugMutex,NULL);
#endif
    
	pthread_cond_init(&m_CreationCond,NULL);
	pthread_cond_init(&m_InitCond,NULL);
	pthread_cond_init(&m_CountCond,NULL);
	pthread_cond_init(&m_RunCond,NULL);

	m_ThreadInitMutexes.clear();
	m_ThreadRunMutexes.clear();

	m_ThreadInitCond.clear();
	m_ThreadRunCond.clear();
	
	m_initFunctors = 0;
	m_runFunctors = 0;
	m_funcPtr = 0;
	
	m_func.clear();
	
	m_runFunctionPtr = 0;
	m_isRunnable = true;
	
	//Lock Mutexes to serialize operations
	pthread_mutex_lock(&m_CreationMutex);
	pthread_mutex_lock(&m_InitMutex);
	pthread_mutex_lock(&m_DestructionMutex);

    m_mapPtr             	= new TaskMap(map);
	m_NumThreadsLaunched 	= 0;
	m_InitReadyCounter   	= 0;
	m_InitCompleteCounter   = 0;
	m_RunReadyCounter    	= 0;
	
	//get my rank in the parent (current) communicator
	PvtolProgram pgm;
	//int numMpiProcesses = pgm.numProcs(); //unused variable 
	m_processRank = pgm.getProcId();
	
	m_Name               = name;
	m_ParentTid          = TaskManager::getCurrentTaskId();
	TaskManager::getNewTaskId(m_Tid ); 
#ifdef PVTOL_DEBUG
	pthread_mutex_lock(&m_DebugMutex);
	std::cout <<"In ctor: Task name:" << name <<
			" TaskId: " << m_Tid  <<
			" Parent Task: " << m_ParentTid <<std::endl;
	pthread_mutex_unlock(&m_DebugMutex);
#endif
	

	m_commScopePtr       = new CommScope(map);
	m_numProcesses       = m_commScopePtr->getNumProcs();
	m_taskBarrierDataPtr = new TaskBarrierData;
	//Get the Ranklist of the Map and store it in m_ranklist
	map.getRankList(m_rankList);

	//Get the list of processes assigned to the parent task 
	TaskBase& parentTask = TaskManager::getCurrentTask();
	std::vector<ProcId> pList;
	parentTask.getProcesses(pList);

	//Build the absolute process list based on the parent process list and ranklist
	for (std::vector<RankId>::iterator iter = m_rankList.begin(); iter != m_rankList.end(); ++iter )
	{
		m_processList.push_back(pList.at(*iter));
		//was m_processList.push_back(*iter);
	}


	int globalRank=0;
	m_numLocalThreads = 0;
	pthread_mutex_t * mtxPtr = 0;
	pthread_cond_t * condPtr =0;
	//T* funcPtr = 0; //unused variable
	std::vector<RankId>::iterator iter;
	for (iter = m_rankList.begin(); iter != m_rankList.end(); ++iter ) 
	{

		if (isLocal(*iter))
		{    		
			m_globalRanks.push_back(globalRank);
			
			//Create a mutex for the thread
			mtxPtr = new pthread_mutex_t();
			pthread_mutex_init(mtxPtr,NULL);
			pthread_mutex_lock(mtxPtr);
			m_ThreadInitMutexes.push_back(mtxPtr);  
			
			mtxPtr = new pthread_mutex_t();
			pthread_mutex_init(mtxPtr,NULL);
			pthread_mutex_lock(mtxPtr);
			m_ThreadRunMutexes.push_back(mtxPtr);  

			condPtr = new pthread_cond_t();
			pthread_cond_init(condPtr,NULL);;
			m_ThreadInitCond.push_back(condPtr);  
			
			condPtr = new pthread_cond_t();
			pthread_cond_init(condPtr,NULL);;
			m_ThreadRunCond.push_back(condPtr); 
			
			++m_numLocalThreads;
		}

		++globalRank;
	}
	
	//Initialize other structures
	// TaskBase::buildXferDealers must be called after m_numLocalThreads
	//   is set
	TaskBase::buildXferDealers();

	#ifdef PVTOL_DEBUG
	//std::cout << "Time to launch threads..." <<std::endl;
	#endif // PVTOL_DEBUG
	
	TaskManager::registerTask(this);
	
	if (m_numLocalThreads)
	{
		//Set the pthread s
		/*int rc = */pthread_setconcurrency(m_numLocalThreads); //unused variable
		//Initialize the Init Functors
		m_initFunctors = new InitFun [m_numLocalThreads];
		
#ifdef USE_RUN_FUNCTORS		
		//Initialize the Run functors
		m_runFunctors = new RunFun [m_numLocalThreads];
		
		//m_funcPtr = new T* [m_numLocalThreads];
#endif

		
		//Launch the threads
		LaunchThreads();
		
		//Create a place to store the function objects
		for (int i=0; i< m_numLocalThreads; ++i)
		{
			// If functionObjectPtr ==0 then create the function objects inside the thread using new T
			m_func.push_back(functionObjectPtr);
		}
		

	}
	
	//Was here TaskManager::registerTask(this);
	
	//Let the threads be built
	pthread_mutex_unlock(&m_CreationMutex);
	
	//This is basically a wait to exit constructor till all threads are done. This should be changed to a more 
	//efficient mechanism later on
	if (m_numLocalThreads)
	{
		counterWait(m_InitReadyCounter);
	}
	
	//std::cout << "Finished Task Constructor " <<std::endl;
	}
	catch(...){
		std::cerr << "Error in Create Task " <<std::endl;
		throw;
	}
	
}



template<typename T>
Task<T>::~Task() throw ()
{
	//Do we force a hang????
	//    pthread_mutex_lock(&m_DestructionMutex);
	//    pthread_mutex_unlock(&m_DestructionMutex);
	
	#ifdef PVTOL_DEVELOP
		pthread_mutex_destroy(&m_DebugMutex);
	#endif // PVTOL_DEBUG

	try{
	pthread_mutex_destroy(&m_Mutex);
	pthread_mutex_destroy(&m_InitMutex);
	pthread_mutex_destroy(&m_CountMutex);	
	pthread_mutex_destroy(&m_CreationMutex);
	pthread_mutex_destroy(&m_DestructionMutex);

	pthread_cond_destroy(&m_CreationCond);
	pthread_cond_destroy(&m_InitCond);
	pthread_cond_destroy(&m_CountCond);
	pthread_cond_destroy(&m_RunCond);
	
//	stlClear(m_ThreadInitMutexes);
//	stlClear(m_ThreadRunMutexes);
//	stlClear(m_ThreadInitCond);
//	stlClear(m_ThreadRunCond);
	//stlClear(m_func);

	for (std::vector<pthread_mutex_t *>::iterator iter = m_ThreadInitMutexes.begin(); iter != m_ThreadInitMutexes.end(); ++iter ) 
	{
			pthread_mutex_destroy(*iter);
	}	
	m_ThreadInitMutexes.clear();
	
	for (std::vector<pthread_mutex_t *>::iterator iter = m_ThreadRunMutexes.begin(); iter != m_ThreadRunMutexes.end(); ++iter ) 
	{
		pthread_mutex_destroy(*iter);
	}	
	m_ThreadRunMutexes.clear();
	for (std::vector<pthread_cond_t *>::iterator iter = m_ThreadInitCond.begin(); iter != m_ThreadInitCond.end(); ++iter ) 
	{
		pthread_cond_destroy(*iter);
	}	
	m_ThreadInitCond.clear();
	
	for (std::vector<pthread_cond_t *>::iterator iter = m_ThreadRunCond.begin(); iter != m_ThreadRunCond.end(); ++iter ) 
	{
		pthread_cond_destroy(*iter);
	}	
	m_ThreadRunCond.clear();
	

	
	if (m_numLocalThreads)
	{
		delete [] m_initFunctors;
		delete [] m_runFunctors;
		
		TaskManager::unregisterTask(this);
	}
	
	//std::cout << "End Destroying task on process rank:" << m_processRank <<std::endl;
	}
	catch(...)
	{
		std::cerr << "End Destroying task on process rank:" << m_processRank <<std::endl;
	}
}


template<typename T>
inline void Task<T>::init() throw(Exception)
{
	try{

		if (m_numLocalThreads)
		{
			//counterWait(m_InitReadyCounter);
			
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func.at(rank)));
			}
			initImpl();
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}


template<typename T>
template<typename T1>
void Task<T>::init( T1 t1 ) throw(Exception)
{
	try{

		if (m_numLocalThreads)
		{
//			pthread_mutex_lock(&m_CountMutex);
//			pthread_cond_wait(&m_InitCond, &m_CountMutex);
			
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func.at(rank)),t1);
			}
			initImpl();
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2>
void Task<T>::init ( T1 t1, T2 t2 ) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2);
			}			
			initImpl();
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

template<typename T>
template<typename T1, typename T2, typename T3>
void Task<T>::init( T1 t1, T2 t2, T3 t3 ) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3);
			}	

			initImpl();
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4 ) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3, t4);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4, typename T5>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4, T5 t5) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3, t4,t5);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4, T5 t5, T6 t6) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3,t4,t5,t6);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4, typename T5,
			typename T6, typename T7>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4, T5 t5, T6 t6,T7 t7) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3,t4,t5,t6,t7);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4, typename T5,
			typename T6, typename T7, typename T8>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4, T5 t5, T6 t6,T7 t7,T8 t8) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3,t4,t5,t6,t7,t8);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
template<typename T1, typename T2, typename T3, typename T4, typename T5,
			typename T6, typename T7, typename T8, typename T9>
void Task<T>::init( T1 t1, T2 t2, T3 t3,T4 t4, T5 t5, T6 t6,T7 t7,T8 t8,T9 t9) throw(Exception)
{
	try{
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::init,boost::ref(m_func[rank]),t1,t2,t3,t4,t5,t6,t7,t8,t9);
			}	
			initImpl();			
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}

}

template<typename T>
inline
int Task<T>::initImpl()  
{

	//Tell the threads to perform the initialization
	//int rc = broadcast(m_InitCond);
	int rc = broadcast(m_ThreadInitMutexes, m_ThreadInitCond);

	//The following forces init to block till all the inits are complete
	if (m_numLocalThreads)
	{
		counterWait(m_InitCompleteCounter);
	}
	
	//Unlock the InitMutex. This enables the run to get going
	pthread_mutex_unlock(&m_InitMutex);
	
	return rc;

}


///Init and Run the task using function call operator
template<typename T>
inline
void Task<T>::operator()() throw(Exception)
{
	try{
		m_isRunnable = false;
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::operator(),boost::ref(m_func[rank]));
				operatorImpl(rank);				
			}	

		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}



}

///Init and Run the task using function call operator
template<typename T>
template<typename T1>
void Task<T>::operator()( T1 t1 ) throw(Exception)
{
	//int rc = 0; //unused variable
	try{
		m_isRunnable = false;
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::operator(),boost::ref(m_func[rank]),t1);
				operatorImpl(rank);
			}	
	
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

///Init and Run the task using function call operator
template<typename T>
template<typename T1, typename T2>
void Task<T>::operator() ( T1 t1, T2 t2 ) throw(Exception)
{
	try{
		m_isRunnable = false;
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::operator(),boost::ref(m_func[rank]),t1,t2);
				operatorImpl(rank);	
			}	
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

///Init and Run the task using function call operator
template<typename T>
template<typename T1, typename T2, typename T3>
void Task<T>::operator()( T1 t1, T2 t2, T3 t3 ) throw(Exception)
{
	try{
		m_isRunnable = false;
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::operator(),boost::ref(m_func[rank]),t1,t2,t3);
				operatorImpl(rank);	
			}	
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

///Init and Run the task using function call operator	
template<typename T>
template<typename T1, typename T2, typename T3, typename T4>
void Task<T>::operator()( T1 t1, T2 t2, T3 t3,T4 t4 ) throw(Exception)
{
	try{
		m_isRunnable = false;
		if (m_numLocalThreads)
		{
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_initFunctors[rank] = boost::bind(&T::operator(),boost::ref(m_func[rank]),t1,t2,t3,t4);
				operatorImpl(rank);	
			}	
			//operatorInit();
			//operatorRun();
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
}

template<typename T>
inline
void Task<T>::operatorImpl(int rank)
{	
	int rc;
	pthread_mutex_lock(m_ThreadInitMutexes[rank]);
	rc = pthread_cond_signal(m_ThreadInitCond[rank]);
	pthread_mutex_unlock(m_ThreadInitMutexes[rank]);
}

template<typename T>
inline
void Task<T>::operatorInit()
{	

	for (int rank=0;rank < m_numLocalThreads; ++rank)
	{
		pthread_mutex_lock(m_ThreadInitMutexes[rank]);
		pthread_cond_signal(m_ThreadInitCond[rank]);
		pthread_mutex_unlock(m_ThreadInitMutexes[rank]);
	}
	pthread_mutex_unlock(&m_InitMutex);
}

template<typename T>
inline
void Task<T>::operatorRun()
{	

	for (int rank=0;rank < m_numLocalThreads; ++rank)
	{	
		pthread_mutex_lock(m_ThreadRunMutexes[rank]);
		pthread_cond_signal(m_ThreadRunCond[rank]);
		pthread_mutex_unlock(m_ThreadRunMutexes[rank]);
	}
}

template<typename T>
inline
int Task<T>::broadcast(std::vector<pthread_mutex_t *>& mutexVector, std::vector<pthread_cond_t *>& condVector)  
{

	if (m_numLocalThreads == 0)
		return 0;
	int rc = 0;
	try {
		//Wait to all threads to get in a state to be broadcasted to
		std::vector<pthread_mutex_t *>::iterator iter;
		int i = 0;
		for (iter = mutexVector.begin(); iter != mutexVector.end(); ++iter ) 
		{
			pthread_mutex_lock(*iter);
			rc = pthread_cond_signal(condVector[i++]);
			pthread_mutex_unlock(*iter);
		}		
	
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}
	return rc;
}


//This should be redone using condition variables and while loop in constructor
template<typename T>
inline 
void Task<T>::counterWait(int& counter) 
{
	while (counter < m_numLocalThreads)
	{
		usleep(100);
	}
}

template<typename T>
inline
int Task<T>::run() throw(Exception) 
{
	int rc =0;
	
	
	try{
		if (m_numLocalThreads && m_isRunnable)
		{

#ifdef USE_RUN_FUNCTORS
			for (int rank=0;rank < m_numLocalThreads; ++rank)
			{
				m_runFunctors[rank] = boost::bind(&T::run,boost::ref(m_func.at(rank)));
			}			
#else
			m_runFunctionPtr = &T::run ;
#endif
			pthread_mutex_lock(&m_InitMutex);

			rc = broadcast(m_ThreadRunMutexes,m_ThreadRunCond);
		}
	}
	catch (Exception& ex) { throw ex; }
	catch (...){ throw Exception("Unknown Error");}



	return rc;
}



//! Wait for task. 
template<typename T>
void Task<T>::waitTillDone() throw(Exception) {    
	
#ifdef PVTOL_DEBUG
	pthread_mutex_lock(&m_DebugMutex);
	std::cout << "Waiting on process rank: " << m_processRank << " Task Id: " << m_Tid << std::endl;
	pthread_mutex_unlock(&m_DebugMutex);
#endif // PVTOL_DEBUG


	//Wait for threads to finish
	std::vector<pthread_t>::const_iterator iter;
	for (iter = m_Threads.begin(); iter != m_Threads.end(); ++iter ) 
	{
		pthread_join(*iter, NULL);
	}
	//Now OK the destruction of this object
	pthread_mutex_unlock(&m_DestructionMutex);
	
//	Barrier b;
//	b.synch();
}


template<typename T>
inline
void Task<T>::LaunchThreads()
{
	pthread_t tid;
	ThreadId id;
	std::vector<ProcId>::iterator iter;
	RankId localRank = 0;

	
	for (iter = m_globalRanks.begin(); iter != m_globalRanks.end(); ++iter ) 
	{
		int rc = pthread_create(&tid, NULL, runImpl,(void*) this);
		if (rc){
			Exception ex("Unable to launch Task Thread");
			throw ex;
		}
		else
		{
			m_Threads.push_back(tid);

			id = ThreadManager::getThreadId(tid);
			m_ThreadRegistry.insert(std::pair<ThreadId,RankId>(id,localRank++));
		}
	}

	//std::cout << "Finished Launching threads for Task " << m_Tid <<std::endl;
	

}

template<typename T>
void* Task<T>::runImpl(void* arg)
{
	try {
	//The arg points to a Task<T> object
	Task<T>* obj = (Task<T>*) arg;

	//Wait for the Task object to be created 
	pthread_mutex_lock(&(obj->m_CreationMutex));

	//Create a TaskInfo object to place in TSS
	TaskInfo* taskInfoPtr = new TaskInfo();	
	taskInfoPtr->taskId = obj->m_Tid;
	
	taskInfoPtr->parentTaskId = obj->m_ParentTid;
	taskInfoPtr->self=pthread_self();
	taskInfoPtr->threadId = ThreadManager::getThreadId(taskInfoPtr->self);
	taskInfoPtr->localRank = obj->m_ThreadRegistry[taskInfoPtr->threadId]; 
	int localRank = taskInfoPtr->localRank;
	taskInfoPtr->globalRank = obj->m_globalRanks[localRank];

	//Place it in TSS
	//	std::cerr << "Setting the following values" <<std::endl;
	//	std::cerr << *taskInfoPtr;
	int rc = pthread_setspecific(TaskManager::getTaskInfoKey(),(void *) taskInfoPtr);
	if (rc)
		std::cerr << "Error in setting the TSS: rc=" <<rc <<std::endl;

#ifdef PVTOL_DEBUG 
	    pthread_mutex_lock(&(obj->m_DebugMutex));
		TaskInfo* taskInfoTestPtr = new TaskInfo();
		taskInfoTestPtr = (TaskInfo* ) pthread_getspecific(TaskManager::getTaskInfoKey());
		std::cerr << "Getting the following values to make sure it was set correctly" <<std::endl;
		std::cerr << *taskInfoTestPtr;
		pthread_mutex_unlock(&(obj->m_DebugMutex));
#endif
	//Create the function object if it was not created while constructing the Task
	if (! (obj->m_func[localRank]))
		obj->m_func[localRank] = new T;
	//obj->m_funcPtr[localRank] = new T;
	
	obj->incrementThreadCounter(obj->m_InitReadyCounter);
	 
	//Release the mutex to let other threads do their work
	pthread_mutex_unlock(&(obj->m_CreationMutex));
	
	
	//wait for init cond
	//std::cerr << "Thread " << obj << " waiting to init m_func:" <<std::endl;

	
//	pthread_cond_wait(obj->m_ThreadInitCond[obj->m_ThreadRegistry[pthread_self()]],
//			         (obj->m_ThreadInitMutexes[obj->m_ThreadRegistry[pthread_self()]]));
	
	pthread_cond_wait(obj->m_ThreadInitCond[localRank],
			         (obj->m_ThreadInitMutexes[localRank]));
	
	
	//Initialize the object
	obj->m_initFunctors[localRank]();


	
	if ((obj->m_isRunnable))
	{
		//Signal that the initialization is complete
		obj->incrementThreadCounter(obj->m_InitCompleteCounter);
		
		//wait for run method to be called

		pthread_cond_wait(obj->m_ThreadRunCond[localRank],(obj->m_ThreadRunMutexes[localRank]));


	
		//std::cerr << "Thread" << obj << "entering m_func:" <<std::endl;
		//use mpl or something else to do this switch
		//was (obj->m_func[localRank])->run();
	
		
#ifdef USE_RUN_FUNCTORS
		if (obj->m_runFunctors[localRank])
			obj->m_runFunctors[localRank]();
#else
		if 	(obj->m_runFunctionPtr)
			CALL_MEMBER_FN(*(obj->m_func[localRank]), (obj->m_runFunctionPtr))();
#endif //USE_RUN_FUNCTOR
	

		pthread_mutex_unlock(obj->m_ThreadInitMutexes[localRank]);
		pthread_mutex_unlock(obj->m_ThreadRunMutexes[localRank]);	
	}
	//std::cerr << "Thread" << obj << "finished m_func:" <<std::endl;
	return NULL;
	
	}
	catch(std::exception& ex )
	{
		std::cerr << "Error: Exception in runImpl" << ex.what() << std::endl;
		return NULL;
	}
}

template<typename T>
void Task<T>::incrementThreadCounter(int& counter)
{
	pthread_mutex_lock(&m_CountMutex);
	counter++;

//	if (counter == m_Threads.size())
//	{
//		std::cerr << "Process " << m_processRank << " counter complete" << std::endl;
//		pthread_cond_signal(&m_CountCond);
//	}
	pthread_mutex_unlock(&m_CountMutex);
}


template <typename T>
ostream& Task<T>::display (ostream& output) const
{
#ifdef PVTOL_DEVELOP
	pthread_mutex_lock(&m_DebugMutex);
#endif
	output <<
	"Name: " << m_Name <<
	", ProcessRank,: "<< m_processRank <<
	", NumLocalThreads: " << m_numLocalThreads << 
	endl;
	
	if (m_numLocalThreads >0)
	{
		output << "Note: Global ranks are global ranks with respect to the parent task" <<endl;
		output << "GlobalRanks: { ";
		std::vector<ProcId>::const_iterator iter;
		for ( iter = m_globalRanks.begin(); iter != m_globalRanks.end(); ++iter ) 
		{
			output << *iter << "  " ;
		}
		output << "}" <<endl;
	}
	
#ifdef PVTOL_DEVELOP
	pthread_mutex_unlock(&m_DebugMutex);
#endif

	return output;

}

template <typename T>
inline
bool Task<T>::isLocal(RankId rank)
{
	//if the Taskmap has absolute ranks use the following line
	//return (m_processRank == rank);
	
	//if they are local ranks then do the following
	TaskInfo info = TaskManager::getTaskInfo();
	RankId globalRank = info.globalRank;
	return (globalRank == rank);

}

template <typename T>
inline
bool Task<T>::isTaskRankLocal(RankId taskRank)
{
	bool result = false;
	std::vector<ProcId>::const_iterator iter;
	for ( iter = m_globalRanks.begin(); iter != m_globalRanks.end(); ++iter ) 
	{
		if (taskRank == *iter)
			return true;
	}
	
	return result;
}

template <typename T>
template <typename R> 
R Task<T>::apply (boost::function< R (T*)> fapply , RankId taskRank) throw (Exception)
{
	if (m_numLocalThreads)
	{
		try{
				if (isTaskRankLocal(taskRank))
				{
					return boost::bind(fapply,boost::ref(m_func.at(taskRank)))();
				}
				//return (m_func.at(rank))->getResult(result);
		}
		catch (Exception& ex) { throw ex; }
		catch (...){ throw Exception("Unknown Error");}

	}
	R r;
	return r;	
}

template <typename T>
template <typename R> 
std::vector<R> Task<T>::apply (boost::function< R (T*)> fapply ) throw (Exception)
{
	vector<R> result;
	if (m_numLocalThreads)
	{

		try{
			for ( int rank = 0; rank < m_numLocalThreads; ++rank ) 
			{
				result.push_back( boost::bind(fapply,boost::ref(m_func.at(rank)))());
			}
			return result;
				//return (m_func.at(rank))->getResult(result);
		}
		catch (Exception& ex) { throw ex; }
		catch (...){ throw Exception("Unknown Error");}
		
	}	
	else 
	{
		return result;
	}
}
template <typename T>
template<typename R> 
R  Task<T>::reduce (boost::function< R (T*)> fapply, boost::function< R (R,R)> freduce, R initialValue) 
//R  Task<T>::reduce (boost::function< R (T*)> fapply, R (*freduce) (R,R), R initialValue) 
	throw (Exception)
{
	R result = 0;
	
	if (m_numLocalThreads)
	{
		result = initialValue;
		std::vector<R> vec = apply<R>(fapply);
		
		for ( int rank = 0; rank < m_numLocalThreads; ++rank )
		{
			result = freduce(vec[rank],result);
		}
	}
	return result;
}


template <typename T>
const T* Task<T>::operator[]( RankId taskRank) const throw(Exception)
{
	T* result = 0;
	int localRank=0;
	if (m_numLocalThreads)
	{
		try
		{
			for (localRank=0; localRank < m_numLocalThreads; ++localRank) 
			{
				if (taskRank == m_globalRanks.at(localRank))
				{
					result = m_func.at(localRank);
					break;
				}
			}	
			
			return m_func.at(localRank);

		}
		catch (Exception& ex) { throw ex; }
		catch (...){ throw Exception("Unknown Error");}
	}
	
	return result;
}

template <typename T>
inline
const T* Task<T>::operator->() const throw(Exception)
{
	const int localRank=0;
	if (m_numLocalThreads)
	{
		return m_func.at(localRank);
	}
	else
	{
		throw Exception("Attempt to deference a non-existent function object.");
	}	
}

