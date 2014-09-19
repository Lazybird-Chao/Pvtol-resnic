/**
 *    File: ConduitEndpoint.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Class definition of Endpoint & EndpointDescript
 *
 *  $Id: ConduitEndpoint.h 945 2009-02-24 21:28:21Z jav5880 $
 *
 */

#ifndef PVTOL_CONDUITENDPOINT_H
#define PVTOL_CONDUITENDPOINT_H

#include <PvtolBasics.h>
#include <Map.h>
#include <DataMap.h>
#include <RingIndex.h>
#include <HierArray.h>

#include <string>
#include <vector>
#include <typeinfo>
#include <iostream>

namespace ipvtol
{
using std::string;
using std::vector;
using std::ostream;

#define MAX_EP_PROCESSES   128

/////////////////////////////////////////////////////////////////////////////
// class EndpointDescript
/////////////////////////////////////////////////////////////////////////////

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
class EndpointDescript
{
    public:
    EndpointDescript();//constructor

    int getSerializedSize();

    int * serialize(int * buf);

    int * deserialize(int * buf);

    unsigned int getNumReplicas();
    unsigned int getReplicaRank();

    void setNumReplicas(unsigned int numReplicas);

    void setReplicaRank(unsigned int replicaRank);
    inline TaskId getTaskId();
    inline int    getThreadId();
    inline int    getProcId();
    
    friend class Endpoint<DATATYPE, TAGTYPE, USE_EOC>;

    private:
    int          m_depth;
    unsigned int m_lengths[MAX_DIM];
    
    unsigned int m_numReplicas;
    unsigned int m_replicaRank;
    int          m_procId;     // absolute ProcID of this endpoint
    int          m_isSrc;
    int          m_isDst;
    int          m_isLocalXfer;
    TaskBase    *m_task;
    TaskId       m_taskid;
    int          m_threadid;
    int          m_numInProcList;
    int          m_procList[MAX_EP_PROCESSES];
    int          m_numInSucRanks;
    int          m_sucRanks[MAX_EP_PROCESSES];

    DataMap     *m_map;
}; // end EndpointDescript


template <class DATATYPE, class TAGTYPE, bool USE_EOC> class Endpoint;

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
ostream& operator<< (ostream& output, const Endpoint<DATATYPE, TAGTYPE, USE_EOC>& ep);

/////////////////////////////////////////////////////////////////////////////
// class Endpoint
/////////////////////////////////////////////////////////////////////////////

template< class DATATYPE, class TAGTYPE, bool USE_EOC>
class Endpoint {

    public:

    friend class Connection<DATATYPE, TAGTYPE, USE_EOC>;

    typedef typename DATATYPE::ElType ElType;
    typedef DataTagSender<DATATYPE, TAGTYPE, USE_EOC> DataTagSenderX;
    typedef EocSender<DATATYPE, TAGTYPE, USE_EOC> EocSenderX;

    enum EndpointType
    {
        SOURCE,
        DEST
    };

    struct EocManagerList
    {
        SeqNum m_mgrIndex;
        vector< SendManager<EocSenderX> *> m_managers;
    };
    
    const static int PREPOST_FOREVER = Conduit<DATATYPE, TAGTYPE, USE_EOC>::PREPOST_FOREVER;

    Endpoint(const string& dataName);
    Endpoint();
    ~Endpoint();

    void setup(const DataMap& srcMap,
               int depth,
               const unsigned int lengths[],
               EndpointType type);

#ifdef NOT_YET 
    void setup(const DataMap& srcMap,
               const BlockType& block,
               int depth,
               const unsigned int lengths[],
               EndpointType type);
#endif // NOT_YET 

    void setup(DATATYPE& obj,
               int depth,
               EndpointType type);

    void setup(EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> & descript,
               EndpointType type);

    void getDescript(EndpointDescript<DATATYPE, TAGTYPE, USE_EOC> & descript);
    inline int getDepth() const;
    inline void setDepth(int depth);
    inline bool localMaster() const;
    inline bool isInitialized() const;
    inline bool isLocal() const;
    inline unsigned int getSize() const;
    inline int getSlotSize() const;
    void setSlotSize(int size);
    inline const DataMap & getMap();
    inline DATATYPE* getDataAddress();
    // ElType* getDataBuff();
    void  * getDataBuff();
    inline TagSeqNumWrapper<TAGTYPE, USE_EOC> * getTagAddress();
    TAGTYPE & getExtractTagHandle();
    TAGTYPE & getInsertTagHandle();
    void rebuildDdo(int size, int numOppositeCommGroup=0);
    void rebindDdo(ElType *);
    void rebuildTagBuff(int depth);
    inline void rebindTagBuff(TagSeqNumWrapper<TAGTYPE, USE_EOC> *newTag);

    void addConnection(Connection<DATATYPE, TAGTYPE, USE_EOC> * conn);

    // Note: maxSrcNumProcs is the maximum number of procs in all sources
    // that send to this destination endpoint.  It is not used when
    // finalSetup is called on a source endpoint.
    void finalSetup(const string & name,
                    unsigned numInCommGroup,
                    unsigned numOppositeCommGroup,
                    unsigned int rankInCommGroup,
                    bool useTag,
                    unsigned maxSrcNumProcs);

    inline int getSequenceNumber();
    inline int getSequenceNumberIncrement();
    inline void incrementSequenceNumber();
    inline bool isBlockInternal() const;
    inline bool isObjectInternal() const;
    inline ProcId procId() const;
    void calcSetupCompleteRanks();

    inline int getHeadBuffIdx() const;
    inline int getTailBuffIdx() const;

    //get and set LocalXfer Status.
    //maa
    void setLocalXfer(bool x);
    bool getLocalXfer();

    inline vector<SeqNum * >&  getEocBlocks();
    void   setEocBlocks(vector<SeqNum * >  &ebs);

    // src end only methods
    bool insertAvailable();
    void insert();
    bool insertEOCAvailable();
    void insertEOC();
    
    // dst end only methods
    bool extractReady();
    void release();
    void extractAtLeast(int count, bool setupCompleteDone);
    void beginPrepost();
    void fillReceiveQueue();
    bool isAtEOC();
    void clearEOC();
    void waitForInsertBuff();
    void waitForExtractBuff();
    inline bool noBuffAvailable() const;
    TAGTYPE & getTagHandleRef();
    inline const vector<int>& getProcList() const;
    inline const vector<int>& getProcRankList() const;
    inline const vector<int>& getSetupCompleteRanks() const;

    //  template< class DATATYPE, class TAGTYPE, bool USE_EOC >
    friend
    ostream& operator<< <> (ostream& output,
                         const Endpoint& ep);
                         //  const Endpoint<DATATYPE, TAGTYPE, USE_EOC>& ep);

    private:

    bool tryStartReceive();
    bool tryStartEocReceive();
    void mapToProcList(const Map& map, vector<int> & pl);
    void mapToProcRankList(const Map& map, vector<int> & pl);

    // fields that describe the endpoint
    // transmitted in Endpoint exchange
    DataMap       *m_map;
    int            m_depth;
    int            m_lengths[MAX_DIM];
    int            m_procId;
    TaskBase      *m_task;
    TaskId         m_taskid;
    int            m_threadid;
    vector<int>    m_procList;
    vector<int>    m_procRankList;
    vector<int>    m_sucRanks;

    // fields that buffer data at the endpoint
    // not transmitted
    ElType            *m_buff;
    bool               m_internalBuff;
    ipvtol::Dense<DATATYPE::dim, ElType>  *m_block;
    bool               m_internalBlock;
    DATATYPE          *m_ddo;
    bool               m_internalObject;
    bool               m_internalTag;
    bool               m_internalEoc;

    TagSeqNumWrapper<TAGTYPE, USE_EOC>   *m_tagBlock;
    TAGTYPE                              *m_tagHandlePtr;
    vector<unsigned>                      m_bufferOffsets;
    vector<SendManager<DataTagSenderX> * > m_bufferMgmt;
    vector<EocManagerList>                m_eocBufferMgmt;

    // fields not transmitted
    string          m_name;
    bool            m_initialized;
    RingIndex       m_bufferHeadIndex;  
    RingIndex       m_bufferTailIndex;
    RingIndex       m_eocBufferHeadIndex;
    RingIndex       m_eocBufferTailIndex;

    // all the outgoing or incoming connections from/to this Endpoint
    vector< Connection<DATATYPE, TAGTYPE, USE_EOC> * >
                    m_connections;

    RingIndex       m_roundRobinIndex;

    // This round-robin pointer lags m_roundRobinIndex and indicates the
    // next connection from which data will be *extracted*
    // (m_roundRobinIndex indicates the next connection from which a
    // receive will be started).  It is not needed to perform extracts
    // correctly, because the buffer manager already has this information,
    // but it is needed for sequencing EOCs correctly.
    RingIndex         m_extractRoundRobinIndex;

    unsigned          m_numInCommGroup;
    unsigned          m_rankInCommGroup;
    unsigned          m_slotSize;
    int               m_prepostsRequested;
    bool              m_beginPrepostDone;
    EndpointType      m_endpointType;
    vector<SeqNum * > m_eocBlocks;

    // sequence-number handling
    int    m_nextSequenceNumber;

    // doing a local xFer
    bool   m_localXfer;
};

//     I N L I N E      Methods
//-------------------------------------------------
template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline unsigned int EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getNumReplicas()
{
    return m_numReplicas;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline unsigned int EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getReplicaRank()
{
    return m_replicaRank;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline void EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::setNumReplicas(
                                                    unsigned int numReplicas)
{
    m_numReplicas = numReplicas;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline void EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::setReplicaRank(
                                                    unsigned int replicaRank)
{
    m_replicaRank = replicaRank;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline ProcId EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getProcId()
{
    return m_procId;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline TaskId EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getTaskId()
{
    return m_taskid;
}

template< class DATATYPE, class TAGTYPE, bool USE_EOC >
inline int EndpointDescript<DATATYPE, TAGTYPE, USE_EOC>::getThreadId()
{
    return m_threadid;
}



template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setDepth(int depth)
{
  m_depth = depth;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getDepth() const
{
  return m_depth;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline ProcId Endpoint<DATATYPE, TAGTYPE, USE_EOC>::procId() const
{
   return m_procId;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const vector<int>&
 Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getProcRankList() const
{
   return m_procRankList;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const vector<int>&
 Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getProcList() const
{
   return m_procList;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const vector<int>&
 Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getSetupCompleteRanks() const
{
   return m_sucRanks;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getHeadBuffIdx() const
{
  return m_bufferHeadIndex;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getTailBuffIdx() const
{
  return m_bufferTailIndex;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::localMaster( ) const
{
    PvtolProgram  prog;
    TaskBase&     ct     = prog.getCurrentTask();
    int           myrank = ct.getGlobalThreadRank();
    const RankList& rl   = m_map->getRankList();
    return (rl.getRank(0) == myrank);
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::noBuffAvailable() const
{
   if ((m_bufferHeadIndex == m_bufferTailIndex)
                         &&
       (m_bufferMgmt[m_bufferHeadIndex] != 0))
             return(true);
     else
             return(false);
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::isInitialized() const
{
    return m_initialized;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::isLocal() const
{
    return m_map->isLocal();
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline unsigned int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getSize( ) const
{
    // unsigned int size = m_lengths[0];
    // for (unsigned i = 1; i < DATATYPE::dim; i++)
    // {
    //     size *= m_lengths[i];
    // }
    // return size;
    return m_slotSize;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getSlotSize( ) const
{
    return m_slotSize;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const DataMap &
Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getMap()
{
    return *m_map;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline DATATYPE* Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getDataAddress()
{
    return m_ddo;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline TagSeqNumWrapper<TAGTYPE, USE_EOC> *
                Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getTagAddress()
{
    return m_tagBlock;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector<SeqNum * > & Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getEocBlocks()
{
    return m_eocBlocks;
}

#ifdef NO_DEF
template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::addConnection(
        shared_ptr< Connection<DATATYPE, TAGTYPE, USE_EOC> > & spConnection)
{
    m_connections.push_back(spConnection);
}
#endif // NO_DEF
template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::addConnection(
         Connection<DATATYPE, TAGTYPE, USE_EOC>  * conn)
{
    m_connections.push_back(conn);
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getSequenceNumber()
{
    return m_nextSequenceNumber;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getSequenceNumberIncrement()
{
    return m_numInCommGroup;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::incrementSequenceNumber()
{
    m_nextSequenceNumber += getSequenceNumberIncrement();
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::setLocalXfer(bool x)
{
  m_localXfer = x;
}
template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::getLocalXfer()
{
  return m_localXfer;
}

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Endpoint<DATATYPE, TAGTYPE, USE_EOC>::rebindTagBuff(
                                  TagSeqNumWrapper<TAGTYPE, USE_EOC> *newTag)
{
  if ((m_tagBlock != NULL) && (m_internalTag))
               delete m_tagBlock;

  m_tagBlock     = newTag;
  m_internalTag  = false;
  m_tagHandlePtr = &(m_tagBlock[0].m_tagVal);
#ifdef PVTOL_DEBUG
  cout << "EP: rebindTagBuff @end set m_tagHandlePtr = "
       << m_tagHandlePtr << " tpAdr=" << &m_tagHandlePtr << endl;
#endif // PVTOL_DEBUG

  return;
}//end rebindTagBuff()

template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::isBlockInternal() const
{
    return m_internalBlock;
}


template<class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Endpoint<DATATYPE, TAGTYPE, USE_EOC>::isObjectInternal() const
{
    return m_internalObject;
}

}//end namespace


#include <ConduitEndpoint.inl> 

#endif // PVTOL_CONDUITENDPOINT_H


