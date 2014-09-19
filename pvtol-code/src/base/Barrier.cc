/**
 * File: Barrier.cc
 *    February 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of Barrier
 *            The Barrier class in PVTOL provides application programmers with
 *            a synchronization primitive. This class implements a Barrier
 *            synchronization across a set of threads specified when the
 *            Barrier is created. The synchronization is activated by a
 *            call to the synch() method.
 *     Algorithm used:
 *            First all threads in each process synch using a pthread
 *              condition variable.
 *            Then each process's thread rank 0 uses the Task's CommScope
 *              barSynch() method to synch across procs.
 *
 *  $Id: Barrier.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <Barrier.h>
#include <PvtolProgram.h>
#include <Transfer.h>
#include <iostream>
#include <time.h>

using std::cout;
using std::cerr;
using std::endl;

#define noPVTOL_BAR_DEBUG

namespace ipvtol
{

/**
 * \brief Default Constructor: Build a barrier that can synch over the
 *                      current Task
 */
  Barrier::Barrier()
  {
    PvtolProgram  prog;
    TaskBase& currTask      = prog.getCurrentTask();
    m_numProcs              = currTask.getNumProcesses();
    TaskBarrierData& tbData = currTask.getTaskBarrierData();
    m_numLocalThreads       = currTask.getNumLocalThreads();
    m_myThreadRank          = currTask.getLocalThreadRank();

    int tRank = currTask.getLocalThreadRank();
    if (tRank == 0)
       m_isFirstThread = true;
     else
       m_isFirstThread = false;

#ifdef PVTOL_BAR_DEBUG
    int mypid = prog.getProcId();
    cout << "Bar[" << mypid << ":" << m_myThreadRank
         << "] in construct() addr tbData "
         << &tbData << endl;
#endif // PVTOL_BAR_DEBUG

    if (m_numLocalThreads > 1)
      {//              alloc Thread Comm Buffer and Mutex
        pthread_mutex_lock(&(tbData.tbdMutex));
	if ((!m_isFirstThread) &&
	   (tbData.rankInit.size() != static_cast<unsigned int>(m_numLocalThreads-1)))
	  {//               still some initing todo.
	    unsigned int i;
	    for (i=0; i<tbData.rankInit.size(); i++) {
              if (tbData.rankInit[i] == m_myThreadRank)
	        {
		    m_myCondMutexIdx = i;
	            break;
		}
	    }//end For i

	    if (i >= tbData.rankInit.size())
	      {//     we have not inited yet
#ifdef PVTOL_BAR_DEBUG
                cout << "Bar[" << mypid << ":" << m_myThreadRank
                     << "] in construct() Initing myidx="
                     << m_myCondMutexIdx << endl;
#endif // PVTOL_BAR_DEBUG

	        tbData.rankInit.push_back(m_myThreadRank);
		m_myCondMutexIdx = tbData.condMutex.size();
		pthread_mutex_t *theMutex = new pthread_mutex_t;
		pthread_mutex_init(theMutex, NULL);
		tbData.condMutex.push_back(theMutex);
		pthread_mutex_lock(tbData.condMutex[m_myCondMutexIdx]);
		tbData.readyIndicator++;
	      }//endIf init
	  }
	 else
	  {
	    for (unsigned int i=0; i<tbData.rankInit.size(); i++) {
              if (tbData.rankInit[i] == m_myThreadRank)
	        {
		    m_myCondMutexIdx = i;
	            break;
		}
	    }//end For i
	  }//endIf initing is done

        pthread_mutex_unlock(&(tbData.tbdMutex));
      }//end multi-thread

    int pRank = currTask.getProcessRank();
    if (pRank == 0)
       m_isFirstProc = true;
     else
       m_isFirstProc = false;

#ifdef PVTOL_BAR_DEBUG
    string isFirThrd;
    string isFirProc;
    if (m_isFirstThread)
        isFirThrd = " 1st Thread ";
     else
        isFirThrd = " NOT 1st Thread ";

    if (m_isFirstProc)
        isFirProc = " 1st Proc ";
     else
        isFirProc = " NOT 1st Proc ";

    cout << "Bar: " << pRank << " #procs = " << m_numProcs
         << " # loc Thrds = " << m_numLocalThreads
	 << " my thrd Rank = " << m_myThreadRank
	 << isFirThrd << isFirProc
         << endl;
#endif // PVTOL_BAR_DEBUG

  }//end Constructor

  Barrier::~Barrier() throw()
  {//   TODO
    return;
  }

  void Barrier::synch() throw()
  {
    PvtolProgram  prog;
    TaskBase& currTask      = prog.getCurrentTask();
    TaskBarrierData& tbData = currTask.getTaskBarrierData();
    CommScope& comm         = currTask.getCommScope();
    int numOtherThreads     = m_numLocalThreads - 1;

#ifdef PVTOL_BAR_DEBUG
    int mypid = prog.getProcId();
    cout << "Bar[" << mypid << ":" << m_myThreadRank
         << "] in synch() addr tbData "
         << &tbData << endl;
#endif // PVTOL_BAR_DEBUG
    if (m_numLocalThreads > 1)
      {
        if (m_isFirstThread)
	  {
	    // while (tbData.rankInit.size() != m_numLocalThreads) {
	    while (tbData.readyIndicator != numOtherThreads) {
#ifdef PVTOL_BAR_DEBUG
	      cout << "Barrier synch() - but All threads not ready!"
	           << endl;
#endif // PVTOL_BAR_DEBUG
	      struct timespec    sleepTime;
	      sleepTime.tv_sec  = 0;
	      sleepTime.tv_nsec = 5000000;// sleep at least 5 ms
	      nanosleep(&sleepTime, NULL);
	    }//endWhile

	    for (int i=0; i<numOtherThreads; i++) {
		pthread_mutex_lock(tbData.condMutex[i]);
	    }//endFor each thread

            pthread_mutex_lock(&(tbData.tbdMutex));
	    tbData.readyIndicator = 0;
            pthread_mutex_unlock(&(tbData.tbdMutex));
	  }
	 else
	  {
	    pthread_cond_wait(&(tbData.barrCond),
	                      tbData.condMutex[m_myCondMutexIdx]);

#ifdef PVTOL_BAR_DEBUG
            cout << "Bar[" << mypid << ":" << m_myThreadRank
                 << "] out of CondWait"
                 << ", idx=" << m_myCondMutexIdx
		 << " MuAddr=" << tbData.condMutex[m_myCondMutexIdx] << endl;
#endif // PVTOL_BAR_DEBUG

            pthread_mutex_lock(&(tbData.tbdMutex));
	    tbData.readyIndicator++;
            pthread_mutex_unlock(&(tbData.tbdMutex));
	  }
      }//end Multi Local Threads

    if (m_isFirstThread)
      {//              alloc Proc Comm Buffer and Mutex
        if (m_numProcs > 1)
	  {
#ifdef PVTOL_BAR_DEBUG
	    cout << "BAR:" << prog.getProcId() << " about to call barSynch"
	         << endl;
#endif // PVTOL_BAR_DEBUG
	    comm.barSynch();
	  }

        if (m_numLocalThreads > 1)
          {
	    // pthread_cond_broadcast(&(tbData.barrCond));
	    for (int i=0; i<numOtherThreads; i++) {
#ifdef PVTOL_BAR_DEBUG
                cout << "Bar[" << mypid << ":" << m_myThreadRank
                    << "] about UnLockMu Addr="
	   	    << tbData.condMutex[i] << endl;
#endif // PVTOL_BAR_DEBUG
		pthread_mutex_unlock(tbData.condMutex[i]);
	    }//endFor each thread

	    for (int i=0; i<numOtherThreads; i++)
	              pthread_cond_signal(&(tbData.barrCond));
	  }
      }//end Thread rank 0 and multi-thread

    return;
  }//end synch()

}//end Namspace

