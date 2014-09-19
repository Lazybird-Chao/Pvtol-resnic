/**
 *    File: Task.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: 2009-08-12 13:26:09 -0400 (Wed, 12 Aug 2009) $
 *  \version $LastChangedRevision: 1144 $
 *  \brief   Definition of the Task class.
 *  
 *  \
 *  $Id: Task.h 1144 2009-08-12 17:26:09Z  $
 *
 */
#ifndef TASK_H_
#define TASK_H_

#include <PvtolBasics.h>
#include <PvtolProgram.h>
#include <CommScope.h>
#include <TaskBase.h>
#include <TaskInfo.h>
#include <TaskMap.h>
#include <HeterogeneousMap.h>
#include <TaskManager.h>
#include <Barrier.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#define noPVTOL_DEBUG
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

namespace ipvtol
{

//#ifdef UGLY
////This segment is a result solely of the coding rules. It is best defined inline
////{
//template <class T> class Task;
//
//template <class T>
//ostream& operator<< (ostream& output, const Task<T>& task);
//
////} of segment
//#endif

/**
* \brief Task Class
*/
template<class T>
class Task: public TaskBase {

	
public:
	
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC TYPES
    /////////////////////////////////////////////////////////////////////////
	
	///The function object type
	typedef T FunctionObjectType;
	
    /////////////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    /////////////////////////////////////////////////////////////////////////
	
	/**
	 * \brief   Task Constructor
	 * \param	name   Task name for Identification purposes
	 * \param	map		Task map
	 * \note	This constructor creates a separate copy of class T in each thread
	 *
	 */
    Task(const std::string name = "TASK", 
    	   const TaskMap& map = TaskMap(vector<int>(1,0), TaskDistDescription() ),
    	   const HeterogeneousMap& hMap = HeterogeneousMap()
    	) throw(Exception)	;
	
	/**
	 * \brief   Task Constructor
	 * \param	functionObject	prebuilt function object
	 * \param	name   			Task name for Identification purposes
	 * \param	map				Task map
	 * \note	This constructor used the supplied function object for each thread. This may not be thread safe. 
	 *          This has not been tested yet.	
	 * \todo	Test this constructor
	 */
    Task(T& functionObject, 
    		const std::string name = "TASK", 
    		const TaskMap& map = TaskMap(vector<int>(1,0), TaskDistDescription() ),
    	   const HeterogeneousMap& hMap = HeterogeneousMap()
    	) throw(Exception);
    
	///Destructor
    ~Task() throw() ;

    ///Initialize the task    
    void init() throw(Exception);
    
    ///Initialize the task
	template<typename T1>
	void init( T1 t1 ) throw(Exception); 
	
	///Initialize the task
	template<typename T1, typename T2>
	void init ( T1 t1, T2 t2 ) throw(Exception);

    ///Initialize the task
	template<typename T1, typename T2, typename T3>
	void init( T1 t1, T2 t2, T3 t3 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4>
	void init( T1 t1, T2 t2, T3 t3,T4 t4 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void init( T1 t1, T2 t2, T3 t3,T4 t4,T5 t5 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
				typename T6>
	void init( T1 t1, T2 t2, T3 t3,T4 t4,T5 t5,T6 t6 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
				typename T6, typename T7>
	void init( T1 t1, T2 t2, T3 t3,T4 t4,T5 t5,T6 t6,T7 t7 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
				typename T6, typename T7, typename T8>
	void init( T1 t1, T2 t2, T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8 ) throw(Exception);
	
    ///Initialize the task	
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
				typename T6, typename T7, typename T8, typename T9>
	void init( T1 t1, T2 t2, T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9 ) throw(Exception);
	
    ///Run the task
	
    int run() throw(Exception);
    
    ///Wait till the task is completed
	
    virtual void waitTillDone(/*int timeout */) throw(Exception);
    
    ///Init and Run the task using function call operator
    
    void operator()() throw(Exception);
    
    ///Init and Run the task using function call operator
	template<typename T1>
	void operator()( T1 t1 ) throw(Exception); 
	
	///Init and Run the task using function call operator
	template<typename T1, typename T2>
	void operator() ( T1 t1, T2 t2 ) throw(Exception);

	///Init and Run the task using function call operator
	template<typename T1, typename T2, typename T3>
	void operator()( T1 t1, T2 t2, T3 t3 ) throw(Exception);
	
	///Init and Run the task using function call operator	
	template<typename T1, typename T2, typename T3, typename T4>
	void operator()( T1 t1, T2 t2, T3 t3,T4 t4 ) throw(Exception);
	
	//For now let's insist on const T* as return value
	///Index operator provides access to the function object with a given task rank
	const T* operator[]( RankId taskRank) const throw(Exception);
	
	///Dereference operator provides access to the function object with local rank 0;
	const T* operator->() const throw(Exception);
	
	///Apply a nullary function returning type R to a specified task rank: Pass in a member function
	template<typename R>
	R apply (boost::function< R (T*)> fapply , RankId taskRank) throw (Exception); 
	
	///Apply a nullary function returning type R  to all ranks: Pass in a member function
	template<typename R> 
	std::vector<R>  apply (boost::function< R (T*)> fapply ) throw (Exception); 
	
	///Apply a nullary member function returning type R  to all ranks. Reduce the results using the second supplied function
	template<typename R> 
	R  reduce (boost::function< R (T*)> fapply, boost::function< R (R,R)> freduce, R initialValue = 0 ) throw (Exception); 
	

protected:
	
    /////////////////////////////////////////////////////////////////////////
    // PROTECTED TYPES
    /////////////////////////////////////////////////////////////////////////
		
	///Member Function Pointer Type for int(void)  
	typedef  int (T::*MemFnPtr)();
	
	///void(void) function object
	typedef boost::function<void()> InitFun;
	
	///int(void) function
	typedef boost::function<int()> RunFun;
	
    /////////////////////////////////////////////////////////////////////////
    // PROTECTED METHODS
    /////////////////////////////////////////////////////////////////////////
	
	///No copy constructor
	Task(const Task&);
	
	///No assignment operator
	Task& operator=(const Task&);
	
	///Implementation of init function
	int initImpl();
	
	///Thread based implementation of the run function
	static void* runImpl(void*);
	
	///Thread based implementation of the operator()
	void operatorImpl(int rank);
	
	///Thread based implementation of the operator()
	void operatorInit();
	
	///Thread based implementation of the operator()
	void operatorRun();
	
	///display function
	virtual ostream& display (ostream& output) const;
	
	
    /////////////////////////////////////////////////////////////////////////
    // PROTECTED MEMBERS
    /////////////////////////////////////////////////////////////////////////	

	///Vector to hold function object pointers
	std::vector<T*> m_func;
	
	///Not used yet
	T** m_funcPtr;

	///Pointer to void() function objects
	InitFun* m_initFunctors;

	///Pointer to int() function objects
	RunFun* m_runFunctors;

	///Member Function Pointer for int(void) 
	MemFnPtr  m_runFunctionPtr;
	
private:
	
    /////////////////////////////////////////////////////////////////////////
    // PRIVATE METHODS
    /////////////////////////////////////////////////////////////////////////	
	///Launch the threads, if needed
	void LaunchThreads();
	
	///Used to count the number of threads that have been created
	void incrementThreadCounter(int& counter);
	
	///Wait for all threads to be created 
	void counterWait(int& counter);
	
	
	
	///Broadcast condition variables to the threads waiting for them of the specified mutex
	int broadcast(std::vector<pthread_mutex_t *>& mutexVector, std::vector<pthread_cond_t *>& condVector);
	
	///Used to decide if a thread needs to be launched locally or not
	bool isLocal(RankId rank);

	///Is the taskRank local to this process
	bool isTaskRankLocal(RankId taskRank);
	
	///Create Method to create the Task (Named constructor)
	
    void CreateTask(const std::string name , 
    	 const TaskMap& map ,
    	 T* sharedFunctionObjectPtr  = 0
    	) throw(Exception);
	
	
    /////////////////////////////////////////////////////////////////////////
    // PRIVATE MEMBERS
    /////////////////////////////////////////////////////////////////////////	
	
	///Mutex Variable
    pthread_mutex_t m_Mutex;
    ///Count Mutex
    pthread_mutex_t m_CountMutex;
    ///Creation Mutex
    pthread_mutex_t m_CreationMutex;
    ///Destruction Mutex
    pthread_mutex_t m_DestructionMutex;
    
#if PVTOL_DEVELOP
    mutable pthread_mutex_t m_DebugMutex;
#endif
    
    ///Condition Variable to signal that object has been created
    pthread_cond_t m_CreationCond;
    
    ///Mutex for init()
    pthread_mutex_t m_InitMutex;
    
    ///Condition variable to wait for initialization
    pthread_cond_t m_InitCond;
    
    ///Condition variable to wait for run command
    pthread_cond_t m_RunCond;
    ///Condition variable to wait for a count to be reached
    pthread_cond_t m_CountCond;
    
	///LocalRank -> ThreadSpecific Mutex  for Init
	std::vector<pthread_mutex_t *> m_ThreadInitMutexes;
	
	///LocalRank -> ThreadSpecific Condition variables for Init
	std::vector<pthread_cond_t *> m_ThreadInitCond;
		
	///LocalRank -> ThreadSpecific Mutex  for run
	std::vector<pthread_mutex_t *> m_ThreadRunMutexes;
	///LocalRank -> ThreadSpecific Condition variables for run
	std::vector<pthread_cond_t *> m_ThreadRunCond;
	
	
	///Number of threads launched on the process in a task
	int m_NumThreadsLaunched;
	
	///Counter to count the number of threads that have been created and are ready for Initialization
	int m_InitReadyCounter;
	
	///Counter to count the number of threads that have been initialized
	int m_InitCompleteCounter;
	
	///Not used
	int m_RunReadyCounter;
	
	//If the operator() is used to invoke the Task then run cannot be called 
	bool m_isRunnable; /*default is true*/

	
};


#include <Task.inl>



} //end namespace


#endif /*TASK_H_*/
