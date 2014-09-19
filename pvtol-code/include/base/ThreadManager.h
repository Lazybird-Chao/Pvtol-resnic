/**
 *    File: ThreadManager.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definition of the ThreadManager class.
 *  
 *  $Id: ThreadManager.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include <PvtolBasics.h>
#include <pthread.h>

//This is only in the base namespace
namespace ipvtol {

/**
 * Class Threadmanager currently provides the ThreadId associated with a pthread_t.
 * 
 */
class ThreadManager {

public:
	

	/**
	 * \brief    Get the ThreadID associated with the current 
	 * 
	 * \return   ThreadId Thread Id of the current thread
	 *
	 */
	static ThreadId getCurrentThreadId();

	/**
	 * \brief    Get the ThreadID associated with the Thread Identifier
	 * \param    pthread_t Thread Identifier
	 * 
	 * \return   ThreadId Thread Id
	 *
	 */
	static ThreadId getThreadId(pthread_t pthId = pthread_self());

private:

	///No copy constructor
	ThreadManager(const ThreadManager&);
	///No assignment operator
	ThreadManager& operator=(const ThreadManager&);

};

} //end namespace ipvtol

#include <ThreadManager.inl>

#endif /*THREADMANAGER_H_*/

