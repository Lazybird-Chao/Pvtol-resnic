/**
 * File: Transfer.cc
 *    January 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline and non-templated methods of Transfer
 *    A Transfer object is used to transfer data from a source Block's local
 *     buffer to a destination Block's buffer which may be local or remote.
 *     Transfer also provides a means to broadcast a source Block's local
 *     buffer to all the nodes a destination Block is mapped onto.
 *
 * See Also: TransferTmpl.cc for templated parts of code
 *
 *  $Id: Transfer.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <Transfer.h>
#include <TaskBase.h>
#include <PvtolRequest.h>
#include <SendRequest.h>
#include <PvtolProgram.h>

#include <time.h>
#include <string.h>
#include <iostream>
#include <map>
using std::cout;
using std::endl;
using std::map;

namespace ipvtol
{

//------------------------------------------------------------------------
//  Method:     Constructor
//    Transfer(ProcId srcProc,  char *srcAddr,
//	       ProcId destProc, char *destAddr,
//	       int    countOfBlockTypes);
//
//  Description: Constructor
//
//  Inputs:
//
//------------------------------------------------------------------------
Transfer::Transfer(int srcRank,  char *srcAddr,
		   int destRank, char *destAddr,
		   int    byteCount, Flags flags) :
		   m_trait(NULL_TRAIT),
		   m_eltSize(1),
		   m_recvdCount(0)
  {
     // PvtolProgram  pvtol;
     // cout << "Trans[" << pvtol.getProcId() << "] in construct()" << endl;
     constructHelper(srcRank, srcAddr, destRank, destAddr, byteCount, flags);
     return;
  }//end construct()

//------------------------------------------------------------------------
//  Method:     constructHelper()
//    Transfer(ProcId srcProc,  char *srcAddr,
//	       ProcId destProc, char *destAddr,
//	       int    byteCount);
//
//  Description: Constructor
//
//  Inputs:
//
//------------------------------------------------------------------------
void Transfer::constructHelper(int srcRank,  char *srcAddr,
		               int destRank, char *destAddr,
		               int byteCount, Flags flags)
  {
    PvtolProgram pvtol;
    TaskBase  &ct  = pvtol.getCurrentTask();
    CommScope &cs  = ct.getCommScope();
    m_commScope    = &cs;
    //ProcId myProc  = pvtol.getProcId(); //unused Variable
    //int myProcRank = cs.rank(myProc);

    m_srcAddr  = static_cast<void *>(srcAddr);
    m_destAddr = static_cast<void *>(destAddr);

    m_myRank   = ct.getGlobalThreadRank();
    m_srcRank  = srcRank;
    m_destRank = destRank;

    m_srcProc      = cs.rankToProcId(srcRank);
    m_destProc     = cs.rankToProcId(destRank);
    m_srcProcRank  = cs.rank(m_srcProc);
    m_destProcRank = cs.rank(m_destProc);

    m_isDest = (destRank == m_myRank);
    m_isSrc  = (srcRank == m_myRank);

    m_sendCount = byteCount;
    m_sendSize  = m_sendCount;

#ifdef XFER_DEBUG
    if (m_isDest)
     {
       cout << "XFER:" << m_myRank << " this is Dest"
	    << " Src=" << m_srcProc << " Rcvr=" << m_destProc
	    << " xfer-size=" << m_sendSize
            << endl;
     }

    if (m_isSrc)
     {
       cout << "XFER:" << m_myRank << " this is Src"
	    << " Src=" << m_srcProc << " Rcvr=" << m_destProc
            << endl;
     }
#endif // XFER_DEBUG

    if (m_destProcRank == m_srcProcRank)
          m_tag = -1;// tag Not needed
     else
          m_tag = ct.getTransferTag(ct.getLocalThreadRank());

//   get next key value for interThread comm (just in case),
//       must be done SPMD
    m_itxdKey = ct.getTransferKey(ct.getLocalThreadRank());

//   Setup the indirect send function
//  ============================================================*
    if ((!m_isSrc && !m_isDest)   ||
	((m_isSrc && m_isDest) && (m_srcAddr == m_destAddr))
       )
      {//         No data movement required locally
	m_trait = NO_LOCAL_DATA_MOVEMENT;
      }
     else if (m_isSrc && m_isDest)
      {//       memcpy will suffice
	m_trait = DATA_MOVEMENT_IS_THREAD_LOCAL;
      }
     else if (m_srcProc == m_destProc)
      {//       in the same process but different threads
	m_trait = DATA_MOVEMENT_IS_PROC_LOCAL;

	// Need to setup a data entry for this xfer. First one
	//   in constructs the entry.
	// Src end grabs the entry's mutex
	// First grab mutex guarding DB
#ifdef PVTOL_DEBUG
        cout << "XFER:" << m_myRank << " my Key="
             << m_itxdKey << endl;
#endif // PVTOL_DEBUG
	pthread_mutex_t &itxdDbMutex = ct.getItxdDbMutex();
	pthread_mutex_lock(&itxdDbMutex);
	map<int, InterThreadXferData>& itxdDb = ct.getItxdDb();
	map<int, InterThreadXferData>::iterator it;
	it = itxdDb.find(m_itxdKey);
	if (it == itxdDb.end())
	  {//   Create and init entry
	     pthread_mutex_init(&(itxdDb[m_itxdKey].itxdMutex), NULL);
	     pthread_mutex_init(&(itxdDb[m_itxdKey].itxdCvMutex), NULL);
	     pthread_cond_init(&(itxdDb[m_itxdKey].itxdCond), NULL);
	     itxdDb[m_itxdKey].sendType   = NONE;
	     itxdDb[m_itxdKey].firstOneIn = false;
	     itxdDb[m_itxdKey].sReq       = NULL;
	     if (m_isSrc)
	       {
	         itxdDb[m_itxdKey].srcAddr  = m_srcAddr;
#ifdef PVTOL_DEBUG
                 cout << "XFER:" << m_myRank << " just set SrcAddr="
                      << m_srcAddr << endl;
#endif // PVTOL_DEBUG
	         itxdDb[m_itxdKey].destAddr = NULL;
		 pthread_mutex_lock(&(itxdDb[m_itxdKey].itxdCvMutex));
	       }
	      else if (m_isDest)
	       {//                    this is dest
	         itxdDb[m_itxdKey].destAddr = m_destAddr;
	         itxdDb[m_itxdKey].srcAddr  = NULL;
	       }
	  }
	 else
	  {//              entry already there
	     if (m_isSrc)
	       {
	         itxdDb[m_itxdKey].srcAddr  = m_srcAddr;
#ifdef PVTOL_DEBUG
                 cout << "XFER:" << m_myRank << " just set SrcAddr="
                      << m_srcAddr << endl;
#endif // PVTOL_DEBUG
		 pthread_mutex_lock(&(itxdDb[m_itxdKey].itxdCvMutex));
	       }
	      else
	       {//                    this is dest
	         itxdDb[m_itxdKey].destAddr = m_destAddr;
	       }
	  }//endIf entry there or Not
	pthread_mutex_unlock(&itxdDbMutex);
	m_itxdEntry = &(itxdDb[m_itxdKey]);
#ifdef PVTOL_DEBUG
        cout << "XFER:" << m_myRank << " itxdKey=" << m_itxdKey
	     << " itxdEntry addr=" << m_itxdEntry << endl;
#endif // PVTOL_DEBUG
      }
     else if (flags & STATIC)
      {
       m_trait = STATIC_TRANSFER;

       // Setup the persistant MPI send
       if (m_isSrc)
	 {
	   m_commScope->sendInit(m_srcAddr,
				m_destAddr,
				m_sendSize,
				m_destProcRank,
				m_tag,
				m_req);
	 }

       // Setup the persistant MPI Recv
       if (m_isDest)
	 {
	   m_commScope->recvInit(m_srcAddr,
				m_destAddr,
				m_sendSize,
				m_srcProcRank,
				m_tag,
				m_req);
	 }
      }
     else
      {
       m_trait = NON_STATIC_TRANSFER;

#ifdef PVTOL_DEBUG
       cout << "Xfer[" << myProc << "," << m_myRank
            << "]: construct TAG=" << m_tag << endl;
#endif // PVTOL_DEBUG
      }

    return;
  }//end constructHelper()


//------------------------------------------------------------------------
//  Method: send()
//
//  Description: Blocking send call
//
//  Inputs: 1: integer count of items to send
//          2: integer offset into the src block
//          3: integer offset into the dest block
//
//  Return: none
//
//------------------------------------------------------------------------
void Transfer::send(int count, int srcOff, int destOff)
  {
    if (m_trait == STATIC_TRANSFER)
	       staticSend();
     else if (m_trait == NON_STATIC_TRANSFER)
	       nonStaticSend(count, srcOff, destOff);
     else if (m_trait == DATA_MOVEMENT_IS_THREAD_LOCAL)
	       localCopy(count, srcOff, destOff);
     else if (m_trait == DATA_MOVEMENT_IS_PROC_LOCAL)
       {//          inter Thread Comm
         //  If this is the src
	 //     we already have the mutex
	 //     Wait on the condition variable
	 //   Else this is dest
	 //     Make sure the src has at least inited this Xfer
	 //     Wait for src by waiting on mutex, when the
	 //         src waits on condition the mutex is unlocked
	 //     Do a local copy
	 //     Signal source we are done and give src back its mutex
	 //  EndIf
#ifdef PVTOL_DEBUG
         cout << "XFER:" << m_myRank << " in Send() m_itxdEntry addr="
              << m_itxdEntry << endl;
#endif // PVTOL_DEBUG
	 pthread_mutex_lock(&(m_itxdEntry->itxdMutex));
	 bool    otherEndAsync = false;
	 if (m_itxdEntry->firstOneIn)
	   {
	     if (m_itxdEntry->sendType == ASYNC)
	                     otherEndAsync = true;

	     m_itxdEntry->firstOneIn = false;
	   }
	  else
	   {
	     m_itxdEntry->firstOneIn = true;
	     m_itxdEntry->sendType   = SYNC;
	   }
	 pthread_mutex_unlock(&(m_itxdEntry->itxdMutex));

	 if (m_isSrc)
	   {
	     if (otherEndAsync)
	       {
	         iLocalCopy(count, srcOff, destOff, *(m_itxdEntry->sReq));
	       }
	      else
	       {
	         pthread_cond_wait(&(m_itxdEntry->itxdCond),
	                           &(m_itxdEntry->itxdCvMutex));
	       }
	     m_itxdEntry->srcAddr = m_srcAddr;
#ifdef PVTOL_DEBUG
	     int mrc = pthread_mutex_trylock(&(m_itxdEntry->itxdCvMutex));
             cout << "XFER:" << m_myRank << " Src in Send() trylock rc="
                  << mrc << endl;
#endif // PVTOL_DEBUG
	   }
          else if (m_isDest)
	   {//                              this is a dest
	     while (m_itxdEntry->srcAddr == NULL)
	       {
	         struct timespec    sleepTime;
		 sleepTime.tv_sec  = 0;
		 sleepTime.tv_nsec = 1000000;// sleep at least 1 ms
		 nanosleep(&sleepTime, NULL);

#ifdef PVTOL_DEVELOP
		 PvtolProgram  prog;
		 int           myprocId = prog.getProcId();
	         cout << "Xfer[" << myprocId << "," << m_destRank
		      << "]: Send() Src Addr is Still NULL" << endl;
#endif // PVTOL_DEVELOP
	       }
	     m_srcAddr = m_itxdEntry->srcAddr;
	     if (otherEndAsync)
	       {
	         iLocalCopy(count, srcOff, destOff, *(m_itxdEntry->sReq));
	       }
	      else
	       {
	         pthread_mutex_lock(&(m_itxdEntry->itxdCvMutex));
	         localCopy(count, srcOff, destOff);
	         m_itxdEntry->srcAddr = NULL;
	         pthread_cond_signal(&(m_itxdEntry->itxdCond));
	         pthread_mutex_unlock(&(m_itxdEntry->itxdCvMutex));
	       }//endIf other end async & already there
	   }
       }//endIf trait type

    return;
  }//end send()



//------------------------------------------------------------------------
//  Method: isend()
//
//  Description: NonBlocking send call
//
//  Inputs: 1: integer count of items to send
//          2: integer offset into the src block
//          3: integer offset into the dest block
//          4: SendRequest reference
//
//  Return: there is no recode code from the function, but
//          the SendRequest may be queried concerning the status of
//          the send
//
//------------------------------------------------------------------------
void Transfer::isend(int count, int srcOff, int destOff, SendRequest &req)
  {
    if (m_trait == NO_LOCAL_DATA_MOVEMENT)
                 req.m_done = true;
      else
                 req.m_done = false;

    if (m_trait == STATIC_TRANSFER)
	       iStaticSend(req);
     else if (m_trait == NON_STATIC_TRANSFER)
	       iNonStaticSend(count, srcOff, destOff, req);
     else if (m_trait == DATA_MOVEMENT_IS_THREAD_LOCAL)
	       iLocalCopy(count, srcOff, destOff, req);
     else if (m_trait == DATA_MOVEMENT_IS_PROC_LOCAL)
       {//          inter Thread Comm
	 pthread_mutex_lock(&(m_itxdEntry->itxdMutex));
	 bool    otherEndSync = false;
	 bool    readyToCopy  = false;

	 if (m_isSrc)
	           m_itxdEntry->srcAddr = m_srcAddr;

	 if (m_isDest)
	           m_itxdEntry->destAddr = m_destAddr;

	 if (m_itxdEntry->firstOneIn)
	   {//            the other guy was here already
	     if (m_itxdEntry->sendType == SYNC)
	                     otherEndSync = true;

	     m_itxdEntry->firstOneIn = false;
	     readyToCopy             = true;
	   }
	  else
	   {//            we are the 1st one here
	     m_itxdEntry->firstOneIn = true;
	     m_itxdEntry->sendType   = ASYNC;
	     m_itxdEntry->sReq       = &req;
	   }
	 pthread_mutex_unlock(&(m_itxdEntry->itxdMutex));

         //  If this is the src
	 //   Else this is dest
	 //  EndIf
	 if (m_isSrc)
	   {
	    if (readyToCopy)
	       {
		 m_destAddr = m_itxdEntry->destAddr;
	         localCopy(count, srcOff, destOff);
                 req.m_done = true;
                 pthread_mutex_lock(&(m_itxdEntry->sReq->m_waitMutex));
		 m_itxdEntry->sReq->m_done = true;
		 m_itxdEntry->sReq->m_recvdCount = count;
		 if (m_itxdEntry->sReq->m_isWaiting)
		   {
	            pthread_mutex_lock(&(m_itxdEntry->sReq->m_waitCvMutex));
	            pthread_cond_signal(&(m_itxdEntry->sReq->m_waitCond));
                    pthread_mutex_unlock(&(m_itxdEntry->sReq->m_waitCvMutex));
		    m_itxdEntry->sReq->m_isWaiting = false;
		   }
                 pthread_mutex_unlock(&(m_itxdEntry->sReq->m_waitMutex));
	       }
#ifdef PVTOL_DEBUG
              else
	       {
		 PvtolProgram  prog;
		 int           myprocId = prog.getProcId();
	         cout << "Xfer[" << myprocId << "," << m_myRank
		      << "]: iSend() Src falling thru" << endl;
	       }//endIf readyToCopy
#endif // PVTOL_DEBUG
	   }
          else
	   {//                              this is a dest
	    if (readyToCopy)
	       {
	         m_srcAddr = m_itxdEntry->srcAddr;
	         localCopy(count, srcOff, destOff);
                 req.m_done = true;
                 req.m_recvdCount = count;

	         if (!otherEndSync)
                   {// the src end is asynchronous
                     pthread_mutex_lock(&(m_itxdEntry->sReq->m_waitMutex));
		     m_itxdEntry->sReq->m_done = true;
		     if (m_itxdEntry->sReq->m_isWaiting)
		       {
	                pthread_mutex_lock(&(m_itxdEntry->sReq->m_waitCvMutex));
	                pthread_cond_signal(&(m_itxdEntry->sReq->m_waitCond));
                        pthread_mutex_unlock(&(m_itxdEntry->sReq->m_waitCvMutex));
		        m_itxdEntry->sReq->m_isWaiting = false;
		       }
                     pthread_mutex_unlock(&(m_itxdEntry->sReq->m_waitMutex));
                   }
		  else
                   {// the src end is synchronous
	             pthread_mutex_lock(&(m_itxdEntry->itxdCvMutex));
	             m_itxdEntry->srcAddr = NULL;
	             pthread_cond_signal(&(m_itxdEntry->itxdCond));
	             pthread_mutex_unlock(&(m_itxdEntry->itxdCvMutex));
                   }
	       }//endIf readyToCopy
#ifdef PVTOL_DEBUG
              else
	       {
		 PvtolProgram  prog;
		 int           myprocId = prog.getProcId();
	         cout << "Xfer[" << myprocId << "," << m_destRank
		      << "]: iSend() Dest falling thru" << endl;
	       }//endIf readyToCopy
#endif // PVTOL_DEBUG
	   }//endIf src or dest
       }//endIf DATA_MOVEMENT_IS_PROC_LOCAL

    return;
  }//end isend()


//------------------------------------------------------------------------
//  Method: getTag()
//
//  Description: returns internal tag used by the Transfer
//
//  Returns: the tag
//
//  Note: this is an internal method to be used only by PVTOL objects
//  (e.g. conduit) that need to link multiple Transfers/Routes
//  on the same pipe.  This requires exact knowledge of the order of sends.
//------------------------------------------------------------------------

int Transfer::getTag() const
  {
    return m_tag;
  } // end getTag()

//------------------------------------------------------------------------
//  Method: setTag()
//
//  Description: replaces internal tag used by the Transfer
//
//  Inputs: 1: the new tag to be used by transfer
//
//  Returns: nothing
//
//  Note: this is an internal method to be used only by PVL objects
//  (e.g. conduit) that need to link multiple Transfers/Routes
//  on the same pipe.  This requires exact knowledge of the order of sends.
//
//  Because this affects the allocation of tags, it must
//  be done in a SPMD way across the task in which the route
//  was created.
//------------------------------------------------------------------------

void Transfer::setTag(int tag)
  {
    // return the old tag
    PvtolProgram  pvtol;
    pvtol.getCurrentTask().getCommScope().returnTag(m_tag);

    // and keep this one
    m_tag = tag;
  } // end setTag()



//------------------------------------------------------------------------
//  Method: localCopy & iLocalCopy()
//
//  Description: copy the data, no communication is required.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
          // ********  Blocking call ******** //
void Transfer::localCopy(int count, int srcOffset, int destOffset)
  {
    int size = count*m_eltSize;
    char *srcAddr=static_cast<char *>(m_srcAddr);
    srcAddr += srcOffset * m_eltSize;

    char *destAddr=static_cast<char *>(m_destAddr);
    destAddr += destOffset * m_eltSize;

    memcpy(destAddr, srcAddr, size);
    m_recvdCount = m_sendCount;

    return;
  }//end localCopy()


          // ********  NonBlocking call ******** //
void Transfer::iLocalCopy(int count, int srcOff, int destOff, SendRequest &req)
  {
    localCopy(count, srcOff, destOff);

    req.m_done = true;
    req.m_recvdCount = count;

    return;
  }//end iLocalCopy()


//------------------------------------------------------------------------
//  Method: nonStaticSend()
//
//  Description: send the data, blocking call
//
//  Inputs: 1: integer count of items to send
//          2: integer offset into the src block
//          3: integer offset into the dest block
//
//  Return: none
//
//------------------------------------------------------------------------
void Transfer::nonStaticSend(int count, int srcOff, int destOff)
  {
    int  size = count * m_eltSize;
    m_recvdCount = 0;
    char *srcAddr, *destAddr;

    if (m_isSrc || m_isDest)
      {
	srcAddr = static_cast<char *>(m_srcAddr) + srcOff * m_eltSize;
	destAddr=static_cast<char *>(m_destAddr) + destOff * m_eltSize;
      }

#ifdef PVTOL_DEBUG
    PvtolProgram  prog;
    int           myprocId = prog.getProcId();
    cout << "Xfer[" << myprocId << "," << m_myRank
         << "]: nsSend TAG=" << m_tag << endl;
#endif // PVTOL_DEBUG

    if (m_isDest)
      {
       m_commScope->recv(srcAddr, destAddr, size, m_srcProcRank, m_tag);
       m_recvdCount = count;
      }

    if (m_isSrc)
      {
       m_commScope->send(srcAddr, destAddr, size, m_destProcRank, m_tag);
      }

    return;
  }//end nonStaticSend()


//------------------------------------------------------------------------
//  Method: iNonStaticSend()
//
//  Description: send the data
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
void Transfer::iNonStaticSend(int count, int srcOff, int destOff, SendRequest &req)
  {
    int  size = count * m_eltSize;
    char *srcAddr, *destAddr;

    if (m_isSrc || m_isDest)
      {
	srcAddr = static_cast<char *>(m_srcAddr) + srcOff * m_eltSize;
	destAddr=static_cast<char *>(m_destAddr) + destOff * m_eltSize;
      }

    if (m_isDest)
      {
       //PvtolStatus   status;
       m_recvdCount = 0;

       if (!req.preset())
	 {
	   req.m_numRecvs    = 1;
	   req.m_recvRequest = new PvtolRequest;
	   req.m_iBuiltReqs  = true;
	 }

       m_commScope->irecv(srcAddr,
			 destAddr,
			 size,
			 m_srcProcRank,
			 m_tag,
			 *req.m_recvRequest);

       req.m_done        = false;// mark the Request as Not Done
      }//endIf this is a dest

    if (m_isSrc)
      {
       if (!req.preset())
	 {
	   req.m_numSends    = 1;
	   req.m_sendRequest = new PvtolRequest;
	   req.m_iBuiltReqs  = true;
	 }

       m_commScope->isend(srcAddr,
			 destAddr,
			 size,
			 m_destProcRank,
			 m_tag,
			 *req.m_sendRequest);

       req.m_done        = false;// mark the PvtolRequest as Not Done
      }//endIf this is a src

    req.m_preset = 1;

    return;
  }//end iNonStaticSend()


//------------------------------------------------------------------------
//  Method: staticSend()
//
//  Description: send the data
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
void Transfer::staticSend()
  {
    if (m_isSrc || m_isDest)
      {
	PvtolStatus   status;
	m_req.start();
        m_recvdCount = 0;
	m_req.wait(status);

	if (m_isDest)
            m_recvdCount = status.getCount();
      }

    return;
  }//end staticSend()


//------------------------------------------------------------------------
//  Method: iStaticSend()
//
//  Description: send the data
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
void Transfer::iStaticSend(SendRequest &req)
  {
     //int  i;

#ifdef _DEBUG_2
    PvtolProgram  pvtol;
    cout << "Transfer[" << pvtol.getProcId() << "], in iStaticSend()"
	 << endl;
#endif
     if (!req.preset())
	  req.setup(*this);

    if (m_isSrc || m_isDest)
      {
	//PvtolStatus   status;

	req.m_done = false;
	m_req.start();
        m_recvdCount = 0;

	if (m_isSrc)
	  {
	    req.m_numSends    = 1;// this is a point to point comm
	    req.m_sendRequest = &m_req;
	  }

	if (m_isDest)
	  {
	    req.m_numRecvs    = 1;// this is a point to point comm
	    req.m_recvRequest = &m_req;
	  }
      }//endIf this is a Src OR a Dest

    return;
  }//end iStaticSend()


//------------------------------------------------------------------------
//  Method: destructor
//
//  Description: destroy the Transfer obj.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
Transfer::~Transfer()
  {
    // return the communications tag
    PvtolProgram  pvtol;
    pvtol.getCurrentTask().getCommScope().returnTag(m_tag);
    m_tag = -1;
  }

}//end Namspace

