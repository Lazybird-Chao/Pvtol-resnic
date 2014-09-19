/**
 * File: ConduitConnection.inl
 *    March 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-19 13:45:17 -0500 (Thu, 19 Feb 2009) $
 *  \version $LastChangedRevision: 939 $
 *  \brief   Methods of the ConduitConnection
 *
 *  $Id: ConduitConnection.inl 939 2009-02-19 18:45:17Z jav5880 $
 *
 * Author: Glenn Schrader
 */

#ifndef PVTOL_ConduitConnection_cc
#define PVTOL_ConduitConnection_cc

#include <ConduitConnection.h>
#include <Transfer.h>
#include <ConduitSenders.h>
#include <ConduitSendManager.h>

#include <scoped_array.hpp>
using boost::scoped_array;

#include <stdio.h>
#include <utility>

#define PVTOL_DEBUG_1

namespace ipvtol
{

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
Connection<DATATYPE, TAGTYPE, USE_EOC>::Connection(
                       EndpointX & src,
                       EndpointX & dst,
                       bool useTag,
                       Route::Flags flags) :
        m_src(src),
        m_dst(dst),
	m_dataSize(0),
	m_dataStaysInPlace(src.getLocalXfer() && dst.getLocalXfer()),
	m_spRoute(NULL)
{
    m_srcProcLocal = localResponsibilities(src);
    m_dstProcLocal = localResponsibilities(dst);

    PvtolProgram pvtol;
    int mypid = pvtol.getProcId();
    TaskBase  &ct  = pvtol.getCurrentTask();
    // int tag = ct.getTransferTag(ct.getLocalThreadRank());
    // CommScope &cs  = ct.getCommScope();

    if (src.getSize() > dst.getSize())
       m_dataSize = src.getSize();
     else
       m_dataSize = dst.getSize();

    unsigned int maxOutstanding = 0;
    if (!m_dataStaysInPlace)
      {// there is some data movement
        if (m_src.m_depth > 1 || m_dst.m_depth > 1)
          {
            // flags = (Route::Flags)(Route::TAG_WILL_BE_SUPPLIED |
	    // 			   (unsigned int)flags);
            m_spRoute = new Route(src.m_ddo, dst.m_ddo,
	                          *(src.m_map), *(dst.m_map),
				  m_srcProcLocal, m_dstProcLocal,
				  src.m_procList, dst.m_procList,
				  flags);
          }
         else
          {
            flags = (Route::Flags)(Route::STATIC |
	                           // Route::TAG_WILL_BE_SUPPLIED |
				   (unsigned int)flags);
            m_spRoute = new Route(src.m_ddo, dst.m_ddo,
	                          *(src.m_map), *(dst.m_map),
				  m_srcProcLocal, m_dstProcLocal,
				  src.m_procList, dst.m_procList,
                                  flags);
          }

        // m_spRoute->setTag(tag);

        if (USE_EOC || useTag)
          {
            const vector<int>& srcRanks = src.getSetupCompleteRanks();
            const vector<int>& dstRanks = dst.getSetupCompleteRanks();
            const vector<int>& srcProcs = src.getProcList();
            const vector<int>& dstProcs = dst.getProcList();

            int *srcsForDest = new int[srcRanks.size()];

            for (unsigned i = 0; i < dstRanks.size(); i++)
              {
#ifdef PVTOL_DEBUG_1
		 cout << "Conn[" << mypid << "] # of destprocs is"
		      << dstRanks.size() << endl;
#endif // PVTOL_DEBUG_1
#ifdef NOT_YET
                // FIXME: suboptimal pairing
                // for each destination, arbitrarily choose its first source as 
                // the node that sends it tags
                int numSrcsForDest = m_spRoute->srcRanks(dstRanks[i],
                                                         srcsForDest);
#else
                int numSrcsForDest;
		if (m_spRoute == NULL)
                     numSrcsForDest = 0;
		 else
                     numSrcsForDest = 1;

#endif // NOT_YET
                int srcToUse = -1;
                if (numSrcsForDest > 0)
                  {
                    srcToUse = srcRanks[0];// FIXME
                  }
                 else
                  {
                    // this destination is in the map, but it's not getting
                    // any data.  Arbitrarily receive from the first node
                    // in the map.  The sequence number sending for one-sided
                    // communication must make sure to send down this pipe as
                    // well.
                    srcToUse = srcRanks[0];
                  }

                m_tagNodePairs.push_back(std::make_pair(srcProcs[0],
		                                        dstProcs[i]));

                shared_ptr<Transfer>
                    spTrans(
                        new Transfer(srcToUse,
		                     reinterpret_cast<char *>(src.m_tagBlock),
                                     dstRanks[i],
				     reinterpret_cast<char *>(dst.m_tagBlock),
                                     sizeof(TagSeqNumWrapper<TAGTYPE, false>)) );

                // spTrans->setTag(tag);

                m_tagTransfers.push_back(spTrans);

                if (USE_EOC)
                  {
                    shared_ptr<Transfer>
                        spEocTrans(
                            new Transfer(srcToUse,
                                     reinterpret_cast<char *>(src.m_eocBlocks[dst.m_rankInCommGroup]),
                                     dstRanks[i],
                                     reinterpret_cast<char *>(dst.m_eocBlocks[src.m_rankInCommGroup]),
                                     sizeof(SeqNum)) );
                    m_eocTransfers.push_back(spEocTrans);
                  }
               }//endFor each dest proc
 
#ifdef DEBUG_TAGS
            if (pvlProcess.getCurrentTaskMap().findIndex(pvlProcess.getProcId())
                == 0)
              {
                for (unsigned i = 0; i < m_tagNodePairs.size(); i++)
                  {
                    cout << "(" << m_tagNodePairs[i].first << ","
                         << m_tagNodePairs[i].second << ")" << endl;
                  }
              }
#endif // DEBUG_TAGS

          } // using tags

        // The route and transfer as set up above should be SPMD, but we don't
        // need any of the other structures if we're not at all local
        if (m_srcProcLocal || m_dstProcLocal)
          {// there is local activity

            // create as many SendManager objects as we need.  To avoid ever
            // running out, we need 1 for every local buffer entry (src & dest),
            // because it is possible the dest buffer could be full of COMPLETED
            // sends that haven't been extracted yet, and the insert side
            // is trying to fill the source buffer.
            if (m_srcProcLocal)
              {
                maxOutstanding += src.m_depth;
              }
            if (m_dstProcLocal)
              {
                maxOutstanding += dst.m_depth;
              }

            for (unsigned i = 0; i < maxOutstanding; i++)
              {
                shared_ptr<SendManager<DataTagSenderX> >
                  spSM( new SendManager<DataTagSenderX>(*this) );
                m_sendManagers.push_back(spSM);

                if (USE_EOC)
                  { 
                    shared_ptr<SendManager<EocSenderX> >
                      spSMEoc( new SendManager<EocSenderX>(*this) );
                    m_eocSendManagers.push_back(spSMEoc);
                  }
              }
          }//endIf there is local activity
      }//endIf some data movement required

    if (maxOutstanding)
      {
        m_mgrsSendIndex.setBufSize(maxOutstanding);
        m_mgrsRecvIndex.setBufSize(maxOutstanding);
      }

    m_mgrsSendIndex = 0;
    m_mgrsRecvIndex = 0;

    if (USE_EOC)
    {
        // Indices into the list of SendManager objects
        if (maxOutstanding)
	  {
            m_eocMgrsSendIndex.setBufSize(maxOutstanding);
            m_eocMgrsRecvIndex.setBufSize(maxOutstanding);
	  }

        m_eocMgrsSendIndex = 0;
        m_eocMgrsRecvIndex = 0;

        // Indices into the EOC buffers at source and destination
        // endpoints, used whenever an EOC SendManager is requested
        // for a transfer
        m_eocSrcBufferIndex.setBufSize(m_src.m_depth);
        m_eocSrcBufferIndex = 0;
        m_eocDstBufferIndex.setBufSize(m_dst.m_depth);
        m_eocDstBufferIndex = 0;
    }

    // These are used to determine which buffer slots to
    // send to or receive from
    int tempSrcBufferIndex, tempDstBufferIndex;
    findInitialBufferIndices(m_src.m_rankInCommGroup,
                             m_src.m_numInCommGroup,
                             m_dst.m_rankInCommGroup,
                             m_dst.m_numInCommGroup,
                             &tempSrcBufferIndex,
                             &tempDstBufferIndex);

    m_srcBufferIndex.setBufSize(m_src.m_depth);
    m_srcBufferIndex = tempSrcBufferIndex;

    m_dstBufferIndex.setBufSize(m_dst.m_depth);
    m_dstBufferIndex = tempDstBufferIndex;
#ifdef PVTOL_DEBUG_1
    int tsttag = ct.getTransferTag(ct.getLocalThreadRank());
    cout << "Conn[" << mypid << "] at end TAG value is " 
         << tsttag << endl;
#endif // PVTOL_DEBUG_1

    return;
}//end Constructor()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
Connection<DATATYPE, TAGTYPE, USE_EOC>::~Connection()
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
SendManager< DataTagSender<DATATYPE, TAGTYPE, USE_EOC> > *
Connection<DATATYPE, TAGTYPE, USE_EOC>::getManagerForSrc()
{
#ifdef PVTOL_DEVELOP
    if (!m_srcProcLocal)
    {
        PvtolProgram prog;
        int myId = prog.getProcId();
        TaskBase &ct     = prog.getCurrentTask();
        TaskId myTaskId  = ct.getTaskID();
        int myThreadRank = ct.getGlobalThreadRank();
	char ostr[256];
	sprintf(ostr,
	        "Connect[%d, %d, %d]: getManagerForSrc when src not local",
		myId, myTaskId, myThreadRank);
        throw Exception(ostr, __FILE__, __LINE__);
    }

#endif // PVTOL_DEVELOP

    SendManager<DataTagSenderX> * mgr =
                    m_sendManagers[m_mgrsSendIndex].get();
    m_mgrsSendIndex++;
 
#ifdef PVTOL_DEVELOP
    // With the current design, we shouldn't run out of managers, but we can
    // check this condition for debugging.  If we make managers a limited
    // resource, this test would need to be in the critical path.
    if (mgr->getState() != SendManager<DataTagSenderX>::IDLE &&
        mgr->getState() != SendManager<DataTagSenderX>::WAITING_FOR_SEND)
    {
        throw Exception("Connection: getManagerForSrc out of SendManagers",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP
    
    // An idle SendManager must be set up with the buffer indices
    // showing where in the src buffer it will send from and where
    // in the receive buffer it will send to.
    // If the SendManager is not idle, these have already been set
    // on the first call to getManagerFor*
    if (mgr->getState() == SendManager<DataTagSenderX>::IDLE)
    {
        mgr->setBufferIndices(m_srcBufferIndex, m_dstBufferIndex);
        m_srcBufferIndex += m_dst.m_numInCommGroup;
        m_dstBufferIndex += m_src.m_numInCommGroup;
    }

#ifdef PVTOL_DEVELOP
    // Make sure the head buffer index at the source endpoint (which
    // indicates the buffer slot the endpoint expects to send from)
    // matches the source buffer index that the Connection is tracking.
    // This should happen automatically, but the algorithm is tricky,
    // so we check it here.
    if (mgr->getSrcBufferIndex() != m_src.m_bufferHeadIndex)
    {
        throw Exception("Connection: src buffer index mismatch in "
                        "getManagerForSrc", __FILE__, __LINE__);
    } 
#endif // PVTOL_DEVELOP

    return mgr;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
SendManager< DataTagSender<DATATYPE, TAGTYPE, USE_EOC> > *
Connection<DATATYPE, TAGTYPE, USE_EOC>::getManagerForDst()
{
#ifdef PVTOL_DEVELOP
    if (!m_dstProcLocal)
    {
        throw Exception("Connection: getManagerForDst when dst not local",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    SendManager<DataTagSenderX> * mgr =
                    m_sendManagers[m_mgrsRecvIndex].get();
    m_mgrsRecvIndex++;

#ifdef PVTOL_DEVELOP
    // With the current design, we shouldn't run out of managers, but we can
    // check this condition for debugging.  If we make managers a limited
    // resource, this test would need to be in the critical path.
    if (mgr->getState() != SendManager<DataTagSenderX>::IDLE &&
        mgr->getState() != SendManager<DataTagSenderX>::WAITING_FOR_RECV)
    {
        throw Exception("Connection: getManagerForDst out of SendManagers",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    // An idle SendManager must be set up with the buffer indices
    // showing where in the src buffer it will send from and where
    // in the receive buffer it will send to.
    // If the SendManager is not idle, these have already been set
    // on the first call to getManagerFor*
    if (mgr->getState() == SendManager<DataTagSenderX>::IDLE)
    {
        mgr->setBufferIndices(m_srcBufferIndex, m_dstBufferIndex);
        m_srcBufferIndex += m_dst.m_numInCommGroup;
        m_dstBufferIndex += m_src.m_numInCommGroup;
    }

#ifdef PVTOL_DEVELOP
    // Make sure the head buffer index at the dest endpoint (which
    // indicates the buffer slot the endpoint expects to receive into)
    // matches the dest buffer index that the Connection is tracking.
    // This should happen automatically, but the algorithm is tricky,
    // so we check it here.
    if (mgr->getDstBufferIndex() != m_dst.m_bufferHeadIndex)
    {
        throw Exception("Connection: dst buffer index mismatch in "
                        "getManagerForDst", __FILE__, __LINE__);
    } 
#endif // PVTOL_DEVELOP

    return mgr;
}//end getManagerForDst()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
SendManager< EocSender<DATATYPE, TAGTYPE, USE_EOC> > *
Connection<DATATYPE, TAGTYPE, USE_EOC>::getEocManagerForSrc()
{
#ifdef PVTOL_DEVELOP
    if (!m_srcProcLocal)
    {
        throw Exception("Connection: getEocManagerForSrc when src not local",
                        __FILE__, __LINE__);
    }

#endif // PVTOL_DEVELOP

    SendManager<EocSenderX> * mgr =
                    m_eocSendManagers[m_eocMgrsSendIndex].get();
    m_eocMgrsSendIndex++;
 
#ifdef PVTOL_DEVELOP
    // With the current design, we shouldn't run out of managers, but we can
    // check this condition for debugging.  If we make managers a limited
    // resource, this test would need to be in the critical path.
    if (mgr->getState() != SendManager<EocSenderX>::IDLE &&
        mgr->getState() != SendManager<EocSenderX>::WAITING_FOR_SEND)
    {
        throw Exception("Connection: getEocManagerForSrc out of SendManagers",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP
    
    // An idle SendManager must be set up with the buffer indices
    // showing where in the src buffer it will send from and where
    // in the receive buffer it will send to.
    // If the SendManager is not idle, these have already been set
    // on the first call to getManagerFor*
    if (mgr->getState() == SendManager<EocSenderX>::IDLE)
    {
        mgr->setBufferIndices(m_eocSrcBufferIndex, m_eocDstBufferIndex);
        m_eocSrcBufferIndex++;
        m_eocDstBufferIndex++;
    }

#ifdef PVTOL_DEVELOP
    // Make sure the head buffer index at the source endpoint (which
    // indicates the buffer slot the endpoint expects to send from)
    // matches the source buffer index that the Connection is tracking.
    // This should happen automatically, but the algorithm is tricky,
    // so we check it here.
    if (mgr->getSrcBufferIndex() != m_src.m_eocBufferHeadIndex)
    {
        throw Exception("Connection: src buffer index mismatch in "
                        "getEocManagerForSrc", __FILE__, __LINE__);
    } 
#endif // PVTOL_DEVELOP

    return mgr;
}//end getEocManagerForSrc()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
SendManager< EocSender<DATATYPE, TAGTYPE, USE_EOC> > *
Connection<DATATYPE, TAGTYPE, USE_EOC>::getEocManagerForDst()
{
#ifdef PVTOL_DEVELOP
    if (!m_dstProcLocal)
    {
        throw Exception("Connection: getEocManagerForDst when dst not local",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    SendManager<EocSenderX> * mgr =
                    m_eocSendManagers[m_eocMgrsRecvIndex].get();
    m_eocMgrsRecvIndex++;

#ifdef PVTOL_DEVELOP
    // With the current design, we shouldn't run out of managers, but we can
    // check this condition for debugging.  If we make managers a limited
    // resource, this test would need to be in the critical path.
    if (mgr->getState() != SendManager<EocSenderX>::IDLE &&
        mgr->getState() != SendManager<EocSenderX>::WAITING_FOR_RECV)
    {
        throw Exception("Connection: getEocManagerForDst out of SendManagers",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    // An idle SendManager must be set up with the buffer indices
    // showing where in the src buffer it will send from and where
    // in the receive buffer it will send to.
    // If the SendManager is not idle, these have already been set
    // on the first call to getManagerFor*
    if (mgr->getState() == SendManager<EocSenderX>::IDLE)
    {
        mgr->setBufferIndices(m_eocSrcBufferIndex, m_eocDstBufferIndex);
        m_eocSrcBufferIndex++;
        m_eocDstBufferIndex++;
    }

#ifdef PVTOL_DEVELOP
    // Make sure the head buffer index at the dest endpoint (which
    // indicates the buffer slot the endpoint expects to receive into)
    // matches the dest buffer index that the Connection is tracking.
    // This should happen automatically, but the algorithm is tricky,
    // so we check it here.
    if (mgr->getDstBufferIndex() != m_dst.m_eocBufferHeadIndex)
    {
        throw Exception("Connection: dst buffer index mismatch in "
                        "getEocManagerForDst", __FILE__, __LINE__);
    } 
#endif // PVTOL_DEVELOP

    return mgr;
}//end getEocManagerForDst()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
bool
Connection<DATATYPE, TAGTYPE, USE_EOC>::localResponsibilities(EndpointX& ep)
{
   bool rc = true, done = false;
   PvtolProgram prog;
   int myId = prog.getProcId();
   if (myId != ep.m_procId)
                  return(false);

   TaskBase &ct      = prog.getCurrentTask();
   TaskId myTaskId   = ct.getTaskID();
   TaskBase *tb      = ep.m_task;
   TaskId taskId     = tb->getTaskID();
   TaskId lasttaskId = -1;

   while (!done) {
     if (myTaskId == taskId)
       {
         done = true;
       }
      else if (taskId == lasttaskId)
       {
         done = true;
         rc   = false;
       }
      else
       {
         lasttaskId = taskId;
	 tb = &(tb->getParentTask());
	 taskId = tb->getTaskID();
       }
   }//endWhile not done

   return(rc);
}//end localResponsibilities()

}//end namespace

#endif // PVTOL_ConduitConnection_cc

