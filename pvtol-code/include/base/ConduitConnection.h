/**
 *    File: ConduitConnection.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-19 13:45:17 -0500 (Thu, 19 Feb 2009) $
 *  \version $LastChangedRevision: 939 $
 *  \brief   Class definition of ConduitConnection
 *
 *  Author: Glenn Schrader
 *
 *  $Id: ConduitConnection.h 939 2009-02-19 18:45:17Z jav5880 $
 *
 */
#ifndef PVTOL_CONDUITCONNECTION_H
#define PVTOL_CONDUITCONNECTION_H

#include <PvtolBasics.h>
#include <Transfer.h>
#include <Route.h>
#include <Conduit.h>
#include <ConduitEndpoint.h>
#include <ConduitSendManager.h>
#include <ConduitSenders.h>

#include <shared_ptr.hpp>

#include <vector>
#include <utility>
#include <string>
#include <typeinfo>

using boost::shared_ptr;
using std::string;
using std::vector;
using std::pair;

namespace ipvtol
{

/////////////////////////////////////////////////////////////////////////////
// class Connection
/////////////////////////////////////////////////////////////////////////////

// This class provides the point-to-point connection(s) for each mode.
// Each mode will have one Connection object for every source/destination
// pair that needs to communicate.

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
class Connection
{
  public:
    typedef Endpoint<DATATYPE, TAGTYPE, USE_EOC> EndpointX;
    typedef DataTagSender<DATATYPE, TAGTYPE, USE_EOC> DataTagSenderX;
    typedef EocSender<DATATYPE, TAGTYPE, USE_EOC> EocSenderX;

    Connection(EndpointX & src,   // Local Xfer Constructor
	       EndpointX & dst);

    Connection(EndpointX & src,
               EndpointX & dst,
               bool useTag,
               Route::Flags flags = Route::DEFAULT_FLAG);

    ~Connection();

    inline Route & getRoute();
    inline bool srcLocal();
    inline bool dstLocal();
    inline int  getDataSize();

    inline vector< shared_ptr<Transfer> > & getTagTransfers();
    inline vector< pair<int, int> >       & getTagTransferNodeList();

    inline vector< shared_ptr<Transfer> > & getEocTransfers();
    inline vector< pair<int, int> >       & getEocTransferNodeList();

    inline vector< shared_ptr<Transfer> > & getSeqNumTransfers();
    inline vector< pair<int, int> >       & getSeqNumTransferNodeList();

    SendManager<DataTagSenderX> * getManagerForSrc();
    SendManager<DataTagSenderX> * getManagerForDst();

    SendManager<EocSenderX>     * getEocManagerForSrc();
    SendManager<EocSenderX>     * getEocManagerForDst();

    bool        localResponsibilities(EndpointX& ep);

    inline void send(
        SendRequest & srq,
        unsigned srcBufferIndex,
        unsigned dstBufferIndex);

    inline void sendTag(
        int tagTransferIndex,
        SendRequest & srq,
        unsigned srcBufferIndex,
        unsigned dstBufferIndex);

    inline void sendEoc(
        int eocTransferIndex,
        SendRequest & srq,
        unsigned srcBufferIndex,
        unsigned dstBufferIndex);

    inline void sendSeqNum(
        int seqNumTransferIndex,
        SendRequest & src,
        unsigned srcBufferIndex,
        unsigned dstBufferIndex);

    inline const vector<unsigned> & getSeqNumLocalDstIndex() const;

    inline EndpointX & getSrcEndpoint();
    inline EndpointX & getDstEndpoint();

  private:
    EndpointX                      & m_src;
    EndpointX                      & m_dst;
    bool                            m_srcProcLocal;
    bool                            m_dstProcLocal;
    bool                            m_dataStaysInPlace;
    Route                          *m_spRoute;
    int                             m_dataSize;
    vector< pair<int, int> >        m_tagNodePairs;
    vector< shared_ptr<Transfer> >  m_tagTransfers;
    vector< shared_ptr<SendManager<DataTagSenderX> > > m_sendManagers;

    RingIndex                       m_mgrsSendIndex;
    RingIndex                       m_mgrsRecvIndex;
    RingIndex                       m_srcBufferIndex;
    RingIndex                       m_dstBufferIndex;

    vector< shared_ptr<SendManager<EocSenderX> > > m_eocSendManagers;
    vector< shared_ptr<Transfer> >  m_eocTransfers;
    RingIndex                       m_eocMgrsSendIndex;
    RingIndex                       m_eocMgrsRecvIndex;
    RingIndex                       m_eocSrcBufferIndex;
    RingIndex                       m_eocDstBufferIndex;
};//end class Connection

//          I N L I N E     Methods
//---------------------------------------------
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline Route & Connection<DATATYPE, TAGTYPE, USE_EOC>::getRoute()
{
    return *m_spRoute;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Connection<DATATYPE, TAGTYPE, USE_EOC>::srcLocal()
{
    return m_srcProcLocal;
}
    
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool Connection<DATATYPE, TAGTYPE, USE_EOC>::dstLocal()
{
    return m_dstProcLocal;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int
Connection<DATATYPE, TAGTYPE, USE_EOC>::getDataSize()
{
    return m_dataSize;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< shared_ptr<Transfer> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getTagTransfers()
{
    return m_tagTransfers;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< pair<int, int> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getTagTransferNodeList()
{
    return m_tagNodePairs;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< shared_ptr<Transfer> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getEocTransfers()
{
    return m_eocTransfers;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< pair<int, int> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getEocTransferNodeList()
{
    // EOCs follow the same node-to-node paths as tags
    return m_tagNodePairs;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< shared_ptr<Transfer> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getSeqNumTransfers()
{
    return this->m_seqNumTransfers;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline vector< pair<int, int> > &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getSeqNumTransferNodeList()
{
    // EOCs follow the same node-to-node paths as tags
    return this->m_seqNumNodePairs;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline Endpoint<DATATYPE, TAGTYPE, USE_EOC>&  //return a reference
Connection<DATATYPE, TAGTYPE, USE_EOC>::getSrcEndpoint()
{
  return m_src;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline Endpoint<DATATYPE, TAGTYPE, USE_EOC>& //return a reference
Connection<DATATYPE, TAGTYPE, USE_EOC>::getDstEndpoint()
{
  return m_dst;
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Connection<DATATYPE, TAGTYPE, USE_EOC>::send(
                                                SendRequest & srq,
                                                unsigned srcBufferIndex,
                                                unsigned dstBufferIndex)
{
    if (m_spRoute != NULL)
         m_spRoute->isend(m_src.m_bufferOffsets[srcBufferIndex],
                          m_dst.m_bufferOffsets[dstBufferIndex],
                          srq);
}
 
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Connection<DATATYPE, TAGTYPE, USE_EOC>::sendTag(
                                                int tagTransferIndex,
                                                SendRequest & srq,
                                                unsigned srcBufferIndex,
                                                unsigned dstBufferIndex)
{
    // Note: since the size of a tag is 1 item, the index into the buffer
    // is the same as the offset into the block, as below
    this->m_tagTransfers[tagTransferIndex]->isend(srcBufferIndex, dstBufferIndex,
                                            srq);
}
  
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Connection<DATATYPE, TAGTYPE, USE_EOC>::sendEoc(
                                                int eocTransferIndex,
                                                SendRequest & srq,
                                                unsigned srcBufferIndex,
                                                unsigned dstBufferIndex)
{
    // Note: since the size of an EOC is 1 item, the index into the buffer
    // is the same as the offset into the block, as below
    this->m_eocTransfers[eocTransferIndex]->isend(srcBufferIndex, dstBufferIndex,
                                            srq);
}
   
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void Connection<DATATYPE, TAGTYPE, USE_EOC>::sendSeqNum(
                                                int seqNumTransferIndex,
                                                SendRequest & srq,
                                                unsigned srcBufferIndex,
                                                unsigned dstBufferIndex)
{
    // Note: since the size of a sequence number is 1 item, the index into
    // the source buffer is the same as the offset into the block, as below
    // The index into the destination block is determined by which source
    // node is sending, because the idea is for each destination to receive
    // the same value from every source.
    this->m_seqNumTransfers[seqNumTransferIndex]->isend(
        srcBufferIndex,
        dstBufferIndex * m_dst.m_seqNumBlockSlotSize +
                         this->m_seqNumDstBufferIndex[seqNumTransferIndex],
        srq);
}
    

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const vector<unsigned> &
Connection<DATATYPE, TAGTYPE, USE_EOC>::getSeqNumLocalDstIndex() const
{
    return this->m_seqNumLocalDstIndex;
}


}//end namespace


#include <ConduitConnection.inl> 

#endif // PVTOL_CONDUITCONNECTION_H


