/**
 * File: SendRequest.cc
 *    February 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of SendRequest
 *    A SendRequest object is used to test for the completion of a non-
 *	   Blocking send from a Transfer, Route, or Broadcast object.
 *	   It may also be used to wait for the completion of a non-
 *         Blocking send from a Transfer, Route, or Broadcast object.
 *
 *    Note: completion is defined as the "send having completed". At the
 *	    a source node this implies the data has left the buffers and
 *	    a user may now safely reuse his send buffer. At a destination
 *	    node this implies the data has been received. The receive buffer
 *	    now holds the data which was sent.
 *
 *  $Id: SendRequest.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <Transfer.h>
#include <PvtolProgram.h>
#include <PvtolRequest.h>
#include <SendRequest.h>
#include <Route.h>
#include <Exception.h>

#include <iostream>
using std::cout;
using std::endl;


namespace ipvtol
{

//------------------------------------------------------------------------
//  Method:     Default Constructor
//          SendRequest()
//
//  Description: Constructs an empty SendRequest
//
//  Inputs: none
//  Returns: void
//
//------------------------------------------------------------------------
SendRequest::SendRequest() :
	   m_preset(false),
	   m_done(true),
	   m_recvdCount(0),
	   m_numRecvs(0),
	   m_recvRequest(NULL),
	   m_numSends(0),
	   m_sendRequest(NULL),
	   m_iBuiltReqs(false),
       m_currRouteSrcXfer(NULL),
       m_currRouteDestXfer(NULL),
	   m_associatedRoute(NULL),
       m_associatedTransfer(NULL),
       m_isWaiting(false)

  {
    pthread_mutex_init(&m_waitMutex, NULL);
    pthread_mutex_init(&m_waitCvMutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);
    return;
  }//end construct


//------------------------------------------------------------------------
//  Method:     Construct for a Route
//          SendRequest(const Route &route)
//
//  Description: Constructs an empty SendRequest
//
//  Inputs: const Route &route
//  Returns: void
//
//------------------------------------------------------------------------
SendRequest::SendRequest(Route &route) :
	   m_done(true),
	   m_recvdCount(0),
	   m_numRecvs(0),
	   m_recvRequest(NULL),
	   m_numSends(0),
       m_sendRequest(NULL),
       m_iBuiltReqs(false),
       m_currRouteSrcXfer(NULL),
       m_currRouteDestXfer(NULL),
       m_associatedRoute(&route),
       m_associatedTransfer(NULL),
       m_isWaiting(false)
  {
    int   i, persist;
    m_preset  = true;
    m_eltSize = route.m_eltSize;

    pthread_mutex_init(&m_waitMutex, NULL);
    pthread_mutex_init(&m_waitCvMutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);

    route.registerSendRequest(this);

//    the SendRequest must allocate its own PvtolRequests.
//  ----------------------------------------------------------*
    if (route.m_maxSends)
              m_sendRequest = new PvtolRequest[route.m_maxSends];

    if (route.m_maxRecvs)
              m_recvRequest = new PvtolRequest[route.m_maxRecvs];

    if (route.m_currSrcXfer.sendInfo == NULL)
           m_numSends = 0;
      else
       {
            m_numSends = route.m_currSrcXfer.numSends;

	    if (route.m_trait == Route::STATIC_ROUTE)
	      {//       setup Persistant Sends
	        for(persist=0, i=0; i<m_numSends; i++) {
		  if (!route.m_currSrcXfer.sendInfo[i].sendIsLocal)
		    {
		      route.m_commScopePtr->sendInit(
			    route.m_currSrcXfer.sendInfo[i].addr,
			    route.m_currSrcXfer.sendInfo[i].destAddr,
			    route.m_currSrcXfer.sendInfo[i].byteSize,
			    route.m_currSrcXfer.sendInfo[i].destRank,
			    route.m_tag,
			    m_sendRequest[persist++]);
		    }//endIf send is local
		}//endFor all sends

		m_numSends = persist;// Remember how many!
	      }//endIf everything stays the same
       }//endIf there are sends

     if (route.m_currDestXfer.recvInfo == NULL)
           m_numRecvs = 0;
      else
       {
	    m_numRecvs    = route.m_currDestXfer.numRecvs;

	    if (route.m_trait == Route::STATIC_ROUTE)
	      {//       setup Persistant Recvs
	        for(persist=0, i=0; i<m_numRecvs; i++) {
		  if (!route.m_currDestXfer.recvInfo[i].recvIsLocal)
		    {
		     route.m_commScopePtr->recvInit(
			   route.m_currDestXfer.recvInfo[i].srcAddr,
			   route.m_currDestXfer.recvInfo[i].addr,
			   route.m_currDestXfer.recvInfo[i].byteSize,
			   route.m_currDestXfer.recvInfo[i].srcRank,
			   route.m_tag,
			   m_recvRequest[persist++]);
		    }//endIf recv is local
		}//endFor All Recvs

		m_numRecvs = persist;// Remember how many!
	      }//endIf everything stays the same
       }//endIf there are recvs

    m_iBuiltReqs = true;

    return;
  }//end construct from Route


//------------------------------------------------------------------------
//  Method:     Construct for a Transfer
//          SendRequest(const Transfer &transfer)
//
//  Description: Constructs
//
//  Inputs: const Transfer &transfer
//  Returns: void
//
//------------------------------------------------------------------------
SendRequest::SendRequest(const Transfer &transfer) :
	   m_preset(true),
	   m_done(true),
	   m_recvdCount(0),
	   m_numRecvs(0),
	   m_recvRequest(NULL),
	   m_numSends(0),
	   m_sendRequest(NULL),
	   m_iBuiltReqs(false),
	   m_currRouteSrcXfer(NULL),
	   m_currRouteDestXfer(NULL),
	   m_associatedRoute(NULL),
       m_associatedTransfer(&transfer),
       m_isWaiting(false)

  {

    m_eltSize = transfer.m_eltSize;

    pthread_mutex_init(&m_waitMutex, NULL);
    pthread_mutex_init(&m_waitCvMutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);

    if (transfer.m_isSrc)
      {
           m_numSends = 1;
      }

    if (transfer.m_isDest)
      {
           m_numRecvs = 1;
      }

    if (transfer.m_trait == Transfer::STATIC_TRANSFER)
      {
       if (transfer.m_isSrc)
         {
   	   m_sendRequest = const_cast<PvtolRequest *>(&transfer.m_req);
         }

       if (transfer.m_isDest)
         {
   	   m_recvRequest = const_cast<PvtolRequest *>(&transfer.m_req);
         }
      }
     else
      {
       if (transfer.m_isSrc)
         {
   	   m_sendRequest = new PvtolRequest[1];
	   m_iBuiltReqs = true;
         }

       if (transfer.m_isDest)
         {
	   m_recvRequest = new PvtolRequest[1];
	   m_iBuiltReqs = true;
         }
      }//endIf Static or not

    return;
  }//end construct from Transfer


//------------------------------------------------------------------------
//  Method:     setup for a particular route
//          setup(Route &route)
//
//  Description: setup for a particular route
//
//  Inputs: a ref to a Route obj
//  Returns: void
//
//------------------------------------------------------------------------
 void SendRequest::setup(Route &route)
  {
    int   i, persist;
    m_preset  = true;
    m_eltSize = route.m_eltSize;

    pthread_mutex_init(&m_waitMutex, NULL);
    pthread_mutex_init(&m_waitCvMutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);

    route.registerSendRequest(this);

//    the SendRequest must allocate its own PvtolRequests.
//  ----------------------------------------------------------*
    if (route.m_maxSends)
              m_sendRequest = new PvtolRequest[route.m_maxSends];

    if (route.m_maxRecvs)
              m_recvRequest = new PvtolRequest[route.m_maxRecvs];

    if (route.m_currSrcXfer.sendInfo == NULL)
           m_numSends = 0;
      else
       {
            m_numSends = route.m_currSrcXfer.numSends;

	    if (route.m_trait == Route::STATIC_ROUTE)
	      {//       setup Persistant Sends
	        for(persist=0, i=0; i<m_numSends; i++) {
		  if (!route.m_currSrcXfer.sendInfo[i].sendIsLocal)
		    {
		      route.m_commScopePtr->sendInit(
			    route.m_currSrcXfer.sendInfo[i].addr,
			    route.m_currSrcXfer.sendInfo[i].destAddr,
			    route.m_currSrcXfer.sendInfo[i].byteSize,
			    route.m_currSrcXfer.sendInfo[i].destRank,
			    route.m_tag,
			    m_sendRequest[persist++]);
		    }//endIf send is local
		}//endFor all sends

		m_numSends = persist;// Remember how many!
	      }//endIf everything stays the same
       }//endIf there are sends

     if (route.m_currDestXfer.recvInfo == NULL)
           m_numRecvs = 0;
      else
       {
	    m_numRecvs    = route.m_currDestXfer.numRecvs;

	    if (route.m_trait == Route::STATIC_ROUTE)
	      {//       setup Persistant Recvs
	        for(persist=0, i=0; i<m_numRecvs; i++) {
		  if (!route.m_currDestXfer.recvInfo[i].recvIsLocal)
		    {
		     route.m_commScopePtr->recvInit(
			   route.m_currDestXfer.recvInfo[i].srcAddr,
			   route.m_currDestXfer.recvInfo[i].addr,
			   route.m_currDestXfer.recvInfo[i].byteSize,
			   route.m_currDestXfer.recvInfo[i].srcRank,
			   route.m_tag,
			   m_recvRequest[persist++]);
		    }//endIf recv is local
		}//endFor All Recvs

		m_numRecvs = persist;// Remember how many!
	      }//endIf everything stays the same
       }//endIf there are recvs

    m_iBuiltReqs = true;

    return;
  }//end setup for a Route


//------------------------------------------------------------------------
//  Method:     setup for a particular transfer
//          setup(Transfer &transfer)
//
//  Description: setup for a particular transfer
//
//  Inputs: a ref to a Transfer obj
//  Returns: void
//
//------------------------------------------------------------------------
 void SendRequest::setup(const Transfer &transfer)
  {
    m_eltSize = transfer.m_eltSize;
    throw NotImplementedYet(__FILE__, __LINE__);

    return;
  }//end setup for a Route


//------------------------------------------------------------------------
//  Method: test()
//
//  Description: Test whether the send request has completed
//
//  Inputs: none
//
//  Return:
//
//------------------------------------------------------------------------
 bool SendRequest::test()
  {
    int    rc = 1;
    int    i;
    PvtolStatus stat;
    int    rcvdCount;

    if ((m_associatedTransfer != NULL) &&
        (m_associatedTransfer->m_trait == Transfer::DATA_MOVEMENT_IS_PROC_LOCAL))
	          return(m_done);

    if ((m_associatedTransfer != NULL) && (m_associatedRoute != NULL)
        &&  (m_recvRequest == NULL) && (m_sendRequest == NULL))
	          return(m_done);

    if (!m_done)
      {
	for (i=0, rcvdCount=0; rc && (i<m_numRecvs); i++) {
	  (m_recvRequest[i]).test(&rc, stat);
	  if (rc)
	    {
	      rcvdCount += stat.getCount();
#ifdef _DEBUG_3
              cout << "SendReq[" << pvlProcess.getProcId()
		   << "]::test Rcvd=" // << stat.getCount(CSP::CHAR)
		   << " Src=" << stat.getSource()
		   << " Tag=" << stat.getTag()
		   << " Err=" << stat.getError()
		   << endl;
#endif
	    }
	}//endFor each recv req

	for (i=0; rc && (i<m_numSends); i++) {
	          (m_sendRequest[i]).test(&rc, stat);
	}//endFor each recv req

      }//endIf !done

    return(rc);
  }//end test()


//------------------------------------------------------------------------
//  Method: wait()
//
//  Description: wait until the send request has completed
//
//  Inputs: none
//
//  Return: bool - true  => send completed OK
//                 false => some error occurred
//
//------------------------------------------------------------------------
 bool SendRequest::wait()
  {
    int    i;
    PvtolStatus stat;

    if ((m_associatedTransfer != NULL) &&
        (m_associatedTransfer->m_trait == Transfer::DATA_MOVEMENT_IS_PROC_LOCAL))
      {
         pthread_mutex_lock(&m_waitMutex);
	 if (!m_done)
	   {
	     m_isWaiting = true;
	     pthread_mutex_lock(&m_waitCvMutex);
             pthread_mutex_unlock(&m_waitMutex);
	     pthread_cond_wait(&m_waitCond, &m_waitCvMutex);
#ifdef PVTOL_DEBUG
	     PvtolProgram  prog;
	     int           myprocId = prog.getProcId();
	     cout << "SReq[" << myprocId
		  << "]: wait() out of CV Wait" << endl;
#endif // PVTOL_DEBUG
	     pthread_mutex_unlock(&m_waitCvMutex);
	   }
	  else
             pthread_mutex_unlock(&m_waitMutex);
      }//endIf assoc Xfer is btwn threads of same process

    if (!m_done)
      {
	 for (i=0, m_recvdCount=0; i<m_numRecvs; i++) {
	     (m_recvRequest[i]).wait(stat);
	     m_recvdCount += stat.getCount();
	 }//endFor each recv req

	 m_recvdCount /= m_eltSize;

	 for (i=0; i<m_numSends; i++) {
	     (m_sendRequest[i]).wait(stat);
	 }//endFor each recv req

         m_done = true;
      }//endIf needs more checking

    return(m_done);
  }//end wait()


//------------------------------------------------------------------------
//  Method: cancel()
//
//  Description: Cancel the send.  Frees Comm library resources associated
//               with the send.
//
//  Inputs: none
//
//------------------------------------------------------------------------
 void SendRequest::cancel()
  {
  //bool   rc = true;
    int    i;
  //PvtolStatus stat;

    if (!m_done)
      {
	   for (i=0, m_recvdCount=0; i<m_numRecvs; i++) {
	     (m_recvRequest[i]).cancel();
	   }//endFor each recv req

	   for (i=0; i<m_numSends; i++) {
	     (m_sendRequest[i]).cancel();
	   }//endFor each recv req
      }//endIf needs more checking

    wait();
  }//end cancel()


//------------------------------------------------------------------------
//  Method:     disassociate
//          disassociate(Route &route)
//
//  Description: Constructs an empty SendRequest
//
//  Inputs: const Route &route
//  Returns: void
//
//------------------------------------------------------------------------
void SendRequest::disassociate(Route &route)
 {
   m_associatedRoute = NULL;

   if (m_iBuiltReqs)
     {
	if (m_recvRequest != NULL)
		delete[] m_recvRequest;

	if (m_sendRequest != NULL)
		delete[] m_sendRequest;
     }

   m_recvRequest = NULL;
   m_sendRequest = NULL;
   m_numSends    = 0;
   m_numRecvs    = 0;
   m_done        = true;

   return;
 }//end disassociate()


//------------------------------------------------------------------------
//  Method:     Assignment Operator "="
//
//  Description: Constructs an empty SendRequest
//
//  Inputs: const SendRequest &req
//  Returns: void
//
//------------------------------------------------------------------------
const SendRequest& SendRequest::operator=(const SendRequest &rhs)
  {
    throw NotImplementedYet(__FILE__, __LINE__);

    return(*this);
  }//end Op=


//------------------------------------------------------------------------
//  Method: destructor
//
//  Description: destroy the SendRequest obj.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
SendRequest::~SendRequest()
  {
    if (m_associatedRoute != NULL)
	 m_associatedRoute->unregisterSendRequest(this);

    if (m_iBuiltReqs)
      {
	if (m_recvRequest != NULL)
		delete[] m_recvRequest;

	if (m_sendRequest != NULL)
		delete[] m_sendRequest;
      }
    return;
  }

}//end Namspace

