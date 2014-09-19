/**
 *    File: Barrier.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    February 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   The Barrier class provides application programmers with a
 *             synchronization primitive. This class implements a Barrier
 *             synchronization across a set of Task threads
 *
 *  $Id: Barrier.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <PvtolBasics.h>
#include <Map.h>
#include <vector>


namespace ipvtol
{

class Transfer;

  class Barrier
  {
  public:
    /**
     * The default constructor builds a Barrier over the current Task's
     * map.
     */
    Barrier();

    /**
     * The destructor reclaims the memory allocated to a Barrier object.
     */
    ~Barrier() throw();
    
    /**
     * The synch() method is used to execute a barrier synchronization. No 
     * process in the barrier's task will leave this function until all
     * processes in that task have reached this point in execution.
     * All processes in the task must call this method; other processes in
     * the Task may call or not call this method.
     */
    void synch() throw();
    	
  private:
    int                     *m_pCommBuffer;
    int                     *m_tCommBuffer;
    int                      m_numProcs;
    int                      m_numLocalThreads;
    int                      m_myThreadRank;
    int                      m_myCondMutexIdx;
    bool                     m_isFirstProc;
    bool                     m_isFirstThread;
	
    Barrier( const Barrier& other );
    Barrier& operator=( const Barrier& );
  };

}//end namespace

#endif // ifndef _BARRIER_H

