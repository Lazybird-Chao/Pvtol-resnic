/**
 *  file: ThreadManager.inl
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Implementation of ThreadManager class
 *
 *  $Id: ThreadManager.inl 945 2009-02-24 21:28:21Z jav5880 $
 *
 *  Author: Sanjeev Mohindra
 */

namespace ipvtol{

inline
ThreadId ThreadManager::getThreadId(pthread_t pthId)
{
	/**

	 */
	///@todo: Remove this HACK final code This "Works" for All BSD,HP, and Windows systems where
	/// pthread_t is a struct instead of an unsigned long
#ifdef PTHREAD_IS_STRUCT
	return (*((unsigned long*)&pthId));
#else
	return static_cast<ThreadId> (pthId);
#endif
}


inline
ThreadId ThreadManager::getCurrentThreadId()
{
	return getThreadId(pthread_self());
}
	

}
