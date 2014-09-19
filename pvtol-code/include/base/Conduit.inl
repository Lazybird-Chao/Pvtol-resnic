/**
 * File: Conduit.inl
 *    March 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Methods of the Conduit, ConduitInsertIf & ConduitExtractIf
 *            The Conduit class provides a means to transport distributed
 *            data objects between two arbitrary task scopes and mappings.
 *     See Also:
 *                -ConduitConnection.inl
 *                -ConduitEndpoint.inl
 *                -ConduitSendManager.inl
 *                -ConduitSenders.inl
 *                -ConduitHelperFuncs.inl
 *
 *  $Id: Conduit.inl 945 2009-02-24 21:28:21Z jav5880 $
 *
 */
#ifndef PVTOL_Conduit_cc
#define PVTOL_Conduit_cc

#include <PvtolBasics.h>
#include <PvtolProgram.h>
#include <Conduit.h>
#include <ConduitEndpoint.h>
#include <Barrier.h>
#include <MathBasics.h>

#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>
#include <map>

#define no_DEBUG_1

using std::vector;
using std::map;
using std::cout;
using std::endl;

namespace ipvtol
{

static void findInitialBufferIndices(int srcReplicaRank,
                                     int srcNumReplicas,
                                     int dstReplicaRank,
                                     int dstNumReplicas,
                                     int * srcBufferIndex,
                                     int * dstBufferIndex)
{
    // step 1: get the modular inverse of srcNumReplicas mod dstNumReplicas
    // and vice versa.  These are combined because they can be calculated
    // simultaneously.
    int srcInverse, dstInverse;
    doubleModularInverse(srcNumReplicas, dstNumReplicas,
                         &srcInverse, &dstInverse);

    // step 2: calculate:
    // (oppositeReplicaRank - replicaRank) * numReplicas(-1)
    // mod oppositeNumReplicas
    *srcBufferIndex = ((dstReplicaRank - srcReplicaRank) * srcInverse)
                      % dstNumReplicas;
    if (*srcBufferIndex < 0)
      {
        (*srcBufferIndex) += dstNumReplicas;
      }

    *dstBufferIndex = ((srcReplicaRank - dstReplicaRank) * dstInverse)
                      % srcNumReplicas;
    if (*dstBufferIndex < 0)
      {
        (*dstBufferIndex) += srcNumReplicas;
      }

    return;
}// end findInitialBufferIndices()
 
static void doubleModularInverse(int a, int b, int * pInvA, int * pInvB)
{
    int gcd;

    extendedEuclid(a, b, pInvA, pInvB, &gcd);

#ifdef PVTOL_DEVELOP
    if (gcd != 1)
    {
        throw Exception("doubleModularInverse: not relatively prime",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    if (*pInvA < 0)
    {
        *pInvA += b;
    }

    if (*pInvB < 0)
    {
        *pInvB += a;
    }
}//end doubleModularInverse()

static void extendedEuclid(int a, int b, int * pA, int * pB, int * pGCD)
{
    int aModb = a % b;
    if (aModb == 0)
    {
        *pGCD = b;
        *pA = 1;
        *pB = 1 - (a / b);
    }
    else
    {
        extendedEuclid(b, aModb, pA, pB, pGCD);
        int newA = *pB;
        *pB = *pA - *pB * (a / b);
        *pA = newA;
    }
}//end extendedEuclid()

#ifdef NOT_READY_YET
// determine whether a map has the current process included
static bool isDirectDescendant(ProcId procId, TaskBase *tb)
 {
   bool isLocal = false;
   PvtolProgram  prog;
   ProcId myId      = prog.getProcId();
   TaskId  mytaskid = prog.getCurrentTaskId();

   if (myId == procId)
     {
       TaskId tid = tb->getTaskID();

       if (tid == mytaskid)
                isLocal = true;
        else
	 {
           bool done      = false;
           TaskId lastTid = tid;
	   while (!done) {
	      TaskBase& ptb = tb->getParentTask();
	      tid = ptb.getTaskID();
	      if (tid == lastTid)
	               done = true;
	       else if (tid == mytaskid)
	        {
                   isLocal = true;
	           done    = true;
		}
	       else
	        {
                   lastTid = tid;
		   tb = &ptb;
		}
	   }//endWhile
	 }
     }//endIf ProcIDs are equal

   return(isLocal);
 }//end isDirectDescendant()
#endif //  NOT_READY_YET

   
/////////////////////////////////////////////////////////////////////////////
// Conduit methods
/////////////////////////////////////////////////////////////////////////////

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::Conduit(const string & name,
                                             Route::Flags flags,
                                             DistributionType distType) :
            m_setupCompleteDone(false),
            m_numCommGroups(0),
            m_doLocalXfer(false),
            m_srcsPerGroup(0),
            m_dstsPerGroup(0),
            m_transposeFlag(flags),
            m_tag(-1),
            m_depth(-1)
{
#ifdef PVTOL_DEVELOP
    if ( !(flags == Route::DEFAULT_FLAG) && !(flags == TRANSPOSE_102) )
      {
        throw Exception("Conduit: specified Route flag is not implemented",
                        __FILE__, __LINE__);
      }
#endif // PVTOL_DEVELOP
    PvtolProgram   prog;
    TaskBase      &ct=prog.getCurrentTask();
    string         tname = ct.getName();
    m_name = tname + ":" + name;

    TAGTYPE           tagt;
    NullConduitTag    ntt;

    if (typeid(tagt) == typeid(ntt))
        m_useTag = false;
    else
        m_useTag = true;

    m_distType = distType;
    pthread_mutex_init(&m_thdInfoMutex, NULL);

#ifdef PVTOL_DEVELOP
    // FIXME: remove this limitation
    if (m_distType != RoundRobin)
      {
        throw Exception("Conduit: only round robin distribution is implemented",
                        __FILE__, __LINE__);
      }
#endif // PVTOL_DEVELOP

    return;
}//end Cdt Construct()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::~Conduit() throw()
{
    return;
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void *
Conduit<DATATYPE, TAGTYPE, USE_EOC>::getThreadInfo()
{
    PvtolProgram   prog;
    ProcId         procid = prog.getProcId();
    TaskBase      &ct     = prog.getCurrentTask();
    TaskId         taskid = ct.getTaskID();
    int            myRank = ct.getGlobalThreadRank();

    int   maxLens[] = {MAX_PROCS, MAX_TASKS, MAX_THREADS};
    NTuple  mapKey(3, maxLens);
    mapKey[0] = procid;
    mapKey[1] = taskid;
    mapKey[2] = myRank;

    pthread_mutex_lock(&m_thdInfoMutex);
    if (m_thdInfo.count(mapKey) == 0)
             m_thdInfo[mapKey] = new ConduitThreadInfo;

    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)(m_thdInfo[mapKey]);
    pthread_mutex_unlock(&m_thdInfoMutex);

    return((void *)myinfo);
}//end getThreadInfo()


#ifdef TEMPORARY_OFF
#endif // TEMPORARY_OFF
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupSrc(
                                                const DataMap & srcMap,
                                                int depth,
                                                const unsigned int lengths[])
{
    PvtolProgram       prog;
    TaskBase          &ct     = prog.getCurrentTask();
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    if (ct.numReplicas() == 1)
      {
        m_srcIsReplicated = false;
      }
     else
      {
        m_srcIsReplicated = true;
      }

    map<NTuple, void* >::iterator ita;
    EndpointX         *firstDstp=NULL;
    bool               done = false;

    for (ita=m_thdInfo.begin(); (!done) && (ita != m_thdInfo.end()); ita++) {
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  if (ctip->m_dstInitialized)
	    {
	      firstDstp = &(ctip->m_dst);
	      done = true;
	    }
    }//endfor ita

    // we're only going to do a local Xfer if:
    //   -we're like mapped
    //   -and we're not doing replicated tasks.
    if (firstDstp != NULL)
      {
	if (!m_srcIsReplicated && !m_destIsReplicated) //no replicas
	  {
	    if (srcMap == (*firstDstp).getMap())   // like mapped
	      {
		if (m_transposeFlag < 0x80)
		  {
		    m_doLocalXfer = true;
		    
		    myinfo->m_src.setLocalXfer(true);
		    (*firstDstp).setLocalXfer(true); 
		  }
	      }
	  }
      }//end if (m_dstInitialized)

    EndpointX & currentSrc = myinfo->m_src;

    //create the source block
    currentSrc.setup(srcMap, depth, lengths, EndpointX::SOURCE);
    myinfo->m_srcInitialized = true;

    if (m_doLocalXfer) //like mapped, no replicas, and dest has been setup
      {
       if (currentSrc.isBlockInternal() && (*firstDstp).isBlockInternal())
         {
	  if (currentSrc.getDepth() == (*firstDstp).getDepth() == 1)
	    {
	      currentSrc.setDepth(2);
	      (*firstDstp).setDepth(2);
	      int size = currentSrc.getSize() * 2;

#ifdef Support_Replicated
              m_numCommGroups = gcd(myinfo->m_src.size(), firstDstp->size());
              m_dstsPerGroup  = firstDstp->size() / m_numCommGroups;
#else
              m_numCommGroups = 1;
              m_dstsPerGroup  = 1;
#endif // Support_Replicated

	      currentSrc.rebuildDdo(size, m_dstsPerGroup);
	      myinfo->m_dst.rebindDdo((typename DATATYPE::ElType *)
	                                currentSrc.getDataBuff());
	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = currentSrc.getEocBlocks();
		  (*firstDstp).setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          currentSrc.rebuildTagBuff(currentSrc.getDepth());
	          (*firstDstp).rebindTagBuff(currentSrc.getTagAddress());
		}
	    }
	  else if (currentSrc.getSize() < (*firstDstp).getSize())
	    {/*destBlock is larger than src block */
	      /* use dest block */
	      currentSrc.setDepth((*firstDstp).getDepth());
	      currentSrc.rebindDdo((typename DATATYPE::ElType *)
	                                (*firstDstp).getDataBuff());
	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = (*firstDstp).getEocBlocks();
		  currentSrc.setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          currentSrc.rebindTagBuff(myinfo->m_dst.getTagAddress());
		}
	    }
	  else//src block is larger than the dest block
	      //use src block
	    {
	      (*firstDstp).rebindDdo((typename DATATYPE::ElType *)
	                                currentSrc.getDataBuff());
	      (*firstDstp).setDepth(currentSrc.getDepth());
	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = currentSrc.getEocBlocks();
		  (*firstDstp).setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          (*firstDstp).rebindTagBuff(currentSrc.getTagAddress());
		}
	    }
         }
	else// the block is not internal
	 {
	    m_doLocalXfer = false;
	    myinfo->m_src.setLocalXfer(false);
	    (*firstDstp).setLocalXfer(false);
	 }//end else if block is not internal
      }//end if like mapped
     else
      {
      }//endIf m_doLocalXfer
    
    myinfo->m_srcSetup = true;
    return;
}//end setupSrc()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupSrc(DATATYPE & srcObj,
                                                   int depth)
{
    PvtolProgram   prog;
    TaskBase      &ct=prog.getCurrentTask();
    ConduitThreadInfo *myinfo = getThreadInfo();

    if (ct.numReplicas() == 1)
      {
        m_srcIsReplicated = false;
      }
     else
      {
        m_srcIsReplicated = true;
      }
    
    EndpointX & currentSrc = myinfo->m_src;
    
    currentSrc.setup(srcObj, depth, EndpointX::SOURCE);

    myinfo->m_srcInitialized = true;
    myinfo->m_srcSetup       = true;
    return;
}//end setupSrc()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupDest(
                                                const DataMap & destMap,
                                                int depth,
                                                const unsigned int lengths[])
{
    PvtolProgram   prog;
    TaskBase      &ct=prog.getCurrentTask();
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

    if (ct.numReplicas() == 1)
      {
        m_destIsReplicated = false;
      }
    else
      {
        m_destIsReplicated = true;
      }

    // we're only going to do a local Xfer if: 
    //   -we're like mapped
    //   -and we're not doing replicated tasks.
    map<NTuple, void* >::iterator ita;
    EndpointX          *firstSrcp=NULL;
    bool               done = false;

    for (ita=m_thdInfo.begin(); (!done) && (ita != m_thdInfo.end()); ita++) {
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  if (ctip->m_srcInitialized)
	    {
	      firstSrcp = &(ctip->m_src);
	      done = true;
	    }
    }//endfor ita

    if (firstSrcp != NULL )
      {
	if (!m_srcIsReplicated && !m_destIsReplicated) //no replicas
	  {
	    const DataMap  &srcMap = (*firstSrcp).getMap();
	    if (srcMap == destMap)
	      {
		if (m_transposeFlag < 0x80)
		  {
		    m_doLocalXfer = true;
		    
		    (*firstSrcp).setLocalXfer(true);
		    myinfo->m_dst.setLocalXfer(true);
		  }
	      }
	  }
      }//end if (m_srcInitialized)

    EndpointX & currentDst = myinfo->m_dst;

    //create the block
    currentDst.setup(destMap, depth, lengths, EndpointX::DEST);
    myinfo->m_dstInitialized = true;

    if (m_doLocalXfer) //like mapped, no replicas, and src has been setup
      {
       if (currentDst.isBlockInternal() && (*firstSrcp).isBlockInternal())
         {
	  if (currentDst.getDepth() == (*firstSrcp).getDepth() == 1)
	    {
	      currentDst.setDepth(2);
	      (*firstSrcp).setDepth(2);
	      int size = currentDst.getSize() * 2;

#ifdef Support_Replicated
              m_numCommGroups = gcd(firstSrcp->size(), myinfo->m_dst.size());
              m_srcsPerGroup  = firstSrcp->size() / m_numCommGroups;
#else
              m_numCommGroups = 1;
              m_srcsPerGroup  = 1;
#endif // Support_Replicated

	      currentDst.rebuildDdo(size, m_srcsPerGroup);
	      (*firstSrcp).rebindDdo(
	              (typename DATATYPE::ElType *)(currentDst.getDataBuff()));

	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = currentDst.getEocBlocks();
		  (*firstSrcp).setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          currentDst.rebuildTagBuff(currentDst.getDepth());
	          (*firstSrcp).rebindTagBuff(currentDst.getTagAddress());
		}
	    }
	  else if (currentDst.getSize() < (*firstSrcp).getSize())
	    {/*srcBlock is larger than dest block */
	      /* use src block */
	      currentDst.rebindDdo((typename DATATYPE::ElType *)
	                                 ((*firstSrcp).getDataBuff()));
	      currentDst.setDepth((*firstSrcp).getDepth());
	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = (*firstSrcp).getEocBlocks();
		  currentDst.setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          currentDst.rebindTagBuff((*firstSrcp).getTagAddress());
		}
	    }
	  else//dest block is larger than the src block
	      //use dest block
	    {
	      (*firstSrcp).rebindDdo((typename DATATYPE::ElType *)
	                                         (currentDst.getDataBuff()));
	      (*firstSrcp).setDepth(currentDst.getDepth());
	      if (USE_EOC)
	        {
	          vector<SeqNum * > &ebs = currentDst.getEocBlocks();
		  (*firstSrcp).setEocBlocks(ebs);
		}

	      if (m_useTag)
	        {
	          (*firstSrcp).rebindTagBuff(currentDst.getTagAddress());
		}
	    }
         }
	else// the block is not internal
	 {
	    (*firstSrcp).setLocalXfer(false);
	    myinfo->m_dst.setLocalXfer(false);
	 }//end else if block is not internal
      }//end if like mapped

    myinfo->m_dstSetup = true;
    return;
}//end setupDest()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupDest(
                                                DATATYPE & destObj,
                                                int depth)
{
    PvtolProgram   prog;
    TaskBase      &ct=prog.getCurrentTask();
    ConduitThreadInfo *myinfo = getThreadInfo();

    if (ct.numReplicas() == 1)
      {
        m_destIsReplicated = false;
      }
    else
      {
        m_destIsReplicated = true;
      }

    EndpointX & currentDst = myinfo->m_dst;
    
    currentDst.setup(destObj, depth, EndpointX::DEST);
    myinfo->m_dstInitialized = true;
    
    myinfo->m_dstSetup = true;
    return;
}//end setupDest()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupComplete()
{
    int             maxXferSize = 2048;
    int             xferSize;

    map<NTuple, void* >::iterator ita;

    for (ita=m_thdInfo.begin(); ita != m_thdInfo.end(); ita++) {
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
          EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> endpointDescript;

	  if (ctip->m_srcInitialized)
	    {
              (ctip->m_src).getDescript(endpointDescript);
              xferSize = endpointDescript.getSerializedSize();

              if (xferSize > maxXferSize)
                        maxXferSize = xferSize;
	    }

	  if (ctip->m_dstInitialized)
	    {
              ctip->m_dst.getDescript(endpointDescript);
              xferSize = endpointDescript.getSerializedSize();

              if (xferSize > maxXferSize)
                        maxXferSize = xferSize;
	    }
    }//endfor ita

    ConduitInitComm initComm(m_name, maxXferSize);

    setupComplete(initComm, m_useTag);

    if (m_doLocalXfer)
      {
        EndpointX  *firstSrcp=NULL;
        bool       done = false;

        for (ita=m_thdInfo.begin();
	     (!done) && (ita != m_thdInfo.end());
	     ita++) {
              ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	      if (ctip->m_srcInitialized)
	        {
	          firstSrcp = &(ctip->m_src);
	          done = true;
		}
        }//endfor ita

	int depth = (*firstSrcp).getDepth();
        m_localXferBufferHeadIndex.setBufSize(depth);
        m_localXferBufferHeadIndex = 0;
        m_localXferBufferTailIndex.setBufSize(depth);
        m_localXferBufferTailIndex = 0;
	m_depth = depth;

        PvtolProgram   prog;
        TaskBase  &ct  = prog.getCurrentTask();
        m_clxdKey      = ct.getTransferKey(ct.getLocalThreadRank());
	pthread_mutex_t &clxdDbMutex = ct.getItxdDbMutex();
	pthread_mutex_lock(&clxdDbMutex);
	map<int, CdtLocalXferData>& clxdDb = ct.getClxdDb();
	map<int, CdtLocalXferData>::iterator it;
	it = clxdDb.find(m_clxdKey);
	if (it == clxdDb.end())
	  {//   Create and init entry
	     pthread_mutex_init(&(clxdDb[m_clxdKey].clxdMutex), NULL);
	     pthread_mutex_init(&(clxdDb[m_clxdKey].clxdCvMutex), NULL);
	     pthread_cond_init(&(clxdDb[m_clxdKey].clxdCond), NULL);
	     clxdDb[m_clxdKey].sendType        = NONE;
	     clxdDb[m_clxdKey].insertPosted    = 0;
	     clxdDb[m_clxdKey].eocPosted       = 0;
	     clxdDb[m_clxdKey].insertWaiting   = 0;
	     clxdDb[m_clxdKey].extractWaiting  = 0;
	     clxdDb[m_clxdKey].noBuffAvailable = false;
	     clxdDb[m_clxdKey].sReq            = NULL;
	     clxdDb[m_clxdKey].destAddr        = NULL;
	     clxdDb[m_clxdKey].srcAddr         = NULL;
	  }

	pthread_mutex_unlock(&clxdDbMutex);
	m_clxdEntry = &(clxdDb[m_clxdKey]);
      }//endIf no movement necessary

}//end setupComplete()
 

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::setupComplete(
        Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm & initComm,
        bool useTag)
{
    Barrier bar;
    PvtolProgram   prog;
    int procId    = prog.getProcId();
    TaskBase&  ct = prog.getCurrentTask();

    map<NTuple, void* >::iterator ita;

    initComm.exchangeEndpoints(m_thdInfo, m_name);

    // Make sure the sources and destinations agree on the
    // essential elements of what is being sent or
    // received
    validateEndpoints();

    // Now that we have the number of replicas on each side, we
    // can complete the endpoint setup
    // since the # of replicas is independant of the thread,
    //       just use the 1st
 
    //   Since we are SPMD all the srcs would have set up identically
    //     and all the dests would also have been set up identically
    EndpointX          *firstSrcp=NULL;
    EndpointX          *firstDstp=NULL;
    bool               done = false;

    for (ita=m_thdInfo.begin(); (!done) && (ita != m_thdInfo.end()); ita++) {
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  if ((ctip->m_src.isInitialized())
	                  &&
	      ctip->m_srcInitialized)
	               firstSrcp = &(ctip->m_src);

	  if ((ctip->m_dst.isInitialized())
	                  &&
	      ctip->m_dstInitialized)
	               firstDstp = &(ctip->m_dst);

	  if ((firstSrcp != NULL ) && (firstDstp != NULL))
	              done = true;
    }//endfor ita

#ifdef Support_Replicated
    m_numCommGroups = gcd((*firstSrcp).size(), (*firstDstp).size());
    m_srcsPerGroup  = (*firstSrcp).size() / m_numCommGroups;
    m_dstsPerGroup  = (*firstDstp).size() / m_numCommGroups;
#else
    m_numCommGroups = 1;
    m_srcsPerGroup  = 1;
    m_dstsPerGroup  = 1;
#endif // Support_Replicated

    unsigned maxSrcNumRanks = 0;
    char ostr[128];

    sprintf(ostr,"%s(%s)",m_name.c_str(),"src_ep");
    string os=ostr;
    firstSrcp->finalSetup(os, m_srcsPerGroup,
			  m_dstsPerGroup, 0, useTag, 0);
   const RankList &rl = firstSrcp->getMap().getRankList();
   int numRanks = rl.getNumRanks();
   if (static_cast<unsigned int>(numRanks) > maxSrcNumRanks)
     {
       maxSrcNumRanks = numRanks;
     }

   sprintf(ostr,"%s(%s)",m_name.c_str(),"dst_ep");
   os=ostr;
   firstDstp->finalSetup(os, m_dstsPerGroup, m_srcsPerGroup,
                         0 /*i / m_numCommGroups*/, useTag, maxSrcNumRanks);
 
    if ((firstSrcp != NULL) && (firstDstp != NULL))
      {
#ifdef Support_Replicated
        for (unsigned int commGroup = 0;
    	     commGroup < m_numCommGroups;
	     commGroup++) {
          unsigned int firstSource, lastSource, firstDest, lastDest;
          firstSource = commGroup;
          lastSource  = firstSource + (m_srcsPerGroup-1) * m_numCommGroups;
          firstDest   = commGroup;
          lastDest    = firstDest + (m_dstsPerGroup-1) * m_numCommGroups;

          for (unsigned sourceNum = firstSource;
               sourceNum <= lastSource;
               sourceNum += m_numCommGroups) {
            for (unsigned destNum = firstDest;
                 destNum <= lastDest;
                 destNum += m_numCommGroups) {
	      shared_ptr<Connection<DATATYPE, TAGTYPE, USE_EOC> >
                        mcP(
                            new Connection<DATATYPE, TAGTYPE, USE_EOC>
                            ((*firstSrcp)[sourceNum],
                             (*firstDstp)[destNum], 
                             useTag,
                             m_transposeFlag) );

              (*firstSrcp)[sourceNum].addConnection(mcP);
              (*firstDstp)[destNum].addConnection(mcP);
            }//end for each dest
          }//end for each src
        }//end for each commgroup 
#else
        Connection<DATATYPE, TAGTYPE, USE_EOC> *connp =
                new Connection<DATATYPE, TAGTYPE, USE_EOC> ((*firstSrcp),
                                                            (*firstDstp), 
                                                            useTag,
                                                            m_transposeFlag);

        (*firstSrcp).addConnection(connp);
        (*firstDstp).addConnection(connp);
#ifdef _DEBUG_2
        cout << "Cdt[" << procId << "] just built conn with EPs:" << endl
	     << "     " << *firstSrcp << endl
	     << "     " << *firstDstp << endl;
#endif // _DEBUG_2
#endif // Support_Replicated
      }//end there is a src or dest here

#ifdef _DEBUG_2
    int threadid = ct.getGlobalThreadRank();
    int taskid   = ct.getTaskID();
    char prefix[96];
    sprintf(prefix, "Cdt[%d, %d, %d] ",
            procId, taskid, threadid);

    map<NTuple, void * >::iterator ita1;

    sleep(procId * 2);
    cout << prefix << "at end of setupC" << endl;
    for (ita1=m_thdInfo.begin(); ita1 != m_thdInfo.end(); ita1++) {
       NTuple  thekey = (*ita1).first;
       cout << "     tiNT=" << thekey << endl;
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita1).second;
          cout << " srcInited=" << ctip->m_srcInitialized
               << ", dstInited=" << ctip->m_dstInitialized
               << ", srcSetup=" << ctip->m_srcSetup
               << ", dstSetup=" << ctip->m_dstSetup << endl;
	  cout << ctip->m_src << endl;
	  cout << ctip->m_dst << endl;
    }//endfor ita
#endif // _DEBUG_2

   m_setupCompleteDone = true;

   if (firstDstp != NULL)
         (*firstDstp).beginPrepost();

   bar.synch();

   return;
}//end setupComplete()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::insert()
{
    if (!insertAvailable())
                  waitForInsert();

    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
#ifdef _DEBUG_2
    PvtolProgram   prog;
    int procId   = prog.getProcId();
    cout << "Cdt[" << procId << "] about to insert EP is "
         << myinfo->m_src << endl;
#endif // _DEBUG_2
    myinfo->m_src.insert();
    if (m_doLocalXfer)
      {
         insertLocalData();
      }

    return;
}//end insert()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
DATATYPE & Conduit<DATATYPE, TAGTYPE, USE_EOC>::getExtractHandle()
{
    ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

#ifdef _DEBUG_2
    cout << "Cdt:" << m_name << " getExtractHandle() Tail="
         << myinfo->m_dst.getTailBuffIdx()
         << " Head=" << myinfo->m_dst.getHeadBuffIdx()
         << " DataAdd=" << myinfo->m_dst.getDataAddress()
         << endl;
#endif // _DEBUG_2

   if (m_doLocalXfer)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
       if (!(m_clxdEntry->insertPosted) && !(m_clxdEntry->eocPosted))
         {
             pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
             waitForLocalData();
         }
        else
             pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
     }
    else
     {//   not local
        if (!extractReady())
          {//   a blocking call
	      myinfo->m_dst.waitForExtractBuff();
          }//endIf not Ready
     }//endIf not local

    return *(myinfo->m_dst.getDataAddress());
}//end getExtractHandle()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
TAGTYPE &
Conduit<DATATYPE, TAGTYPE, USE_EOC>::getExtractTagHandle()
{
   ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();

   if (m_doLocalXfer)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
       if (!(m_clxdEntry->insertPosted) && !(m_clxdEntry->eocPosted))
         {
             pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
             waitForLocalData();
         }
        else
             pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));

       return myinfo->m_dst.getTagHandleRef();
     }
    else
     {//   not local
       return myinfo->m_dst.getExtractTagHandle();
     }//endIf not local

}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::insertLocalData()
{
   pthread_mutex_lock(&(m_clxdEntry->clxdMutex));

   ++(m_clxdEntry->insertPosted);

   ++m_localXferBufferHeadIndex;
   if ((m_localXferBufferHeadIndex == m_localXferBufferTailIndex)
                             &&
       (m_clxdEntry->insertPosted == m_depth))
     {
       m_clxdEntry->noBuffAvailable = true;
     }

   if (m_clxdEntry->extractWaiting)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
       pthread_cond_signal(&(m_clxdEntry->clxdCond));
       pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
     }

   pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));

   return;
}//end insertLocalData()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::insertLocalEOC()
{
   pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
   ++(m_clxdEntry->eocPosted);

   if (m_clxdEntry->extractWaiting)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
       pthread_cond_signal(&(m_clxdEntry->clxdCond));
       pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
     }

   pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));

   return;
}//end insertLocalEOC()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Conduit<DATATYPE, TAGTYPE, USE_EOC>::isAtEOC()
{
   bool  rc=false;
   ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
   if (m_doLocalXfer)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
       if (m_clxdEntry->eocPosted)
                  rc = myinfo->m_dst.isAtEOC();

       pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
     }
    else
       rc = myinfo->m_dst.isAtEOC();

    return(rc);
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::waitForInsert()
{
   if (m_doLocalXfer)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
       if (!insertAvailable() || !(m_clxdEntry->insertPosted))
         {
           pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
           ++(m_clxdEntry->extractWaiting);
           pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
           pthread_cond_wait(&(m_clxdEntry->clxdCond),
	                 &(m_clxdEntry->clxdCvMutex));
           --(m_clxdEntry->extractWaiting);
           pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
         }
        else
         {
           --(m_clxdEntry->insertPosted);
           pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
         }
     }
    else
     {//    data is being moved between procs
        ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
	myinfo->m_src.waitForInsertBuff();
     }//endIf Local

   return;
}//end waitForInsert()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::waitForInsertBuff()
{
   if (m_doLocalXfer)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
       if (m_clxdEntry->noBuffAvailable)
         {
           pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
           ++(m_clxdEntry->insertWaiting);
           pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
           pthread_cond_wait(&(m_clxdEntry->clxdCond),
	                 &(m_clxdEntry->clxdCvMutex));
           pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
           --(m_clxdEntry->insertWaiting);
         }
        else
         {
           pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
         }
     }
    else
     {//    data is being moved between procs
        ConduitThreadInfo *myinfo = (ConduitThreadInfo *)getThreadInfo();
	myinfo->m_src.waitForInsertBuff();
     }//endIf Local

   return;
}//end waitForInsertBuff()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::waitForLocalData()
{
   pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
   if (!(m_clxdEntry->insertPosted) && !(m_clxdEntry->eocPosted))
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
       ++(m_clxdEntry->extractWaiting);
       pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
       pthread_cond_wait(&(m_clxdEntry->clxdCond),
	                 &(m_clxdEntry->clxdCvMutex));
       pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
       --(m_clxdEntry->extractWaiting);
     }
    else
     {
       pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));
     }

   return;
}//end waitForLocalData()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::localRelease()
{
   pthread_mutex_lock(&(m_clxdEntry->clxdMutex));
   ++m_localXferBufferTailIndex;

   if (m_clxdEntry->insertWaiting)
     {
       pthread_mutex_lock(&(m_clxdEntry->clxdCvMutex));
       pthread_cond_signal(&(m_clxdEntry->clxdCond));
       pthread_mutex_unlock(&(m_clxdEntry->clxdCvMutex));
     }

   --(m_clxdEntry->insertPosted);
   m_clxdEntry->noBuffAvailable = false;
   pthread_mutex_unlock(&(m_clxdEntry->clxdMutex));

   return;
}//end localRelease()

// Make sure all the endpoints in this mode have
// consistent sizes
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::validateEndpoints()
{
 
//   Since we are SPMD all the srcs would have set up identically
//     and all the dests would also have been set up identically
    map<NTuple, void* >::iterator ita;
    EndpointX                     *firstSrcp=NULL;
    EndpointX                     *firstDstp=NULL;
    bool                          done = false;

    for (ita=m_thdInfo.begin(); (!done) && (ita != m_thdInfo.end()); ita++) {
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  if (ctip->m_srcInitialized)
	               firstSrcp = &(ctip->m_src);

	  if (ctip->m_dstInitialized)
	               firstDstp = &(ctip->m_dst);

	  if ((firstSrcp != NULL ) && (firstDstp != NULL))
	              done = true;
    }//endfor ita

    // compare the size of the first source to that of all
    // other sources
    //                          TBD

    // compare the size of the first source to that of all
    // of the destinations
    //                          TBD

    return;
}//end validateEndpoints()


/////////////////////////////////////////////////////////////////////////////
// ConduitInitComm methods
/////////////////////////////////////////////////////////////////////////////
template< class DATATYPE, class TAGTYPE, bool USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm::ConduitInitComm(
                                        const string& name,
					int           maxSize)
    : m_name(name),
      m_transferSize(maxSize)
{
    PvtolProgram   prog;
    TaskBase      &ct = prog.getCurrentTask();
    CommScope     &cs = ct.getCommScope();
    int        myrank = cs.rank(cs.getProcId());
    int numRanks      = cs.getNumProcs();
    m_masterIsLocal   = (0 == myrank);
    m_initCommBuff    = new int[m_transferSize];

    for( int i=1; i < numRanks; i++ )
    {
        Transfer* to(new Transfer(i,
	                          reinterpret_cast<char *>(m_initCommBuff),
                                  0,
	                          reinterpret_cast<char *>(m_initCommBuff),
                                  m_transferSize) );

        Transfer* fm(new Transfer(0,
	                          reinterpret_cast<char *>(m_initCommBuff),
                                  i,
	                          reinterpret_cast<char *>(m_initCommBuff),
                                  m_transferSize) );

        m_toMaster.push_back(to);
        m_fromMaster.push_back(fm);
    }

    return;
}//end construct ConduitInitComm()


template< class DATATYPE, class TAGTYPE, bool USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm::~ConduitInitComm()
{
   for (unsigned int i=0; i < m_toMaster.size(); i++)
                    delete m_toMaster[i];

   for (unsigned int i=0; i < m_fromMaster.size(); i++)
                    delete m_fromMaster[i];

   delete [] m_initCommBuff;

   return;
}//end ~ConduitInitComm()

// This method implements the protocol by which all nodes in the parent
// task discover all the sources and destinations
template < class DATATYPE, class TAGTYPE, bool USE_EOC>
void Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitInitComm::exchangeEndpoints(
                      map<NTuple, void* >& thdInfo,
                      const string & name)
{
    PvtolProgram   prog;
    int      myprocid = prog.getProcId();
    TaskBase      &ct = prog.getCurrentTask();
    int      numprocs = prog.numProcs();
    CommScope     &cs = ct.getCommScope();
    int    myProcRank = cs.rank(cs.getProcId());
    int  myLocThdRank = ct.getLocalThreadRank();
#ifdef _DEBUG_2
    int procId   = prog.getProcId();
    int threadid = ct.getGlobalThreadRank();
    int taskid   = ct.getTaskID();
    char prefix[96];
    sprintf(prefix, "Exchg[%d, %d, %d] ",
            procId, taskid, threadid);
#endif // _DEBUG_2

    EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> endpointDescript;
    int numMessages;
    int maxMsgSize;
    int *msgBuff;

    EPtype epType;

    for (int n=0; n<2; n++) {
      if (n == 0)
            epType = EndpointX::SOURCE;
       else
            epType = EndpointX::DEST;

      if (m_masterIsLocal && (myLocThdRank == 0))
        {
          // receive from everyone except myself in order
          // this code assumes master is index 0, which is no loss because
          // that is already assumed by the code which constructs the
          // toMaster and fromMaster transfers
          for (int recvProc = 1; recvProc < numprocs; recvProc++) {
            // since 0 is the master, there is no transfer corresponding to it,
            // so our the transfer from recvNode is in recvNode - 1
            m_toMaster[recvProc - 1]->send(2*sizeof(int));

            msgBuff      = beginLocal();
            numMessages  = msgBuff[0];
	    maxMsgSize   = msgBuff[1];

#ifdef PVTOL_DEVELOP
	    if (m_transferSize < maxMsgSize)
	      {
                throw Exception("ConduitInitComm: Msg Size too big! ",
                                __FILE__, __LINE__);
	      }
#endif // PVTOL_DEVELOP
            for (int i = 0; i < numMessages; i++) {
                // since 0 is the master, there is no transfer corresponding
                // to it, so our the transfer from recvNode is in recvNode - 1
                m_toMaster[recvProc - 1]->send(size());

                endpointDescript.deserialize(beginLocal());

		TaskId eptask = endpointDescript.getTaskId();
		int  epthread = endpointDescript.getThreadId();
		int  epprocid = endpointDescript.getProcId();

                int   maxLens[] = {MAX_PROCS, MAX_TASKS, MAX_THREADS};
                NTuple  mapKey(3, maxLens);
                mapKey[0] = epprocid;
                mapKey[1] = eptask;
                mapKey[2] = epthread;

                if (thdInfo.count(mapKey) == 0)
		  {
                    (thdInfo[mapKey]) = (void *)new ConduitThreadInfo;
		  }

                // put this endpoint in its proper place
		if ((epType == EndpointX::SOURCE)                  &&
	            (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_src.isInitialized())) &&
		    (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_srcInitialized)))
		  {
		    ((ConduitThreadInfo *)
		         (thdInfo[mapKey]))->m_src.setup(
                                                   endpointDescript, epType);
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_srcInitialized = true;
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_srcSetup = true;
		  }

	         if ((epType == EndpointX::DEST)                   &&	
	            (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_dst.isInitialized())) &&
		    (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_dstInitialized)))
		  {
		    ((ConduitThreadInfo *)
		         (thdInfo[mapKey]))->m_dst.setup(
                                                   endpointDescript, epType);
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_dstInitialized = true;
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_dstSetup = true;
		  }
            }//endFor each msg
          }//endFor each rcv node

          map<NTuple, void* >::iterator ita;
          numMessages = 0;

          for (ita=thdInfo.begin(); ita != thdInfo.end(); ita++) {
                ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
		if (epType == EndpointX::SOURCE)
		  {
	            if (ctip->m_src.isInitialized() && ctip->m_srcInitialized)
	                             ++numMessages;
		  }
                 else
		  {
	            if (ctip->m_dst.isInitialized() && ctip->m_dstInitialized)
	                             ++numMessages;
		  }
          }//endfor ita

          // Send all the endpoint information to each node besides the master
          // (besides me)
          for (int sendProc = 1; sendProc < ct.getNumProcesses(); sendProc++)
          {
            // send the number of msgs to expect
            msgBuff    = beginLocal();
            msgBuff[0] = numMessages;
	    msgBuff[1] = m_transferSize;

            m_fromMaster[sendProc - 1]->send(2*sizeof(int));

            for (ita=thdInfo.begin(); ita != thdInfo.end(); ita++) {
                  ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  	  if (epType == EndpointX::SOURCE)
		    {
	              if (ctip->m_src.isInitialized() && ctip->m_srcInitialized)
		        {
                          if (myprocid == ctip->m_src.procId())
			                ctip->m_src.calcSetupCompleteRanks();

                          ctip->m_src.getDescript(endpointDescript);
                          endpointDescript.setNumReplicas(1);
                          endpointDescript.setReplicaRank(0);
                          endpointDescript.serialize(beginLocal());
                          m_fromMaster[sendProc - 1]->send(m_transferSize);
			}
		    }
                   else
		    {
	              if (ctip->m_dst.isInitialized() && ctip->m_dstInitialized)
		        {
                          if (myprocid == ctip->m_dst.procId())
			                ctip->m_dst.calcSetupCompleteRanks();

                          ctip->m_dst.getDescript(endpointDescript);
                          endpointDescript.setNumReplicas(1);
                          endpointDescript.setReplicaRank(0);
                          endpointDescript.serialize(beginLocal());
                          m_fromMaster[sendProc - 1]->send(m_transferSize);
			}
		    }
            }//endfor ita
          }//endFor each send Node
        }
      else if (myLocThdRank == 0)
        {
          // count the number of messages to send to the comm master
          map<NTuple, void* >::iterator ita;
          numMessages = 0;

          for (ita=thdInfo.begin(); ita != thdInfo.end(); ita++) {
                ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
		if (epType == EndpointX::SOURCE)
		  {
	            if (ctip->m_src.isInitialized() && ctip->m_srcInitialized)
	                             ++numMessages;
		  }
                 else
		  {
	            if (ctip->m_dst.isInitialized() && ctip->m_dstInitialized)
	                             ++numMessages;
		  }
          }//endfor ita

          // send the master a message telling it how many EndpointInfos
          // I will send
          msgBuff    = beginLocal();
          msgBuff[0] = numMessages;
	  msgBuff[1] = m_transferSize;

          // since 0 is the master, there is no transfer corresponding to it,
          // so our the transfer from myProcRank is in myProcRank - 1
          m_toMaster[myProcRank - 1]->send(2*sizeof(int));

          for (ita=thdInfo.begin(); ita != thdInfo.end(); ita++) {
                  ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
	  	  if (epType == EndpointX::SOURCE)
		    {
	              if (ctip->m_src.isInitialized() && ctip->m_srcInitialized)
		        {
                          if (myprocid == ctip->m_src.procId())
			                ctip->m_src.calcSetupCompleteRanks();

                          ctip->m_src.getDescript(endpointDescript);
                          endpointDescript.setNumReplicas(1);
                          endpointDescript.setReplicaRank(0);
                          endpointDescript.serialize(beginLocal());
                          m_toMaster[myProcRank - 1]->send(m_transferSize);
			}
		    }
                   else
		    {
	              if (ctip->m_dst.isInitialized() && ctip->m_dstInitialized)
		        {
                          if (myprocid == ctip->m_dst.procId())
			                ctip->m_dst.calcSetupCompleteRanks();

                          ctip->m_dst.getDescript(endpointDescript);
                          endpointDescript.setNumReplicas(1);
                          endpointDescript.setReplicaRank(0);
                          endpointDescript.serialize(beginLocal());
                          m_toMaster[myProcRank - 1]->send(m_transferSize);
			}
		    }
          }//endfor ita
        
          // now, receive the entire set of endpoints from the master
          m_fromMaster[myProcRank - 1]->send(2*sizeof(int));

          msgBuff      = beginLocal();
          numMessages  = msgBuff[0];
	  maxMsgSize   = msgBuff[1];

#ifdef PVTOL_DEVELOP
	  if (m_transferSize < maxMsgSize)
	    {
              throw Exception("ConduitInitComm: Msg Size too big! ",
                                __FILE__, __LINE__);
	    }
#endif // PVTOL_DEVELOP
          for (int i = 0; i < numMessages; i++) {
              // since 0 is the master, there is no transfer corresponding
              // to it, so our the transfer from recvNode is in recvNode - 1
              m_fromMaster[myProcRank - 1]->send(size());

              endpointDescript.deserialize(beginLocal());

	      TaskId eptask = endpointDescript.getTaskId();
	      int  epthread = endpointDescript.getThreadId();
	      int  epprocid = endpointDescript.getProcId();

              int   maxLens[] = {MAX_PROCS, MAX_TASKS, MAX_THREADS};
              NTuple  mapKey(3, maxLens);
              mapKey[0] = epprocid;
              mapKey[1] = eptask;
              mapKey[2] = epthread;

              if (thdInfo.count(mapKey) == 0)
	        {
                  (thdInfo[mapKey]) = (void *)new ConduitThreadInfo;
		}
                
              // put this endpoint in its proper place
	      if ((epType == EndpointX::SOURCE)                  &&
	          (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_src.isInitialized())) &&
		  (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_srcInitialized)))
	        {
	          ((ConduitThreadInfo *)
		        (thdInfo[mapKey]))->m_src.setup(
                                                  endpointDescript, epType);
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_srcInitialized = true;
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_srcSetup = true;
	        }

	      if ((epType == EndpointX::DEST)                   &&	
	          (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_dst.isInitialized())) &&
		  (!(((ConduitThreadInfo *)(thdInfo[mapKey]))->m_dstInitialized)))
	        {
	          ((ConduitThreadInfo *)
		         (thdInfo[mapKey]))->m_dst.setup(
                                                   endpointDescript, epType);
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_dstInitialized = true;
	          ((ConduitThreadInfo *)
		     (thdInfo[mapKey]))->m_dstSetup = true;
	        }
          }//endFor each msg

        }//endIf master is local
    }//endFor Src & Dest

#ifdef _DEBUG_2
    map<TaskId, map<int, void* > >::iterator ita;

    sleep(procId * 3);
    cout << prefix << "at end of exchgEP" << endl;
    for (ita=thdInfo.begin(); ita != thdInfo.end(); ita++) {
       cout << "     NTup=" << (*ita).first << endl;
          ConduitThreadInfo *ctip = (ConduitThreadInfo *)(*ita).second;
          cout << "     srcInited=" << ctip->m_srcInitialized
               << ", dstInited=" << ctip->m_dstInitialized
               << ", srcSetup=" << ctip->m_srcSetup
               << ", dstSetup=" << ctip->m_dstSetup << endl;
	  cout << ctip->m_src << endl;
	  cout << ctip->m_dst << endl;
       }//endfor itb
    }//endfor ita
#endif // _DEBUG_2

   return;
}//end exchangeEndpoints()



/////////////////////////////////////////////////////////////////////////////
// ConduitThreadInfo methods
/////////////////////////////////////////////////////////////////////////////
template< class DATATYPE, class TAGTYPE, bool USE_EOC>
Conduit<DATATYPE, TAGTYPE, USE_EOC>::ConduitThreadInfo::ConduitThreadInfo()
    : m_srcInitialized(false),
      m_srcSetup(false),
      m_dstInitialized(false),
      m_dstSetup(false)
{
    return;
}//end construct ConduitThreadInfo()


}//end Namspace

#endif // PVTOL_Conduit_cc


