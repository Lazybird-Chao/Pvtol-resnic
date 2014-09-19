/**
 * File: CommScope.cc
 *    December 2007 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of the CommScope class.
 *            A CommScope object contains communications information used to
 *	        give scope to a set of communications. When MPI is the basis
 *	        of Pvtol's communications the major info is: a Communicator
 *              and the ProcIds of its members
 *
 *  $Id: CommScope.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <CommScope.h>
#include <TaskBase.h>
#include <PvtolProgram.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#define noPVTOL_DEBUG
#define no_DEBUG_1

namespace ipvtol
{
  bool CommScope::m_firstConstructed = false;

/**
 *                 CommScope(int numNodes)
 *
 * \brief Constructor for the first CommScope, the one with scope
 *          over "the world"
 * \param int - the number of nodes in the world.
 * \return void
 */
CommScope::CommScope(int worldSize)
  {

    if (m_firstConstructed)
      {
	throw Exception(
	     "Only the first CommScope can be constructed without a Map!",
	     __FILE__, __LINE__);
      }
    else
      {
	m_firstConstructed = true;
      }

#ifdef _STANDARD_MPI
    MPI_Comm tmp = MPI_COMM_WORLD;
#endif // _STANDARD_MPI
    const CommType* parentComm = &tmp;
    vector<ProcId> parentCommProcs;
    for (int i = 0; i < worldSize; i++)
      {
	m_commProcsLongList.push_back(i);
	m_commProcsShortList.push_back(i);
	parentCommProcs.push_back(i);
      }

    constructCommon(parentComm, parentCommProcs);
  }//end construct


/**
 *                 CommScope(const Map& map)
 *
 * \brief Constructor for Constructor for all CommScopes except the first.
 * \param const Map& - Map providing the Procs that the Commm is scoped over
 * \return void
 */
CommScope::CommScope(const Map& theMap)
  {
    int i, j;

    if (!m_firstConstructed)
      {
	throw Exception("Must construct the first CommScope without a Map!", 
			__FILE__, __LINE__);
      }

    theMap.getRankList(m_commProcsLongList);
    int longSize = m_commProcsLongList.size();
    int shortSize;

    for (i=0; i<longSize; i++) {
       shortSize = m_commProcsShortList.size();
       for (j=0; j<shortSize; j++) {
         if (m_commProcsShortList[j] == m_commProcsLongList[i])
	     break;
       }//endFor j

       if (j == shortSize)
            m_commProcsShortList.push_back(m_commProcsLongList[i]);
    }//endFor i

    //PvtolProgram  pvtol;
    //In the consructor The current task is the parent task
    //TaskBase&  ptif = pvtol.getCurrentTask(); //was pvtol.getParentTask();
    TaskBase&  ptif = TaskManager::getCurrentTask();
    
#ifdef PVTOL_DEBUG
    TaskId myParentTaskId =  TaskManager::getCurrentTaskId();
    cout << "CS Parent task id=" << myParentTaskId << endl;
#endif // PVTOL_DEBUG
    
    CommType       tmpComm;
    vector<ProcId> parentCommProcs;
    ptif.getRankList(parentCommProcs);
    //ptif.getProcesses(parentCommProcs);
    CommScope& parentCommScope = ptif.getCommScope();
    tmpComm = parentCommScope.comm();

    constructCommon(&tmpComm, parentCommProcs);

    return;
  }//end construct


/**
 *                 constructCommon(const CommType* parentComm,
 *                                 const vector<ProcId>& parentCommProcs)
 *
 * \brief part of construction which is common to all constructors
 *             note: the m_commProcsShortList vector is expected to already
 *                   contain the set of PocIds in this scope
 * \param const CommType* parentComm, parent's Comm object
 * \param const vector<ProcId>& parentCommProcs, parent's ProcId list
 * \return void
 */
void CommScope::constructCommon(const CommType* parentComm,
				const vector<ProcId>& parentCommProcs)
  {
    int             i, j;

    m_tag = 0;
 
#ifdef _STANDARD_MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &m_pid);
#endif // _STANDARD_MPI

    int  sizeofParent = parentCommProcs.size();
    int  sizeofNewComm = m_commProcsShortList.size();
    int  *ranks = new int[sizeofNewComm];
    m_pRank = -1;

#ifdef PVTOL_DEBUG
    cout << m_pid << ": CS sizeofNewComm=" << sizeofNewComm
         << ", sizeofParent=" << sizeofParent
	 << endl;
#endif // PVTOL_DEBUG

    for (i=0; i<sizeofNewComm; i++) {
      if (m_commProcsShortList[i] == m_pid)
        {
	  m_pRank = i;
        }
      for (j=0; j<sizeofParent; j++) {
	if (m_commProcsShortList[i] == j)
	  {
	    ranks[i] = j;
	    break;
	  }
      }//endFor each parent processor

      if (m_commProcsShortList[i] >= sizeofParent)
      {
	 throw Exception("CommScope rank list contains # gtr than sizeof parent",
	                 __FILE__, __LINE__);
      }

    }//endFor each proc in new communicator

#ifdef PVTOL_DEBUG
    cout << "CS[" << m_pid << "] new comm with ranks: ";
    for (int k=0; k<sizeofNewComm; k++)
                       cout << ranks[k] << ", ";
    cout << endl;
#endif // PVTOL_DEBUG

#ifdef _STANDARD_MPI
    MPI_Group parentGroup;
    MPI_Comm_group(*parentComm, &parentGroup);
    MPI_Group newGroup;
    MPI_Group_incl(parentGroup, sizeofNewComm, ranks, &newGroup);
    MPI_Comm_create(*parentComm, newGroup, &m_comm);
    MPI_Group_free(&newGroup);
#endif // _STANDARD_MPI

#ifdef PVTOL_DEBUG
    cout << "CS[" << m_pid << "] just built new mpiCOMM" << endl;
#endif // PVTOL_DEBUG
    delete[] ranks;
  }//end constructCommon


/**
 *                 rank(ProcId proc)
 *
 * \brief Determines the rank of a particular processor with
 *          respect to this CommScope.
 * \param ProcId - proc of interest
 * \return int indicating the rank of the Processor w.r.t. the
 *            CommScope. NOTE a negative number implies the ProcId is
 *            not within the CommScope.
 *
 *          Note: the rank returned is the lowest rank this proc occupies.
 *                a process may occupy several ranks in a task
 */
int CommScope::rank(ProcId proc) const
  {
    const int PROC_NOT_IN_COMMSCOPE = -1;
    int theRank = PROC_NOT_IN_COMMSCOPE;

    int size = m_commProcsShortList.size();
    for (int i=0; i<size; i++) {
      if (m_commProcsShortList[i] == proc)
	{
	 theRank = i;
	 break;
	}
    }//end for each proc in the CommScope

    return(theRank);
  }//end rank()

/**
 *                 getNextTag()
 *
 * \brief get the next available comm tag value.
 * \return int tag value
 *         Exception thown if tag exceeds max positive int
 */
int CommScope::getNextTag() throw(Exception)
  {
    int tag = m_tag++;
    if (m_tag < 0)
       throw Exception("MAX comm tag exceeded", __FILE__, __LINE__);

#ifdef _DEBUG_1
    PvtolProgram   prog;
    int procid   = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid     = ct.getTaskID();
    int threadid   = ct.getGlobalThreadRank();
    cout << "CS[" << procid << "," << taskid << "," << threadid
         << "], returning tag = " << tag
         << endl;
#endif

    return(tag);
  }

/**
 *                 returnTag(int tag)
 *
 * \brief return a tag, currently nothing is done
 * \param int - the comm tag being returned
 * \return void
 */
void CommScope::returnTag(int tag)
  {
    return;
  }


//------------------------------------------------------------------------
//  Method: destructor
//
//  Description: destroy the CommScope obj.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
/**
 *                 ~CommScope()
 *
 * \brief Default Destructor
 */
CommScope::~CommScope()
  {
      // make all the tags available
    m_tag = 0;

    if (m_comm != MPI_COMM_NULL)
      {
	MPI_Comm_free(&m_comm);
      }
  }//end destructor


//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   send()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
void CommScope::send(void *srcAddr, 
		     void *dstAddr,
		     int byteCount, 
		     int destRank, 
		     int tag) const
{
  bool doMemcpy = true;
  if (destRank != m_pRank)
    {
      doMemcpy = false;
      MPI_Send(srcAddr, byteCount, MPI_CHAR, destRank, tag, m_comm);
    }

  if (doMemcpy)
    {
      char* dst = static_cast<char*>(dstAddr);
      memcpy(dst, srcAddr, byteCount);
    }
}// end send()


//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
//                   isend()
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++*
void CommScope::isend(void *srcAddr, 
		      void *dstAddr,
		      int byteCount, 
		      int destRank,
		      int tag, 
		      PvtolRequest& req) const
{
  bool doMemcpy = true;
  if (destRank != m_pRank)
    {
      doMemcpy = false;
      MPI_Isend(srcAddr, 
		byteCount, 
		MPI_CHAR, 
		destRank, 
		tag, 
		m_comm, 
		req);
    }
  req.setIsLocal(doMemcpy);
  if (doMemcpy)
    {
      char* dst = static_cast<char*>(dstAddr);
      memcpy(dst, srcAddr, byteCount);
      static_cast<CopyRequest&>(req).m_isSrc = true;
    }
}// end isend()


void CommScope::recv(void *srcAddr, 
		     void *destAddr, 
		     int byteCount, 
		     int srcRank, 
		     int tag) const
{
  PvtolStatus stat;
  if (srcRank != m_pRank)
    {
      MPI_Recv(destAddr, 
	       byteCount, 
	       MPI_CHAR, 
	       srcRank, 
	       tag, 
	       m_comm, 
	       stat);
    }
}// end recv()

void CommScope::irecv(void *srcAddr,
		      void *destAddr, 
		      int byteCount, 
		      int srcRank,
		      int tag, 
		      PvtolRequest& req) const
{
  bool doMemcpy = true;
  if (srcRank != m_pRank)
    {
      doMemcpy = false;
      MPI_Irecv(destAddr, 
		byteCount, 
		MPI_CHAR, 
		srcRank, 
		tag, 
		m_comm, 
		req);
    }
  req.setIsLocal(doMemcpy);
  if (doMemcpy)
    {
      CopyRequest& creq = req;
      creq.m_isSrc = false;
      creq.m_srcRank = srcRank;
      creq.m_tag = tag;
      creq.m_numBytes = byteCount;
    }
}// end irecv()


void CommScope::sendInit(void *srcAddr, 
			 void *dstAddr,
			 int byteCount, 
			 int destRank,
			 int tag, 
			 PvtolRequest& req) const
{
  bool doMemcpy = true;
  if (destRank != m_pRank)
    {
      doMemcpy = false;
      MPI_Send_init(srcAddr, 
		    byteCount, 
		    MPI_CHAR, 
		    destRank, 
		    tag, 
		    m_comm, 
		    req);
    }

  req.setIsLocal(doMemcpy);

  if (doMemcpy)
    {
      CopyRequest& creq = req;
      creq.m_dstAddr = static_cast<char*>(dstAddr);
      creq.m_isSrc = true;
      creq.m_srcAddr = srcAddr;
      creq.m_srcRank = m_pRank;
      creq.m_dstRank = destRank;
      creq.m_tag = tag;
      creq.m_numBytes = byteCount;
    }
}

void CommScope::recvInit(void *srcAddr,
			 void *destAddr, 
			 int byteCount, 
			 int srcRank,
			 int tag, 
			 PvtolRequest& req) const
{
  bool doMemcpy = true;

  if (srcRank != m_pRank)
    {
      doMemcpy = false;
      MPI_Recv_init(destAddr, 
		    byteCount, 
		    MPI_CHAR, 
		    srcRank, 
		    tag, 
		    m_comm, 
		    req);
    }
  req.setIsLocal(doMemcpy);
  if (doMemcpy)
    {
      CopyRequest& creq = req;
      creq.m_isSrc = false;
      creq.m_srcAddr = srcAddr;
      creq.m_dstAddr = destAddr;
      creq.m_srcRank = srcRank;
      creq.m_dstRank = m_pRank;
      creq.m_tag = tag;
      creq.m_numBytes = byteCount;
    }
}// end recvInit()

}//end Namspace

