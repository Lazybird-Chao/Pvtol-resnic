/**
 *    File: Route.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    January 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Definintion of class Route
 *    A Route object provides the means of doing a real time data re-org.
 *    It is capable of redistributing a data object which is distributed
 *    accross one set of nodes to one distributed on another set of nodes.
 *    Both the starting and ending distributions must be described via
 *    PVTOL Maps.
 *    Note: there are several classes defined here that are for Route's
 *          internal use. These are:
 *                PackingInfo, SendInfo, RecvInfo, LocalXferInfo,
 *                SrcXferInfo, DestXferInfo
 *
 *     See also Route.inl for the inline methods.
 *
 *  $Id: Route.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_ROUTE_H
#define PVTOL_ROUTE_H

#include <PvtolBasics.h>
#include <CommScope.h>
#include <PvtolRequest.h>
#include <Pitfalls.h>
#include <HierArray.h>
#include <DataMap.h>
#include <NTuple.h>
#include <Falls.h>

#include <vector>
#include <list>

//#define _DEBUG_2
//#define _DEBUG_3
//#define DEBUG_MATRIX_ARRAY_PARAMS

namespace ipvtol
{
    using std::vector;
    using std::list;

    // Forward declarations
    class SendRequest;

    class PackingInfo {
      public:
        // TBD
    };

    class SendInfo {
      public:
        void         *addr;
        void         *destAddr;
        int           byteSize;
        int           destRank;
        int           destPfIdx;

        bool          sendIsLocal;
        int           destAddrOffset;
        int           serStartIdx;
        bool          packingRequired;
        PackingInfo  *packInfo;
    };

    class RecvInfo {
      public:
        void         *addr;
        void         *srcAddr;
        int           byteSize;
        int           srcRank;
        int           srcPfIdx;
        bool          recvIsLocal;
        int           srcAddrOffset;
        int           serStartIdx;
        bool          unpackingRequired;
        PackingInfo  *packInfo;
    };

    class LocalXferInfo {
      public:
        const void    *srcAddr;
        void          *destAddr;
        int            destAddrOffset;
        int            byteSize;
    };


    class SrcXferInfo {
      public:
        SendInfo      *sendInfo;
        PvtolRequest  *sendReq;
        int            numSends;
        
        SrcXferInfo();
    };

    class DestXferInfo {
      public:
        RecvInfo      *recvInfo;
        PvtolRequest  *recvReq;
        int            numRecvs;
        
        DestXferInfo();
    };

/** Route is an object used to perform data redistribution. Route supports
 *    the ability to move a data object which is distributed, in some
 *    PVTOL supported fashion, on one set of processors to another data
 *    object with its own distribution and processor set.
 *    Route supports blocking, nonblocking, and persistant versions
 *    of communications. The data Route moves must be resident in
 *    buffers which are PVTOL Block objects.
 *
 *   The send method is expected to be used in a SPMD manner throughout
 *    the task(s) using the send and receive buffers (a.k.a. Blocks).
 *    The Route object determines what is to be done on each node.
 *    The same send command covers both the source end (i.e. the sender)
 *    and the destination end (i.e. the receiver). This API has been
 *    intentionally built neutral as to whether the underlying comm
 *    protocol is a two sided or a one sided protocol.
 *
 *   The "i" prefix on the isend method implies a non-blocking call.
 *    That is, the send returns immediately without waiting for the
 *    sending or receiving of data to complete. All isend methods
 *    take a SendRequest ref as a parameter. After the call this
 *    SendRequest object may be queried as to the completion of the
 *    send.
 *
 *   There are various overloaded versions of the send and isend methods.
 *    These version differ on whether the user want to alter the offsets
 *    into the src and dest buffers.
 *   A Route object is templated on the type of data which it will move.
 *      This type Must be identical to that of the Block objects which are
 *      to be used as src and dest buffers.
 *
 *   @author Jim Daly
 *   @see MapDependentType, SendRequest, Transfer
 */
class Route
{
    //----------------------------
  public:
    //----------------------------
    
    enum Flags {
        DEFAULT_FLAG  = 0x00,
        STATIC        = 0x01,  // bit 0 denotes static
        TRANSPOSED    = 0x80,  // bit 5 denotes transposed
        TRANSPOSE_102 = 0x84,  // bits 1 - 4 describe transpose
	TAG_WILL_BE_SUPPLIED = 0x100
    };


/** Construct from 2 DistributedViewBase objects.
 *  @param srcView   DistributedViewBase ref object which is the source of
 *                                    the data redistribution.
 *  @param destView  DistributedViewBase ref object which is the destination of
 *                                    the data redistribution.
 *  @param flags     <optional> Bitflag, mutiple flags may be OR'd, which
 *                   identifies special attributes of this instance. These
 *                   flags are used to facilitate optimal performance
 *                   when the communication is performed.
 *                   The following flags are supported:
 *                       Route::DEFAULT FLAG This implies there is
 *                              nothing special. The user is free to
 *                              use all forms of send and isend later on.
 *                       Route::STATIC  indicates no offsets will
 *                              be used when send is performed.
 *                              Note: this is a promise by the user Not
 *                              to use offsets at send-time.
 * @return void No return value.
 */
#ifdef INCLUDE_MAPS
  template<dimension_type N, typename T, class Block1, class Map1>
                                       //  class Block2, class Map2>
Route(const HierArray<N, T, Block1, Map1& >* srcHA,
      const HierArray<N, T, Block1, Map1& >* destHA,
      Flags flags=DEFAULT_FLAG);
#else // do not INCLUDE_MAPS
  template<dimension_type N, typename T, class Block1>
Route(const HierArray<N, T, Block1 >* srcHA,
      const HierArray<N, T, Block1 >* destHA,
      const DataMap& srcMap,
      const DataMap& destMap,
      bool  srcIsLocal,
      bool  destIsLocal,
      const vector<int>& srcProcList,
      const vector<int>& destProcList,
      Flags flags=DEFAULT_FLAG);
#endif // INCLUDE_MAPS


//           destructor
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 ~Route(void) throw();

/** Blocking send.
 *
 * @return void No return value.
 */
 void send(void);

/** Blocking send, change offsets.
 *
 * @param srcOff    integer offset, in num of block types, into the src
 *                   buffer where the send is to begin.
 * @param destOff   integer offset, in num of block types, into the dest
 *                   buffer where the data is to be sent.
 * @return void No return value.
 */
 void send(int srcOffset, int destOffset);

/** Non-blocking send.
 *
 * @param sendReq   SendRequest ref This object is later used to determine
 *                   when the local part of the send has completed.
 * @return void No return value.
 */
 void isend(SendRequest &request);

/** Non-blocking send, change offsets.
 *
 * @param srcOff    integer offset, in num of block types, into the src
 *                   buffer where the send is to begin.
 * @param destOff   integer offset, in num of block types, into the dest
 *                   buffer where the data is to be sent.
 * @param sendReq   SendRequest ref This object is later used to determine
 *                   when the local part of the send has completed.
 * @return void No return value.
 */
 void isend(int srcOffset, int destOffset, SendRequest &request);

/** provide number of sources
 *   The numSrcs() method returns the number of sources
 *    for the local node.
 *
 * @param  void, there are no input parameters
 * @return int, number of sources
 */
 int numSrcs(void) const;

/** provide number of destinations
 *   The numDests() method returns the number of destinations
 *    for the local node.
 *
 * @param  void, there are no input parameters
 * @return int, number of destinations
 */
 int numDests(void) const;

/** provide list of sources by rank
 *   The srcRanks() method provides a list of ranks which are sources
 *    for the local node.
 *
 * @param  int *, pointer to an array of integers which the method
 *         will fill with the Ranks of the sources. It is the
 *         callers responsibility to be sure the array is big enough.
 * @return int, number of sources
 */
 int srcRanks(int *srcs) const;

/** provide list of sources by rank
 *   The srcRanks() method provides a list of ranks which are sources
 *    for the specified node.
 *
 * @param  ProcId, the ID of the node for which the list of sources will
 *         be generated.
 * @param  int *, pointer to an array of integers which the method
 *         will fill with the Ranks of the sources. It is the
 *         callers responsibility to be sure the array is big enough.
 * @return int, number of sources
 */
 int srcRanks(int destRank, int *srcs) const;

/** provide list of destinations by rank
 *   The destRanks() method provides a list of ranks which are destinations
 *    for the local node.
 *
 * @param  int *, pointer to an array of integers which the method
 *         will fill with the Ranks of the destinations. It is the
 *         callers responsibility to be sure the array is big enough.
 * @return int, number of destinations
 */
 int destRanks(int *dests) const;

 /** Get the internal communications tag used by the Route.
 *  For internal use only!
 *
 *  @return integer communications tag used by the Route 
 */
 int getTag() const;

 /**
 *  Set the internal communications tag used by the Route.
 *  Because this affects the allocation of tags, it must
 *  be done in a SPMD way across the task in which the route
 *  was created.
 *
 *  For internal use only!
 *
 *  @param integer communications tag to be used by the Route 
 */
 void setTag(int tag);

//----------------------------
private:
//----------------------------
 friend class SendRequest;

 void localCopy(int srcOff, int destOff);

 void iStaticSend(SendRequest &req);
 void staticSend();

 void iNonStaticSend(int srcOff, int destOff, SendRequest &req);
 void nonStaticSend(int srcOff, int destOff);

 void simpleSendInfo(void    *srcBlkAddr,
                     void    *destBlkAddr,
		     int      dataSize,
		     ProcId   destProc);

 void simpleRecvInfo(void    *srcBlkAddr,
                     void    *destBlkAddr,
		     int      dataSize,
		     ProcId   srcProc);

#ifdef NOT_YET
 void calcSendInfo(const Layout  &srcL,
                   const void    *srcBlkAddr,
                   const vector<void*>& destBlockAddrs,
                   const Layout  &destL);

 void calcRecvInfo(const Layout  &srcL,
                   const vector<const void*>& srcBlkAddrs,
                   void    *destBlkAddr,
                   const Layout  &destL);
#endif // NOT_YET

 void procXferStructs();
 
 int  getSegSize(NTuple        &segTup,
                 vector<Falls> *interFalls[]);
 
 void getSegStart(NTuple        &segTup,
                  vector<Falls> *interFalls[],
                  NTuple        &segStart);

 void sortStrideIdx(const int *strides, int len, int *idxArr) const;
 
 bool registerSendRequest(SendRequest *sr);
 
 void unregisterSendRequest(SendRequest *sr);

 void transposeLengths( unsigned int* lengths, Flags flags );
 
 enum Trait {
     NULL_TRAIT=0,
     STATIC_ROUTE,
     NO_LOCAL_DATA_MOVEMENT,
     ALL_MOVEMENT_LOCAL_CONTIGUOUS,
     ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_ROW,
     ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_COL,
     NON_STATIC_ROUTE
 };

// Member data
 int                   m_eltSize;
 bool                  m_isSrc;
 bool                  m_isDest;
 int                   m_sendCount;
 int                   m_recvdCount;
 int                   m_tag;
 int                   m_srcRowStride;
 int                   m_destRowStride;
 
 void                 *m_localSrcAddr;
 void                 *m_localDestAddr;

 CommScope            *m_commScopePtr;
 Trait                 m_trait;
 LocalXferInfo        *m_locXferInfo;
 int                   m_numLocXfers;
 
 bool                  m_srcIsLocal;
 bool                  m_destIsLocal;
 DataMap              *m_srcMap;
 DataMap              *m_destMap;

 SrcXferInfo           m_currSrcXfer;
 DestXferInfo          m_currDestXfer;
 
 vector<Pitfalls>      m_srcPitfalls;
 vector<Pitfalls>      m_destPitfalls;
 int                   m_maxSends;
 int                   m_maxRecvs;
 list<SendRequest *>   m_sendReqs;
 int                   m_numDims;

// methods declared private to prevent their use
//    Default Constructor, Assignment Operator, Copy Constructor
 Route(void);
 Route& operator=(const Route& transfer);
 Route(const Route& other);
};

//                 I N L I N E     Methods
//---------------------------------------------------------------
inline SrcXferInfo::SrcXferInfo() :
        sendInfo(NULL),
        sendReq(NULL),
        numSends(0)
        { return; }


inline DestXferInfo::DestXferInfo() :
        recvInfo(NULL),
        recvReq(NULL),
        numRecvs(0)
        { return; }

inline
void Route::isend(SendRequest &req)
{ this->isend(0, 0, req); }

inline
void Route::send()
{ this->send(0, 0); }

inline
bool Route::registerSendRequest(SendRequest *sr)
{
    m_sendReqs.push_back(sr);
    return(true);
}

inline
void Route::unregisterSendRequest(SendRequest *sr)
{
    m_sendReqs.remove(sr);
}

}//end namespace

#include <RouteTmpl.inl>

#endif // PVTOL_ROUTE_H not defined

