/**
 * File: ConduitEndpoint.inl
 *    March 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Methods of the Conduit Endpoint class template.
 *           Methods of the Conduit EndpointDescript class template. 
 *
 *  $Id: ConduitEndpoint.inl 945 2009-02-24 21:28:21Z jav5880 $
 *
 */
#ifndef PVTOL_ConduitEndpoint_cc
#define PVTOL_ConduitEndpoint_cc
#include <PvtolBasics.h>
#include <Conduit.h>
#include <PvtolProgram.h>
#include <ConduitEndpoint.h>
#include <ConduitConnection.h>
#include <Exception.h>

#include <stdio.h>
#include <utility>
#include <iostream>

#define no_DEBUG_1

namespace ipvtol
{

using namespace std;


template< class DATATYPE, class TAGTYPE, bool USE_EOC >
EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::EndpointDescript() :
    m_depth(-1),
    m_numReplicas(0),
    m_replicaRank(0),
    m_isSrc(0),
    m_isDst(0),
    m_isLocalXfer(0),
    m_task(NULL),
    m_taskid(-1),
    m_threadid(-1),
    m_numInProcList(0),
    m_numInSucRanks(0),
    m_map(NULL)
{
    return;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
int * EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::serialize(int * buf)
{
#ifdef _DEBUG_5
    PvtolProgram   prog;
    cout << "In serialize...." << endl;
    cout << "this: " << this << endl;
    cout << "On processor: " << prog.getProcId() << endl;

#endif

#ifdef PVL_DEVELOP
    int * oldbuf = buf;
#endif

    // copy various other fields into the buffer

    *(buf++) = m_depth;
    *(buf++) = m_numReplicas;
    *(buf++) = m_replicaRank;
    *(buf++) = m_procId;
    *(buf++) = m_isSrc;
    *(buf++) = m_isDst;
    *(buf++) = m_isLocalXfer;

    union {
      TaskBase * tb;
      int        i[2];
    } u;

    u.tb = m_task;
    *(buf++) = u.i[0];
    if (sizeof(TaskBase *) > sizeof(int))
                   *(buf++) = u.i[1];

    *(buf++) = m_taskid;
    *(buf++) = m_threadid;

    for(int i=0; i<MAX_DIM; i++) {
        *(buf++) = m_lengths[i];
    }

    *(buf++) = m_numInProcList;
    for(int i=0; i<MAX_EP_PROCESSES; i++)
               *(buf++) = m_procList[i];

    *(buf++) = m_numInSucRanks;
    for(int i=0; i<MAX_EP_PROCESSES; i++)
               *(buf++) = m_sucRanks[i];
 
    buf = m_map->serialize(buf);

#ifdef PVL_DEVELOP
    if (buf - oldbuf > maxSerializedSize())
    {
        throw Exception(
            "EndpointDescript: Internal Error: Serialized size > "
            "maxSerializedSize", __FILE__, __LINE__);
    }
#endif
#ifdef _DEBUG_1
    PvtolProgram  prog;
    int procId     = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid   = ct.getTaskID();
    int threadid = ct.getGlobalThreadRank();
    const RankList& rl = m_map->getRankList();
    for (int i=0; i<rl.getNumRanks(); i++) {
       int rank = rl.getRank(i);
       if ((rank < 0) || (rank > 3))
         {
           cout << "EPD[" << procId << "," << taskid << "," << threadid
	        << "] serialize and rank=" << rank
                << endl;
	 }
    }//endFor i
#endif // _DEBUG_1

    return buf;
}//end serialize()

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
int * EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::deserialize(int * buf)
{
#ifdef _DEBUG_5
    PvtolProgram   prog;
    cout << "In deserialize...." << endl;
    cout << "this: " << this << endl;
    cout << "On processor: " << prog.getProcId() << endl;
#endif

    // Read out various other fields
    m_depth       = *(buf++);
    m_numReplicas = *(buf++);
    m_replicaRank = *(buf++);
    m_procId      = *(buf++);
    m_isSrc       = *(buf++);
    m_isDst       = *(buf++);
    m_isLocalXfer = *(buf++);

    union {
      TaskBase * tb;
      int        i[2];
    } u;
    u.i[0] = *(buf++);
    if (sizeof(TaskBase *) > sizeof(int))
                 u.i[1] = *(buf++);

    m_task     = u.tb;
    m_taskid   = *(buf++); 
    m_threadid = *(buf++); 

    for(int i=0; i<MAX_DIM; i++)
    {

#ifdef _DEBUG_5
        PvtolProgram   prog;
        cout << "On processor: " << prog.getProcId() << "   m_lengths["
             << i << "]: " << m_lengths[i] << endl;
#endif
        m_lengths[i] = *(buf++);
    }

    m_numInProcList = *(buf++);
    for(int i=0; i<MAX_EP_PROCESSES; i++)
               m_procList[i] = *(buf++);

    m_numInSucRanks = *(buf++);
    for(int i=0; i<MAX_EP_PROCESSES; i++)
               m_sucRanks[i] = *(buf++);

    if (m_map != NULL)
          delete m_map;

    m_map = new DataMap;
    buf = m_map->deserialize( buf );
#ifdef _DEBUG_1
    PvtolProgram  prog;
    int procId     = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid   = ct.getTaskID();
    int threadid = ct.getGlobalThreadRank();
    const RankList& rl = m_map->getRankList();
    for (int i=0; i<rl.getNumRanks(); i++) {
       int rank = rl.getRank(i);
       if ((rank < 0) || (rank > 3))
         {
           cout << "EPD[" << procId << "," << taskid << "," << threadid
	        << "] deserialize and rank=" << rank
                << endl;
	 }
    }//endFor i
#endif // _DEBUG_1

    return buf;
}//end deserialize()

 
// Return the size of the block of type int that is the maximum needed
// to serialize the EndpointDescript
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
int EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getSerializedSize()
{
    int maxSize = 0;

    maxSize += 3; // space for m_depth, m_numReplicas, m_replicaRank
    maxSize += 4; // space for m_procId, m_isSrc, m_isDst, m_isLocalXfer
    maxSize += 2; // space for m_taskid, m_threadid
    int taskPtrSize = sizeof(TaskBase *)/sizeof(int);
    if ((taskPtrSize * sizeof(int)) <  sizeof(TaskBase *))
              ++taskPtrSize;

    maxSize += taskPtrSize;

    maxSize += MAX_DIM; // space for the m_lengths, one int for each dimension
    maxSize += MAX_EP_PROCESSES + 1;//space for m_procList & m_numInProcList
    maxSize += MAX_EP_PROCESSES + 1;//space for m_sucRanks & m_numInsucRanks

    maxSize += m_map->getSerializedSize(); // space for the map;

    return maxSize;
}//end getSerializedSize()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
Endpoint<DATATYPE, TAGTYPE, USE_EOC>::Endpoint() :
    m_map(NULL),
    m_buff(NULL),
    m_internalBuff(false),
    m_block(NULL),
    m_internalBlock(false),
    m_ddo(NULL),
    m_internalObject(false),   
    m_internalTag(false),
    m_internalEoc(false),
    m_tagBlock(NULL),
    m_tagHandlePtr(NULL), 
    m_initialized(false),
    m_slotSize(0),
    m_task(NULL),
    m_taskid(-1),
    m_threadid(-1),
    m_procId(-1),
    m_prepostsRequested(PREPOST_FOREVER),
    m_beginPrepostDone(false),
    m_localXfer(false)
{
    return;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
Endpoint<DATATYPE, TAGTYPE, USE_EOC>::Endpoint(const string& dataName) :
    m_map(NULL),
    m_buff(NULL),
    m_internalBuff(false),
    m_block(NULL),
    m_internalBlock(false),
    m_ddo(NULL),
    m_internalObject(false),   
    m_internalTag(false),
    m_internalEoc(false),
    m_tagBlock(NULL),
    m_tagHandlePtr(NULL), 
    m_name(dataName),
    m_initialized(false),
    m_slotSize(0),
    m_task(NULL),
    m_taskid(-1),
    m_threadid(-1),
    m_procId(-1),
    m_prepostsRequested(PREPOST_FOREVER),
    m_beginPrepostDone(false),
    m_localXfer(false)
{
    return;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
Endpoint<DATATYPE, TAGTYPE, USE_EOC>::~Endpoint( )
{
    if (m_map != NULL)
                delete m_map;

    if ( ( m_ddo != static_cast<DATATYPE*>(NULL) ) && ( m_internalObject ) )
      {
        delete m_ddo;
        m_ddo = NULL;
	m_internalObject =false;

        if ( ( m_block != NULL ) && ( m_internalBlock ) )
          {
            delete m_block;
            m_block = NULL;
	    m_internalBlock = false;

            if ( ( m_buff != NULL) && ( m_internalBuff ) )
              {
                delete [] m_buff;
		m_buff = NULL;
		m_internalBuff = false;
              }
          }
      }

    if ((m_tagBlock != NULL) && m_internalTag)
      {
        delete [] m_tagBlock;
        m_tagBlock = NULL;
	m_internalTag = false;
      }

    if (m_internalEoc && m_eocBlocks.size())
      {
        for (unsigned int i = 0; i < m_eocBlocks.size(); i++)
          {
            if (m_eocBlocks[i])
              {
                delete [] m_eocBlocks[i];
              }
          }
      }//endif

    int numConns = m_connections.size();
    if ((numConns > 0) && (m_endpointType == SOURCE))
      {
        for (int i=0; i<numConns; i++)
	      delete m_connections[i];

        m_connections.clear();
      }

    return;
}//end Endpoint destructor


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setup( const DataMap& map,
                         int   depth,
                         const unsigned int lengths[],
                         EndpointType type)
{
    PvtolProgram  prog;
    m_procId     = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    unsigned numReplicas = ct.numReplicas();
    m_task       = &(prog.getCurrentTask());
    m_taskid     = ct.getTaskID();
    m_threadid   = ct.getGlobalThreadRank();
#ifdef _DEBUG_2
    char *typstr;
    if (type == SOURCE)
            typstr = "Src";
     else
            typstr = "Dest";

    char ep_prefix[96];
    sprintf(ep_prefix, "EPsetup[%d, %d, %d]%s ",
            m_procId, m_taskid, m_threadid, typstr);
#endif // _DEBUG_2

    m_depth = (depth + numReplicas - 1) / numReplicas;

    m_map = new DataMap(map);

    const RankList& rl = m_map->getRankList();

    if ((m_map->getDataMapType() == DataMap::LOCAL_MAP)
                  ||
        (rl.getNumRanks() == 1))
      {
#ifdef _DEBUG_2
        cout << ep_prefix << endl;
#endif // _DEBUG_2
        if (m_map->isLocal())
          {
	    mapToProcRankList(*m_map, m_procRankList);
	    mapToProcList(*m_map, m_procList);
#ifdef _DEBUG_2
            cout << endl << ep_prefix
	         << "] m_procList is: ";
            for (int i=0; i<m_procList.size(); i++)
	           cout << m_procList[i] << ", ";
	    cout << endl; 
#endif // _DEBUG_2
            int lclSize = 1;
            for (int i = 0; i < DATATYPE::dim; i++ )
              {
                m_lengths[i] = lengths[i];
                lclSize      = lclSize * lengths[i];
              }

            m_slotSize      = lclSize;
            lclSize        *= m_depth;
            m_buff          = new ElType[lclSize];
            m_internalBuff  = true;

            Length<DATATYPE::dim>  len;
            for (int i=0; i<DATATYPE::dim; i++)
                                 len[i] = lengths[i];
            m_block = new Dense<DATATYPE::dim,ElType>(len, m_buff);
            m_internalBlock = true;
            //             --- ### Need to give a Map ### ---
            HierArray<DATATYPE::dim, ElType, Dense<DATATYPE::dim, ElType> >
                     *tmpDdo;
            tmpDdo = new HierArray<DATATYPE::dim, ElType,
                      Dense<DATATYPE::dim, ElType> > (len, *m_block);
            m_ddo = (DATATYPE *)tmpDdo;

            m_internalObject = true;
            m_initialized    = true;
#ifdef PVTOL_DEBUG
            if (type == Endpoint::SOURCE)
                 cout << "EP:setup Src end. " << endl;
              else
                 cout << "EP:setup Dst end. " << endl;

            if (m_block->user_storage() == 1 /* array_format */ )
                 cout << "EP:setup block user storage is array format. "
		      << endl;
              else
                 cout << "EP:setup block user storage is NOT array format. "
	              << m_block->user_storage() << endl;
#endif // PVTOL_DEBUG
          }//endIf data is local

        m_endpointType   = type;
      }//endif all the data is here
     else
      {
	cout << "CdtEP[" << prog.getProcId() << "]:setup "
	     << "Warning: Distributed data not currently supported." << endl;
      }

    return;
}//end setup()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setup(DATATYPE& obj,
                                                 int depth,
                                                 EndpointType type)
{
    m_map   = new DataMap(obj.getMap());

    // m_depth = depth / numReplicas, rounded up
    PvtolProgram prog;
    m_procId = prog.getProcId();
    unsigned numReplicas = (prog.getCurrentTask()).numReplicas();
    m_task     = &(prog.getCurrentTask());
    m_taskid   = m_task->getTaskID();
    m_threadid = m_task->getGlobalThreadRank();
    m_depth = (depth + numReplicas - 1) / numReplicas;

    m_slotSize = 1;
#ifdef NOT_YET
    for(int ii = 0 ; ii < DATATYPE::dim; ii++)
    {
        m_lengths[ii] = obj.size(ii);
        m_slotSize *= 
	  obj.getLayout().getDistributedLayout().getLocalLength(ii,ranks[0]);
    }

    // Make sure the object provides enough space on this node
    if (m_map->isLocal() &&
	obj.getBlock().getLocalSize() < m_slotSize * m_depth)
    {
        throw(Exception("Block is too small", __FILE__, __LINE__));
    }

    m_ddo = &obj;
#endif // NOT_YET
    m_internalObject = false;
    m_initialized    = true;
    m_endpointType   = type;

    return;
}//end setup()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setup(
    EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> & descript,
    EndpointType type)
{
    m_map   = new DataMap(*descript.m_map);
    m_depth = descript.m_depth;
    m_slotSize = 1;
    for (int i=0; i<DATATYPE::dim; i++) {
        m_lengths[i] = descript.m_lengths[i];
        m_slotSize *= m_lengths[i];
    }

    m_procId   = descript.m_procId;
    m_task     = descript.m_task;
    m_taskid   = descript.m_taskid;
    m_threadid = descript.m_threadid;

    for (int i=0; i<descript.m_numInProcList; i++) {
       m_procList.push_back(descript.m_procList[i]);
    }

    for (int i=0; i<descript.m_numInSucRanks; i++) {
       m_sucRanks.push_back(descript.m_sucRanks[i]);
    }

    if (descript.m_isSrc)
              m_endpointType = SOURCE;

    if (descript.m_isDst)
              m_endpointType = DEST;

    if (descript.m_isLocalXfer)
              m_localXfer = true;
     else
              m_localXfer = false;

    m_initialized  = true;
    m_endpointType = type;

    // Capture information about the Endpoint's slot size.  We need this 
    // information even if the Endpoint is non local, because when we use a 
    // one-sided communication library, the source must pass the destination 
    // offset as an argument to Route::send().
#ifdef NOT_YET
    DistributedLayout dlo(m_map, m_lengths);
    m_slotSize = 1;
    for (int i = 0; i < DATATYPE::dim; i++)
    {
	m_slotSize *= dlo.getLocalLength(i, ranks[0]);
    }
#endif // NOT_YET

    return;
}//end setup()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getDescript(
        EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> & descript)
{
    descript.m_map   = m_map;
    descript.m_depth = m_depth;

    for (signed i = 0; i<DATATYPE::dim; i++)
    {
        descript.m_lengths[i] = m_lengths[i];
    }

    descript.m_procId      = m_procId;
    descript.m_task        = m_task;
    descript.m_taskid      = m_taskid;
    descript.m_threadid    = m_threadid;
    descript.m_isLocalXfer = m_localXfer;
    if (m_endpointType == SOURCE)
                descript.m_isSrc = 1;
      else
                descript.m_isSrc = 0;

    if (m_endpointType == DEST)
                descript.m_isDst = 1;
      else
                descript.m_isDst = 0;

    descript.m_numInProcList = m_procList.size();
    for (int i=0; i<descript.m_numInProcList; i++) {
       descript.m_procList[i] = m_procList[i];
    }

    descript.m_numInSucRanks = m_sucRanks.size();
    for (int i=0; i<descript.m_numInSucRanks; i++) {
       descript.m_sucRanks[i] = m_sucRanks[i];
    }

    return;
}//end getDescript()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::rebuildDdo(int size,
                                                      int numOppositeCommGroup)
{
  if ((m_ddo != NULL) && m_internalObject)
    {
       m_block->release();
       delete m_ddo;
       m_ddo = NULL;
       m_internalObject = false;

       if (m_internalBlock)
         {
           delete m_block;
           m_block = NULL;
           m_internalBlock = false;

           if (m_internalBuff)
             {
               delete [] m_buff;
	       m_buff = NULL;
               m_internalBuff = false;
	     }
	 }
    }

  m_buff = new ElType[size];
  m_internalBuff = true;

  Length<DATATYPE::dim> len;
  for (int i=0; i<DATATYPE::dim; i++)
                             len[i] = m_lengths[i];
  m_block = new Dense<DATATYPE::dim,ElType>(len, m_buff);
  m_internalBlock = true;

  HierArray<DATATYPE::dim, ElType, Dense<DATATYPE::dim, ElType> >
     *tmpDdo;
  tmpDdo = new HierArray<DATATYPE::dim, ElType,
                    Dense<DATATYPE::dim, ElType> > (len, *m_block);
  m_ddo = (DATATYPE *)tmpDdo;
  m_internalObject = true;

    if (USE_EOC)
    {
      if (m_eocBlocks.size() > 0)
        {
          for (unsigned int i=0; i < m_eocBlocks.size(); i++)
                    delete [] m_eocBlocks[i];

          vector<SeqNum * >::iterator  vbeg=m_eocBlocks.begin();
          vector<SeqNum * >::iterator  vend=m_eocBlocks.end();
          m_eocBlocks.erase(vbeg, vend);

          m_internalEoc = false;
        }

      // Build multiple blocks, one for each source we are sending
      // to or dest we are receiving from.
      for (signed i = 0; i < numOppositeCommGroup; i++)
        {
	  SeqNum* newBlock = new SeqNum[m_depth];
          m_eocBlocks.push_back(newBlock);

	  // EMR - initialize seq # block entries to -1 to prevent 
	  // an EOC from being detected in error early on.
	  for (int i = 0; i < m_depth; i++)
	    {
	        newBlock[i] = -1;
	    }
        }

      if (m_eocBlocks.size() > 0)
                           m_internalEoc = true;
    }//endIf USE_EOC

  return;
}//end rebuildDdo()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void
    Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setEocBlocks(vector<SeqNum * >  &ebs)
{
    if ((m_eocBlocks.size() > 0) && m_internalEoc)
      {
        for (unsigned int i=0; i<m_eocBlocks.size(); i++)
                   delete [] m_eocBlocks[i];

        vector<SeqNum * >::iterator  vbeg=m_eocBlocks.begin();
        vector<SeqNum * >::iterator  vend=m_eocBlocks.end();
        m_eocBlocks.erase(vbeg, vend);
      }

    m_internalEoc = false;

    for (unsigned int i=0; i<ebs.size(); i++)
                   m_eocBlocks.push_back(ebs[i]);

    return;
}//end setEocBlocks()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
// ElType * Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getDataBuff()
void * Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getDataBuff()
{
    return (void *)m_buff;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::mapToProcRankList(const Map& map,
                                                         vector<int> & prl)
{
    PvtolProgram  prog;
    TaskBase &ct    = prog.getCurrentTask();
     CommScope &cs  = ct.getCommScope();
    unsigned int i, j;

    int numranks = map.getNumRanks();
    if (numranks > 0)
      {
        const RankList & rl = map.getRankList();
	for (i=0; i<numranks; i++) {
           int proc  = cs.rankToProcId(rl.getRank(i));
           int prank = cs.rank(proc);
	   for (j=0; j<prl.size(); j++) {
	      if (prank == prl[j])
	            break;
	   }//endFor j

	   if (j >= prl.size())
	        prl.push_back(prank);
	}//endFor
      }//endIf map was not empty

    return;
}//end mapToProcRankList()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::mapToProcList(const Map& map,
                                                         vector<int> & pl)
{
    PvtolProgram  prog;
    TaskBase &ct = prog.getCurrentTask();
    TaskId tid   = ct.getTaskID();
    TaskId ptid  = -1;
    TaskBase *parTask = &ct;
    vector<int> oldlist;
    vector<int> newlist;
    int         i, j;

    int numranks = map.getNumRanks();
    if (numranks > 0)
      {
        const RankList & rl = map.getRankList();
	for (i=0; i<numranks; i++) {
	   oldlist.push_back(rl.getRank(i));
	}//endFor

	while (tid != ptid) {
	   vector<int> tsklist;
	   parTask->getRankList(tsklist);

           int num = oldlist.size();
	   for (i=0; i<num; i++) {
	     int rank = oldlist[i];
	     for (j=0; j<i; j++) {
	       if (rank == oldlist[j])
	               break;
	     }//endFor j

	     if (i == j)
	          newlist.push_back(tsklist[rank]);
	   }//endFor i

           parTask = &(parTask->getParentTask());
	   tid = ptid;
	   ptid = parTask->getTaskID();
	   oldlist = newlist;
	   newlist.clear();
	}//endWhile

	// since tid == ptid that implies RootTask which means the 
	//     Ranks are the proc IDs
	pl = oldlist;
      }//endIf map was not empty

    return;
}//end mapToProcList()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::calcSetupCompleteRanks()
{
    PvtolProgram  prog;
    TaskBase &ct      = prog.getCurrentTask();
    TaskId currTid    = ct.getTaskID();
    TaskId tid        = m_taskid;
    TaskBase *parTask = &(m_task->getParentTask());
    TaskBase *tsk     = m_task;
    vector<int> oldlist;
    vector<int> newlist;
    int         i;

    int numranks = m_map->getNumRanks();
    if (numranks > 0)
      {
        const RankList & rl = m_map->getRankList();
	for (i=0; i<numranks; i++) {
	   oldlist.push_back(rl.getRank(i));
	}//endFor

	while (currTid != tid) {
	   vector<int> tsklist;
	   tsk->getRankList(tsklist);

           int num = oldlist.size();
	   for (i=0; i<num; i++) {
	     newlist.push_back(tsklist[oldlist[i]]);
	   }//endFor i

	   tid = parTask->getTaskID();
           parTask = &(parTask->getParentTask());
	   oldlist = newlist;
	   newlist.clear();
	}//endWhile

	m_sucRanks = oldlist;
      }//endIf map was not empty

    return;
}//end calcSetupCompleteRanks()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::rebindDdo(typename DATATYPE::ElType *newBuff)
{
  if (m_ddo != NULL)
    {
       m_block->release();
       delete m_ddo;
       m_ddo = NULL;
       m_internalObject =false;

       if (m_internalBlock)
         {
           delete m_block;
           m_block = NULL;
           m_internalBlock = false;

           if (m_internalBuff)
             {
               delete [] m_buff;
	       m_buff = NULL;
	       m_internalBuff = false;
             }
         }
    }

  m_buff = newBuff;

  Length<DATATYPE::dim> len;
  for (int i=0; i<DATATYPE::dim; i++)
                             len[i] = m_lengths[i];
  m_block = new Dense<DATATYPE::dim,ElType>(len, newBuff);
  m_internalBlock = true;
  HierArray<DATATYPE::dim, ElType, Dense<DATATYPE::dim, ElType> >
     *tmpDdo;
  tmpDdo = new HierArray<DATATYPE::dim, ElType,
                 Dense<DATATYPE::dim, ElType> > (len, *m_block);
  m_ddo = (DATATYPE *)tmpDdo;

  m_internalObject = true;

  return;
}//end rebindDdo()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::rebuildTagBuff(int depth)
{
  if ((m_tagBlock != NULL) && m_internalTag)
    {
       delete [] m_tagBlock;
       m_tagBlock = NULL;
       m_internalTag = false;
    }


  m_tagBlock     = new TagSeqNumWrapper<TAGTYPE, USE_EOC>[depth];
  m_internalTag  = true;
  m_tagHandlePtr = &(m_tagBlock[0].m_tagVal);
#ifdef PVTOL_DEBUG
  cout << "EP: rebuildTagBuff @end set m_tagHandlePtr = "
         << m_tagHandlePtr << " tpAdr=" << &m_tagHandlePtr << endl;
#endif // PVTOL_DEBUG

  return;
}//end rebuildTagBuff()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setSlotSize( int size )
{
    m_slotSize = size;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::finalSetup(const string & name,
                                             unsigned numInCommGroup,
                                             unsigned numOppositeCommGroup,
                                             unsigned rankInCommGroup,
                                             bool useTag,
                                             unsigned maxSrcNumProcs)
{
    // PvtolProgram prog;
    // int procId = prog.getProcId();

    m_numInCommGroup  = numInCommGroup;
    m_rankInCommGroup = rankInCommGroup;
    m_roundRobinIndex.setBufSize(numOppositeCommGroup);
    m_roundRobinIndex = m_rankInCommGroup % numOppositeCommGroup;
    m_extractRoundRobinIndex.setBufSize(numOppositeCommGroup);
    m_extractRoundRobinIndex = m_rankInCommGroup % numOppositeCommGroup;
    
        m_nextSequenceNumber = rankInCommGroup;

        for (int i = 0; i < m_depth; i++)
        {
            m_bufferOffsets.push_back(i * m_slotSize);
        }

        m_bufferHeadIndex.setBufSize(m_depth);
        m_bufferHeadIndex = 0;
        m_bufferTailIndex.setBufSize(m_depth);
        m_bufferTailIndex = 0    ;

        m_eocBufferHeadIndex.setBufSize(m_depth);
        m_eocBufferHeadIndex = 0;
        m_eocBufferTailIndex.setBufSize(m_depth);
        m_eocBufferTailIndex = 0;

        m_bufferMgmt.clear();
        m_bufferMgmt.resize(m_depth, 0);

        m_eocBufferMgmt.clear();
        m_eocBufferMgmt.resize(m_depth);
        for (unsigned i = 0; i < m_eocBufferMgmt.size(); i++)
        {
          m_eocBufferMgmt[i].m_mgrIndex = -1;
          m_eocBufferMgmt[i].m_managers.resize(numOppositeCommGroup, 0);
        }
    
        if (USE_EOC || useTag)
        {
    	    if (m_tagBlock == NULL)
	      {
                m_tagBlock = new TagSeqNumWrapper<TAGTYPE, USE_EOC>[m_depth];
	        m_internalTag = true;
	      }

	    m_tagHandlePtr = &(m_tagBlock[0].m_tagVal);
#ifdef PVTOL_DEBUG
            cout << "EP: finalSetup @end set m_tagHandlePtr = "
                 << m_tagHandlePtr << " tpAdr=" << &m_tagHandlePtr << endl;
#endif // PVTOL_DEBUG
	    // EMR - initialize tag block seq #s to -1
	    for (int i = 0; i < m_depth; i++)
	      {
	        m_tagBlock[i].setSeqNum(-1);
	      }
        }

        if (USE_EOC)
          {
	    if (!m_localXfer)
	      {
                // Build multiple blocks, one for each source we are sending
                // to or dest we are receiving from.
                for (unsigned i = 0; i < numOppositeCommGroup; i++)
                 {
	            SeqNum* newBlock = new SeqNum[m_depth];
                    m_eocBlocks.push_back(newBlock);

	            // EMR - initialize seq # block entries to -1 to prevent 
	            // an EOC from being detected in error early on.
	            for (int j = 0; j < m_depth; j++)
	             {
	                newBlock[j+(m_depth*i)] = -1;
	             }
                 }

                if (m_eocBlocks.size() > 0)
                           m_internalEoc = true;
	      }//endif not local xfer
          }//endIf USE_EOC

    return;
}//end finalSetup()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::insertAvailable()
{
    // if this endpoint isn't local, than we are always "ready" to insert
    if (!m_ddo || !m_ddo->isLocal())
      {
        return true;
      }

    // check for a free buffer slot
    SendManager<DataTagSenderX> * pSendManager;
    pSendManager = m_bufferMgmt[m_bufferHeadIndex];

    if (pSendManager == 0)
      {
        return true;
      }
    else
      {
#ifdef PVTOL_DEVELOP
        if (m_bufferHeadIndex != m_bufferTailIndex)
          {
            // if these aren't equal when the buffer is full, then something
            // in the buffer management has been messed up
	    cout << "EP: about to throw: head = " << m_bufferHeadIndex
	         << ", tail = " << m_bufferTailIndex << endl;
	    sleep(2);
            throw Exception("Endpoint: internal error: buffer full and "
                            " buffer head index != buffer tail index",
                            __FILE__, __LINE__);
          }
#endif // PVTOL_DEVELOP

        // try to complete the SendManager object to
        // free the slot
        if (pSendManager->testComplete())
          {
            pSendManager->release();
            m_bufferMgmt[m_bufferHeadIndex] = 0;
            // Since the tail indicates the oldest buffer slot still in use,
            // advance that
            m_bufferTailIndex++;
            return true;
          }
        else
          {
            return false;
          }
      }//endIf pSendManager != 0

}//end insertAvailable()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::waitForInsertBuff()
{
   if (noBuffAvailable())
     {
       // the tail indicates the oldest buffer slot still in use,
       m_bufferMgmt[m_bufferTailIndex]->waitCommComplete();
       m_bufferMgmt[m_bufferTailIndex]->setState(
                                      SendManager<DataTagSenderX>::COMPLETE);
       m_bufferMgmt[m_bufferTailIndex]->release();
       m_bufferMgmt[m_bufferTailIndex] = 0;
       m_bufferTailIndex++;
     }//endIf

   return;
}//end waitForInsertBuff()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::waitForExtractBuff()
{
   if (noBuffAvailable())
     {
       // the tail indicates the oldest buffer slot still in use,
       m_bufferMgmt[m_bufferTailIndex]->waitCommComplete();
       m_bufferMgmt[m_bufferTailIndex]->setState(
                                     SendManager<DataTagSenderX>::COMPLETE);
     }//endIf

   return;
}//end waitForExtractBuff()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
TAGTYPE & Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getExtractTagHandle()
{
    m_bufferMgmt[m_bufferTailIndex]->waitForTag();
    m_bufferMgmt[m_bufferTailIndex]->setState(
                                     SendManager<DataTagSenderX>::COMPLETE);

    return *m_tagHandlePtr;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
TAGTYPE & Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getTagHandleRef()
{
    return *m_tagHandlePtr;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
TAGTYPE & Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getInsertTagHandle()
{
    return *m_tagHandlePtr;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::insert()
{
    // if we are not local then there is nothing to do
    if (!m_ddo || !m_ddo->isLocal())
      {
        return;
      }
    
#ifdef PVTOL_DEVELOP
    // Make sure this buffer slot is actually ready to be sent
    if (m_bufferMgmt[m_bufferHeadIndex] != 0)
      {
        throw Exception("Conduit: insert when insert not available",
                        __FILE__, __LINE__);
      }
#endif
#ifdef _DEBUG_2
    {
    PvtolProgram  prog;
    int procId   = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid   = ct.getTaskID();
    int threadid = ct.getGlobalThreadRank();

    char prefix[96];
    sprintf(prefix, "EPinsert[%d, %d, %d] ",
            procId, taskid, threadid);
    //  cout << prefix << *this << endl;
    }
#endif // _DEBUG_2


    // grab the appropriate outgoing connection
    Connection<DATATYPE, TAGTYPE, USE_EOC> & connection =
                    *m_connections[m_roundRobinIndex];

    // put the sequence number in the tag buffer, if applicable
    if (m_tagBlock)
      {
        m_tagBlock[m_bufferHeadIndex].setSeqNum(getSequenceNumber());
      }

    incrementSequenceNumber();

    if(!m_localXfer)
      {
        // Put the SendManager item in the buffer and kick off the send
        m_bufferMgmt[m_bufferHeadIndex] = connection.getManagerForSrc();
	m_bufferMgmt[m_bufferHeadIndex]->send();

        // Advance pointers for next send
        m_bufferHeadIndex++;
      }
    else //doing local xFer
      {
        // Advance pointers for next send
        m_bufferHeadIndex++;

	//maa  We also want to move the dst Head index (Since they're the same)
	connection.getDstEndpoint().m_bufferHeadIndex++;	
      }

    m_block->release();
    ElType  *newBuff = &(m_buff[m_bufferOffsets[m_bufferHeadIndex]]);
    m_block->rebind(newBuff);
#ifdef PVTOL_DEBUG
    cout << "EP: insert head = " << m_bufferHeadIndex
         << "rebind Addr= " << newBuff << endl;
#endif // PVTOL_DEBUG

    if (m_tagBlock)
    {
      m_tagHandlePtr = &(m_tagBlock[m_bufferHeadIndex].m_tagVal);
#ifdef _DEBUG_2
      cout << prefix
           << " insert @end set m_tagHandlePtr = " << m_tagHandlePtr
           << " headidx=" << m_bufferHeadIndex << endl;
#endif // _DEBUG_2
    }
 
    m_roundRobinIndex += m_numInCommGroup;
    
    return;
}//end insert()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::insertEOCAvailable()
{
    // if not local, nothing to do and we are always available
    if (!m_ddo || !m_ddo->isLocal())
      {
        return true;
      }

    // check for a free EOC buffer slot
    if (m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex == -1)
      { // the next buffer slot is free
        return true;
      }

    // The EOC buffer is full.  Try to free up the next slot.
#ifdef PVL_DEVELOP
    if (m_eocBufferHeadIndex != m_eocBufferTailIndex)
    {
        // if these aren't equal when the buffer is full, then something
        // in the buffer management has been messed up
	    cout << "EP: about to throw EOC head = " << m_eocBufferHeadIndex
	         << ", tail = " m_eocBufferTailIndex << endl;
	    sleep(2);
        throw Exception("Endpoint: internal error: buffer full and "
                        " buffer head index != buffer tail index",
                        __FILE__, __LINE__);
    }
#endif // PVL_DEVELOP
        
    // try to free the slot
    unsigned i;
    for (i = m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex;
         i < m_connections.size();
         i++) {
        SendManager<EocSenderX> * pSendManager;
        pSendManager = m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[i];

        if (pSendManager)
          {
            // a send is outstanding for this Connection.  Try to complete it
            if (pSendManager->testComplete())
              {
                // it is complete
                pSendManager->release();
                m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[i] = 0;
              }
            else
              {
                // we're stuck on a transfer that hasn't completed yet
                break;
              }
          }
    }//endFor i

    // see if we got through all of the transfers -- if we did, we freed
    // the buffer slot
    if (i == m_connections.size())
      {
        // free the slot
        m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex = -1;
        // Since the tail indicates the oldest buffer slot still in use,
        // advance that
        m_eocBufferTailIndex++;
        return true;
      }
    else
      {
        // we didn't free the slot, but we may have completed some of the
        // transfers.  Mark this so we don't have to check them again.
        m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex = i;
        return false;
      }

}//end insertEOCAvailable()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::insertEOC()
{
    // if not local, nothing to do
    if (!m_ddo || !m_ddo->isLocal())
      {
        return;
      }

    if (!insertEOCAvailable())
      {
        throw Exception("Conduit: insertEOC when insertEOC not available",
                        __FILE__, __LINE__);
      }

    if (m_localXfer)
      {
         // put the sequence number of the EOC in the approriate block
         // to prepare it for sending
#ifdef PVL_DEBUG
	 int i = m_eocBlocks.size();
	 cout << "EP: insEoc m_eocBlocks.size=" << i;
	 if (i)
	     cout << ", addr of 1st blk = " << m_eocBlocks[0] << endl;
	   else
	     cout << ", size is zero." << endl;
#endif // PVL_DEBUG

         (m_eocBlocks[m_roundRobinIndex])[m_eocBufferHeadIndex] =
						  getSequenceNumber();
         incrementSequenceNumber();
      }
     else
      {
#ifdef PVL_DEVELOP
        // Make sure this EOC buffer slot is ready to be sent
       if (m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex != -1)
          {
            throw Exception("Conduit: insertEOC: buffer slot not ready",
                            __FILE__, __LINE__);
          }
#endif // PVL_DEVELOP

        // mark this buffer slot not free
        m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex = 0;

        // start a send to each of the destinations in our comm group
        RingIndex eocCnxnIndex = m_roundRobinIndex;
        for (unsigned i = 0; i < m_connections.size(); i++)
        {
            // grab the appropriate outgoing connection
            Connection<DATATYPE, TAGTYPE, USE_EOC> & connection =
                        *m_connections[eocCnxnIndex];

            // put the sequence number of the EOC in the approriate block
            // to prepare it for sending
            (m_eocBlocks[eocCnxnIndex])[m_eocBufferHeadIndex] =
                    getSequenceNumber();
            incrementSequenceNumber();
        
            // get a SendManager for this destination and kick off the send
            m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[eocCnxnIndex] =
                        connection.getEocManagerForSrc();
            m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[eocCnxnIndex]->send();

            eocCnxnIndex += m_numInCommGroup;
        }
      }//endIf all xfers local

    m_eocBufferHeadIndex++;
    
    return;
}//end insertEOC()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::extractReady()
{
    // if not local, we are always ready
    if (!m_ddo || !m_ddo->isLocal())
      {
        return true;
      }

    SendManager<DataTagSenderX> * pSendManager;
    pSendManager = m_bufferMgmt[m_bufferTailIndex];

    if (!m_localXfer && !pSendManager)
      {
        // No receive in progress -- try to start one
        if (!tryStartReceive())
          {
            // we couldn't
            return false;
          }

        fillReceiveQueue();

        // we kicked off a new receive.  If it happens to complete immediately,
        // we can say we're ready now
        pSendManager = m_bufferMgmt[m_bufferTailIndex];
      }

    if (!m_localXfer)
      {
#ifdef PVL_DEVELOP
       // There *should* be a send going on right now since we just started
       // one if there wasn't one before
       if (!pSendManager)
         {
           throw Exception("Endpoint: internal error: pSendManager == 0 in "
                           "extractReady",
                           __FILE__, __LINE__);
         }
#endif // PVL_DEVELOP

       if (!pSendManager->testComplete())
         { // The receive hasn't completed
          return false;
         }
      }//endIf not local xfer

    if (USE_EOC)
      {
        if (m_tagBlock[m_bufferTailIndex].getSeqNum() != getSequenceNumber())
          {
	    // an EOC has to arrive first
	    return false;
	  }
      }

    return true;
}//end extractReady()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::release()
{
    // if not local, nothing to do
    if (!m_ddo || !m_ddo->isLocal())
      {
        return;
      }

    SendManager<DataTagSenderX> * pSendManager;
    pSendManager = m_bufferMgmt[m_bufferTailIndex];

#ifdef PVTOL_DEVELOP
    PvtolProgram  prog;
    TaskBase &ct   = prog.getCurrentTask();
    int procId     = prog.getProcId();
    int taskid     = ct.getTaskID();
    int threadid   = ct.getGlobalThreadRank();
    if ((!m_localXfer)    &&    (!pSendManager ||
            pSendManager->getState() != SendManager<DataTagSenderX>::COMPLETE))
    {
	char  str[128];
	sprintf(str, "Endpoint[%d, %d, %d]: release when receive not complete",
	            procId, taskid, threadid);
        throw Exception(str, __FILE__, __LINE__);
    }
#endif // PVL_DEVELOP

    incrementSequenceNumber();
    if (!m_localXfer)
              pSendManager->release();

    m_bufferMgmt[m_bufferTailIndex] = 0;
    m_bufferTailIndex++;

    m_block->release();
    ElType  *newBuff = &(m_buff[m_bufferOffsets[m_bufferTailIndex]]);
    m_block->rebind(newBuff);

    m_extractRoundRobinIndex += m_numInCommGroup;
    
    if (m_tagBlock)
      {
      m_tagHandlePtr = &(m_tagBlock[m_bufferTailIndex].m_tagVal);
#ifdef PVTOL_DEBUG
      cout << "EP: release @end set m_tagHandlePtr = " << m_tagHandlePtr
           << " Tailidx=" << m_bufferTailIndex << endl;
#endif // PVTOL_DEBUG
      }

    fillReceiveQueue();

    return;
}//end release()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::extractAtLeast(
    int count, bool setupCompleteDone)
{
#ifdef PVTOL_DEVELOP
    if (count != 0 && count != PREPOST_FOREVER)
    {
        // we can either pre-post forever or turn off pre-posting.  Other
        // numbers don't work nicely with replicated tasks, so we
        // are disallowing them for now.
        throw Exception("Conduit: extractAtLeast: prepost count must be "
                        "0 or PREPOST_FOREVER", __FILE__, __LINE__);
    }

    if (m_beginPrepostDone)
    {
        if (m_prepostsRequested == PREPOST_FOREVER && count != PREPOST_FOREVER)
        {
            // you can't "turn off" prepost forever, because it's not
            // known (to the application) which receives have been posted yet
            throw Exception("Conduit: extractAtLeast: Can't undo "
                            "prepost PREPOST_FOREVER", __FILE__, __LINE__);
        }
    }
#endif // PVTOL_DEVELOP
            
    m_prepostsRequested = count;

    // If setupComplete is done, we can start the preposts now.  Otherwise,
    // important objects aren't ready and the preposts have to be done
    // at setupComplete time.
    if (setupCompleteDone)
    {
        // actually do the preposts
        fillReceiveQueue();
    }
}//end extractAtLeast()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::beginPrepost()
{
    PvtolProgram  prog;
    int currproc = prog.getProcId();
    unsigned int i;
    for (i=0; i<m_procList.size(); i++) {
        if (currproc ==  m_procList[i])
                  break;
    }//endFor

    if (i < m_procList.size())
      {
        fillReceiveQueue();

        if (USE_EOC)
          {
            bool success = tryStartEocReceive();
#ifdef PVTOL_DEVELOP
           // This should never fail, there must always be the ability to start
           // one EOC receive, if we are using EOCs at all
            if (!success)
              {
                throw Exception("Conduit: couldn't start EOC receive",
                                __FILE__, __LINE__);
              }
#endif // PVTOL_DEVELOP
          }
      }//enfIf currennt proc is dest

    m_beginPrepostDone = true;

   return;
}//end beginPrepost()

template<class DATATYPE, class TAGTYPE, bool USE_EOC >
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::fillReceiveQueue()
{
    if ((!m_beginPrepostDone)     ||
        (m_map->isLocal() && m_ddo && m_ddo->isLocal()))
    {
      if(!getLocalXfer())
      	{
	  while (m_prepostsRequested && tryStartReceive());
	}
      else
	{
	  while (tryStartReceive());
	}
    }
}//end fillReceiveQueue()

template<class DATATYPE, class TAGTYPE, bool USE_EOC >
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::isAtEOC()
{
    // if not local, the EOC has always arrived
    if (!m_ddo || !m_ddo->isLocal())
    {
        return true;
    }

    if (!m_localXfer)
      {
#ifdef PVTOL_DEVELOP
        // Check if there is an EOC receive outstanding in the next buffer slot.
        // There should *always* be an EOC receive outstanding.
        if (m_eocBufferMgmt[m_eocBufferTailIndex].m_mgrIndex == -1)
        {
          // The slot is totally free.  This should not happen.
          throw Exception("Endpoint: isAtEOC: Internal error - no EOC pending",
                        __FILE__, __LINE__);
        }
#endif // PVTOL_DEVELOP
    
        // Try to complete outstanding transfers, if any,
        // belonging to this buffer slot
        unsigned int i;
        for (i = m_eocBufferMgmt[m_eocBufferTailIndex].m_mgrIndex;
             i < m_connections.size();
             i++)
          {
            SendManager<EocSenderX> * pSendManager;
            pSendManager = m_eocBufferMgmt[m_eocBufferTailIndex].m_managers[i];
            if (pSendManager)
              {
                if (pSendManager->testComplete())
                  {
                    pSendManager->release();
                    m_eocBufferMgmt[m_eocBufferTailIndex].m_managers[i] = 0;
                  }
                else
                  {
                    // can't complete this one
                    break;
                  }
              }
          }//end for each connection

        if (i < m_connections.size())
          {
            // we got stuck at one of the Transfers, which means we're still
            // waiting for an EOC to arrive.
            return false;
          }
      }//endIf all xfers local

    // An EOC indicator has arrived.  Now we look at the sequence number 
    // from each source to see if we actually have an EOC
    RingIndex eocCnxnIndex = m_extractRoundRobinIndex;
    int numLoops = m_connections.size();
    if (numLoops == 0)
               numLoops = 1;

    for (int i = 0; i < numLoops; i++)
      {
        if ((m_eocBlocks[eocCnxnIndex])[m_eocBufferTailIndex] >
            getSequenceNumber() + i * getSequenceNumberIncrement())
          {
              // More to extract before the EOC
	      return false;
	  }

	if ((m_eocBlocks[eocCnxnIndex])[m_eocBufferTailIndex] <
                getSequenceNumber() + i * getSequenceNumberIncrement())
	  {
	    return false;
	  }
	eocCnxnIndex += m_numInCommGroup;
      }//endFor i

    return true;
}//end isAtEOC()

template<class DATATYPE, class TAGTYPE, bool USE_EOC >
void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::clearEOC()
{
    // if we are not local then there is nothing to do
    if (!m_ddo || !m_ddo->isLocal())
    {
        return;
    }

#ifdef PVL_DEVELOP
    if (!isAtEOC())
    {
        // this call is invalid unless we're at an EOC
        throw Exception("Conduit: clearEOC when EOC not pending",
                        __FILE__, __LINE__);
    }
#endif // PVL_DEVELOP

    // increment the sequence number for each connection sending to use
    for (unsigned i = 0; i < m_connections.size(); i++)
    {
        incrementSequenceNumber();
    }
    m_eocBufferMgmt[m_eocBufferTailIndex].m_mgrIndex = -1;

    m_eocBufferTailIndex++;

    bool success = tryStartEocReceive();

#ifdef PVTOL_DEVELOP
    // This should never fail -- there must always be the ability to start
    // one EOC receive, if we are using EOCs at all
    if (!success)
      {
        throw Exception("Conduit: couldn't start EOC receive",
                        __FILE__, __LINE__);
      }
#endif // PVTOL_DEVELOP
    
}//end clearEOC()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::tryStartReceive()
{
    // if no data movement required
    if (m_localXfer)
      {
        return false;
      }

    // if this endpoint isn't local, just return
    if ((m_beginPrepostDone)     &&
        (!m_map->isLocal() || !m_ddo || !m_ddo->isLocal()))
      {
        return false;
      }

    // check for a free buffer slot
    SendManager<DataTagSenderX> * pSendManager;
    pSendManager = m_bufferMgmt[m_bufferHeadIndex];

    if (pSendManager == 0)
    {
        // start a receive here
        Connection<DATATYPE, TAGTYPE, USE_EOC> & connection =
                    *m_connections[m_roundRobinIndex];

        // Put the SendManager item in the buffer and kick off the receive
        m_bufferMgmt[m_bufferHeadIndex] = connection.getManagerForDst();
	m_bufferMgmt[m_bufferHeadIndex]->recv();

        // Advance pointers for next send
        m_bufferHeadIndex++;

        m_roundRobinIndex += m_numInCommGroup;

        return true;
    }
    else
    {
        // We can't complete this buffer slot and then start a new receive,
        // because the current receive must be extracted first
        // Hence, we fail.
        return false;
    }

}//end tryStartReceive()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::tryStartEocReceive()
{
    // if no data movement required
    if (m_localXfer)
      {
        return true;
      }
    // if this endpoint isn't local, we always succeed
    if (!m_map->isLocal() || !m_ddo || !m_ddo->isLocal())
      {
        return true;
      }

    // check for a free buffer slot
    if (m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex == -1)
      {
        // start a new EOC receive here
        
        // mark this buffer slot occupied
        m_eocBufferMgmt[m_eocBufferHeadIndex].m_mgrIndex = 0;

        // start the transfers
        for (unsigned i = 0; i < m_connections.size(); i++)
        {
            Connection<DATATYPE, TAGTYPE, USE_EOC> & connection =
                    *m_connections[i];

            m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[i] =
                    connection.getEocManagerForDst();
            m_eocBufferMgmt[m_eocBufferHeadIndex].m_managers[i]->recv();
        }

        m_eocBufferHeadIndex++;
            
        return true;
      }
    else
    {
        return false;
    }
}//end tryStartEocReceive()


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
ostream& operator<< (ostream& output, const Endpoint<DATATYPE, TAGTYPE, USE_EOC>& ep)
{
  output << "EP[" << ep.m_procId << "," << ep.m_taskid << "," << ep.m_threadid
         << "] init=";
  if (ep.m_initialized)
     output << "true";
   else
     output << "false";

  if (ep.m_endpointType == Endpoint<DATATYPE, TAGTYPE, USE_EOC>::SOURCE)
     output << " Src ";
   else
     output << " Dst ";

  output << "#Conns=" << ep.m_connections.size() << " ";
  // output << endl;

  return(output);
}//end operator<<

}//end Namspace
#endif // PVTOL_ConduitEndpoint_cc

