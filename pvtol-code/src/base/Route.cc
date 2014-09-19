/**
 * File: Route.cc
 *    May 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of the Route class which are Not Templated.
 *    A Route object provides the means of doing a real time data re-org.
 *    It is capable of redistributing a data object which is distributed
 *    accross one set of nodes to one distributed on another set of nodes.
 *    Both the starting and ending distributions must be described via
 *    PVTOL Maps.
 *
 *  See Also: RouteTmpl.cc which contains all the templated Route code
 *
 *  $Id: Route.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 *   Author - Jim Daly June 2000
 */
#include <Route.h>
#include <SendRequest.h>
#include <PvtolStatus.h>
#include <NTuple.h>
#include <PvtolProgram.h>
#include <TaskBase.h>
#include <scoped_array.hpp>
#include <unistd.h>
#include <iostream>

#define no_DEBUG_1

namespace ipvtol
{
    using std::string;
    using std::cerr;
    using std::cout;
    using std::endl;
    using boost::scoped_array;


//------------------------------------------------------------------------
//  Method: simpleSendInfo()
//
//  Description: This is for the simple case where we are sending from a
//               single thread to a single thread in a separate process.
//               Precalculate as much info as we can for this src node.
//
//  Inputs:   1.) Local address of src data's block
//            2.) Local address of dest data's block
//            3.) the # of elements to be transfered
//
//  Return: none
//------------------------------------------------------------------------
void Route::simpleSendInfo(void    *srcBlkAddr,
                           void    *destBlkAddr,
			   int      dataSize,
			   ProcId   destProc)
{
    m_currSrcXfer.sendInfo = new SendInfo;

    if (m_srcIsLocal)
      {//                                We care about this src
        m_currSrcXfer.sendInfo->addr            = srcBlkAddr;
        m_currSrcXfer.sendInfo->destAddr        = destBlkAddr;
        m_currSrcXfer.sendInfo->byteSize        = dataSize * m_eltSize;
        m_currSrcXfer.sendInfo->destPfIdx       = 0;
        m_currSrcXfer.sendInfo->destRank        = destProc;
        m_currSrcXfer.sendInfo->sendIsLocal     = false;
        m_currSrcXfer.sendInfo->destAddrOffset  = 0;
        m_currSrcXfer.sendInfo->serStartIdx     = 0;
        m_currSrcXfer.sendInfo->packingRequired = false;//No packing Yet...
        m_currSrcXfer.sendInfo->packInfo        = NULL;

#ifdef _DEBUG_2
        cout << "Rte[" << pvlProcess.getProcId() << "] sSI: "
             << "sendLocal="    << m_currSrcXfer.sendInfo->sendIsLocal
             << " Dest="        << m_currSrcXfer.sendInfo->destPfIdx
             << " segAddr="     << m_currSrcXfer.sendInfo->addr
             << " dest Addr="     << m_currSrcXfer.sendInfo->destAddr
             << " serStartIdx=" << m_currSrcXfer.sendInfo->serStartIdx
             << " byteSize="    << m_currSrcXfer.sendInfo->byteSize
             << endl;
#endif

        m_currSrcXfer.numSends = 1;
        m_currSrcXfer.sendReq  = new PvtolRequest;
        m_maxSends             = 1;
      }//endIf local proc

    return;
}//end simpleSendInfo()


//------------------------------------------------------------------------
//  Method: simpleRecvInfo()
//
//  Description: This is for the simple case where we are sending from a
//               single thread to a single thread in a separate process.
//               Precalculate as much info as we can for this src node.
//
//  Inputs:   1.) Local address of src data's block
//            2.) Local address of dest data's block
//            3.) the # of elements to be transfered
//
//  Return: none
//------------------------------------------------------------------------
void Route::simpleRecvInfo(void    *srcBlkAddr,
                           void    *destBlkAddr,
			   int      dataSize,
			   ProcId   srcProc)
{
    m_currDestXfer.recvInfo = new RecvInfo;

    if (m_destIsLocal)
      {
        m_currDestXfer.recvInfo->addr              = destBlkAddr;
        m_currDestXfer.recvInfo->srcAddr           = srcBlkAddr;
        m_currDestXfer.recvInfo->byteSize          = dataSize * m_eltSize;
        m_currDestXfer.recvInfo->srcPfIdx          = 0;
        m_currDestXfer.recvInfo->srcRank           = srcProc;
        m_currDestXfer.recvInfo->unpackingRequired = false;//No packing Yet
        m_currDestXfer.recvInfo->recvIsLocal       = false;
        m_currDestXfer.recvInfo->packInfo          = NULL;
        m_currDestXfer.recvInfo->srcAddrOffset     = 0;
        m_currDestXfer.recvInfo->serStartIdx       = 0;
#ifdef _DEBUG_2
        cout << "Rte[" << pvlProcess.getProcId() << "] sRI: "
             << "recvLocal=" << m_currDestXfer.recvInfo->recvIsLocal
             << " Src=" << m_currDestXfer.recvInfo->srcPfIdx
             << " byteSize=" << m_currDestXfer.recvInfo->byteSize
             << " loc addr=" << m_currDestXfer.recvInfo->addr
             << " src addr=" << m_currDestXfer.recvInfo->srcAddr
             << endl;
#endif
      }//endIf local proc

    m_currDestXfer.numRecvs = 1;
    m_currDestXfer.recvReq  = new PvtolRequest;
    m_maxRecvs = 1;

    return;
}//end simpleRecvInfo()


//------------------------------------------------------------------------
//  Method: calcSendInfo()
//
//  Description: Precalculate as much info as we can for this src node.
//
//  Inputs:   1.) Src Layout
//            2.) Local address of src data's block
//            3.) Local addresses of dest data's block (one for each dest
//                Node).
//            4.) Dest Layout
//
//  Return: none
//
//   NOTE:   !!!!this only gets called if local proc is a Src!!!!
//------------------------------------------------------------------------
#ifdef NOT_YET
void Route::calcSendInfo(const Layout  &srcL,
                         const void    *srcBlkAddr,
                         const vector<void*>& destBlkAddrs,
                         const Layout  &destL
			)
{
    int            i;
    int            idx, dim;
    SendInfo       sInfo;

    const DistributedLayout &srcDl  = srcL.getDistributedLayout();
    const LocalLayout       &srcLl  = srcL.getLocalLayout();
    const DistributedLayout &destDl = destL.getDistributedLayout();
    const LocalLayout       &destLl = destL.getLocalLayout();

    int         pfDestProcs = 1;
    int         sortedSrcIndices[MAX_DIM];

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//     Prelim Src Info
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    const int  *srcStrides = srcLl.getStrides();
    sortStrideIdx(srcStrides, m_numDims, sortedSrcIndices);

    if (srcStrides[sortedSrcIndices[m_numDims-1]] != 1)
      {
        cerr << "Route: min stride NOT 1, ="
             << srcStrides[sortedSrcIndices[m_numDims-1]]
             << ", Not supported yet" << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
      }

#ifdef PVTOL_DEVELOP
    for (i=0; i<m_numDims; i++) {
        if (srcStrides[i] <= 0)
          {
            cerr << "Route: unsupported stride=" << srcStrides[i]
                 << endl;
            throw NotImplementedYet(__FILE__, __LINE__);
          }
    }//endFor each dim
#endif // PVTOL_DEVELOP

    vector<int> locIndexTuple;
    (m_srcMap.getGrid()).findIndex(locIndexTuple);

#ifdef _DEBUG_3
    cout << "Route[" << pvlProcess.getProcId() << "], in calcSendInfo"
         << " locIndexTuple[0]=" << locIndexTuple[0]
         << " locIndexTuple[1]=" << locIndexTuple[1]
         << endl;
#endif

    vector<Pitfalls>  srcPF;
    int               srcNumP[MAX_DIM];
    int               sortedDimLen[MAX_DIM];

    for (i=0; i<m_numDims; i++) {
        const Pitfalls& pf = srcDl.getPitfalls(i);

        srcNumP[i] = pf.getP();         // save # Ps per dim
        srcPF.push_back(pf);

        sortedDimLen[sortedSrcIndices[i]] =
            srcDl.getLength(sortedSrcIndices[i]);
    }//endFor each Dim in Src

    NTuple  liTuple(m_numDims, srcNumP);
    for (i=0; i<m_numDims; i++)
        liTuple[i] = locIndexTuple[i];  // put into NTuple

//   Calculate the iproc of this src relative to the PITFALLS
//  ---------------------------------------------------------
    int  pfIproc=-1;

    pfIproc = liTuple.serialIndex();

#ifdef _DEBUG_3
    cout << "Route[" << pvlProcess.getProcId() << "], in calcSendInfo"
         << " my pfIproc=" << pfIproc
         << endl;
#endif

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//     Prelim Dest Info
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    int              destNumP[MAX_DIM];

    const RankList & destRanks = m_destMap.getRankList();

    vector<Pitfalls>  destPF;
    for (i=0; i<m_numDims; i++) {
        const Pitfalls& pf = destDl.getPitfalls(i);

        pfDestProcs *= pf.getP();
        destNumP[i]  = pf.getP();

        destPF.push_back(pf);
    }//endFor each Dim in Dest

    if (destRanks.getNumRanks() != pfDestProcs)
      {
        cerr << "PvtolRoute: Map ProcSet != PITFALLS ProcSet, not supported!"
             << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
      }

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//   Determine what needs to be sent from here
//    And where to send it
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    NTuple  srcTuple(m_numDims, srcNumP);
    int     maxLocalSends=0;

    if (m_srcMap.isLocal())
      {//                                We care about this src
            vector<SendInfo>         sendInfos;
            NTuple                   destTuple(m_numDims, destNumP);
            int                      numSrcSends;
            vector<Falls>           *interFalls[MAX_DIM];
            int                      segCoordLen[MAX_DIM];

            for (int dest=0; dest<pfDestProcs; dest++, destTuple++) {
                // Find out if there is a FALLS intersection, which
                //  indicates this src must send something to this dest
                //  There is an intersection at every dimension if there
                //  is data to be communicated.
                //------------------------------------------------------
                bool  realDest    = true;
                int   mult        = 1;
                int   destId      = destRanks.getRank(dest);
                void* destBlkAddr = destBlkAddrs[dest];

                for (dim=0; dim<m_numDims; dim++) {
                    //  Put dims in expected order
                    idx = sortedSrcIndices[dim];
                    interFalls[dim]  = NULL;
                    bool srcHasData  = (srcDl.getLocalLength(dim, srcId) > 0);
                    bool destHasData = (destDl.getLocalLength(dim, destId) > 0);

                    Falls df = destPF[idx].getFalls(destTuple[idx]);
                    Falls sf = srcPF[idx].getFalls(srcTuple[idx]);

                    if ((!srcHasData) || (!destHasData) || (!intersects(sf, df)))
                    {
                        realDest = false;
                        break;
                    }
                    else
                    {
                        vector<Falls> *vfp = new vector<Falls>;

                        *vfp = intersection(sf, df);

#ifdef _DEBUG_2
                        cout << "Route[" << pvlProcess.getProcId() << "] CSI: "
                             << " FallsInters[dim=" << dim <<"] Dest=" << dest
                             << " numFalls=" << vfp->size() << " 1st: "
                             << " L=" << (*vfp)[0].getL()
                             << " R=" << (*vfp)[0].getR()
                             << " S=" << (*vfp)[0].getS()
                             << " N=" << (*vfp)[0].getN()
                             << endl;

                        if (vfp->size() > 1)
                        {
                            cout << "Route[" << pvlProcess.getProcId()
                                 << "] CSI: "
                                 << " FallsInters[dim=" << dim <<"] Dest="
                                 << dest
                                 << " numFalls=" << vfp->size() << " 2nd: "
                                 << " L=" << (*vfp)[1].getL()
                                 << " R=" << (*vfp)[1].getR()
                                 << " S=" << (*vfp)[1].getS()
                                 << " N=" << (*vfp)[1].getN()
                                 << endl;
                        }
#endif
                        interFalls[dim] = vfp;

                        segCoordLen[dim] = 0;

                        for (i=0; i<interFalls[dim]->size(); i++) {
                            if (dim == (m_numDims-1))
                            {//                      Fastest Dim
                                segCoordLen[dim] +=
                                    (*(interFalls[dim]))[i].getN();
                            }
                            else
                            {//                  NOT Fastest Dim
                                segCoordLen[dim] +=
                                    ((*(interFalls[dim]))[i].getR() -
                                     (*(interFalls[dim]))[i].getL() + 1) *
                                    (*(interFalls[dim]))[i].getN();
                            }
                        }//endFor each Falls at this dim

                        if (dim < (m_numDims-1))
                            mult *= segCoordLen[dim];
                    }
                }//end check for FALLs intersection


                if (realDest)
                {
                    NTuple segStartIdx(m_numDims, sortedDimLen);
                    NTuple segTuple(m_numDims, segCoordLen);
                    int    destId  = destRanks.getRank(dest);
                    int    numSegs = segCoordLen[m_numDims-1] * mult;
                    int    locSegIdx[MAX_DIM];
                    for (i=0; i<MAX_DIM; i++)
                        locSegIdx[i] = 0;
                    int locSrcBlkOffset  = srcLl.getLocalBlockIndex(locSegIdx) *
                        m_eltSize;
                    int locDestBlkOffset =
                        destLl.getLocalBlockIndex(locSegIdx) * m_eltSize;

#ifdef _DEBUG_2
                    if (locSrcBlkOffset)
                        cout << "  !####! locSrcBlkOffset=" << locSrcBlkOffset
                             << endl;
#endif

                    for (int seg=0; seg<numSegs; seg++, segTuple++) {
                        int   segSize = getSegSize(segTuple, interFalls);

                        getSegStart(segTuple, interFalls, segStartIdx);

                        //  Determine the local address of the 1st element of
                        //   this segment.
                        //----------------------------------------------------
                        int  blkStartIdx;

                        srcDl.globalToLocal(segStartIdx.getCoord(),
                                            srcId,
                                            locSegIdx);
                        blkStartIdx = srcLl.getLocalBlockIndex(locSegIdx);

                        void *segAddr =
                            static_cast<char *>(const_cast<void *>(srcBlkAddr)) +
                            (blkStartIdx * m_eltSize);
#ifdef _DEBUG_2
                        cout << "Rte[" << pvlProcess.getProcId() << "] CSI: "
                             << "segAddr starting at: " << segAddr << endl;
#endif

                        //  Determine the serial index of the 1st element of
                        //   this segment.
                        //----------------------------------------------------
                        int  segSerIdx = segStartIdx.serialIndex();

                        // dest Addr: we can compute the offset from the
                        //     start of block here. This still needs to be
                        //     adjusted by adding in the start of block addr
                        //     this is only used if the transfer turns out
                        //     to be local AND to decide if we can concatenate
                        //     multiple sends to the same destination.
                        //----------------------------------------------------
                        destDl.globalToLocal(segStartIdx.getCoord(),
                                             destId,
                                             locSegIdx);

                        void *segAddrDest = static_cast<char *>(destBlkAddr) +
                            (destLl.getLocalBlockIndex(locSegIdx) * m_eltSize);
                        blkStartIdx = destLl.getLocalBlockIndex(locSegIdx);


                        bool   concatSend = false;
                        char  *nextAddress = NULL;

                        numSrcSends = sendInfos.size();

                        if (numSrcSends  &&
                            (sendInfos[numSrcSends-1].destPfIdx == dest)
                            )
                        {//   Check if we should just add on to another send

                            // To correctly calculate if sends are
                            // concatinated, nextAddress must include
                            // localSrcBlkOffset.
                            nextAddress = static_cast<char *>
                                (sendInfos[numSrcSends-1].addr) +
                                locSrcBlkOffset;

                            nextAddress += sendInfos[numSrcSends-1].byteSize;

                            if (nextAddress == segAddr)
                            {//   same src side address
                                int idxDiff;
                                idxDiff=segSerIdx-sendInfos[numSrcSends-1].
                                    serStartIdx;
                                if (idxDiff ==
                                    (sendInfos[numSrcSends-1].byteSize /
                                     m_eltSize) )
                                    concatSend = true;
                            }//endIf this is next addr
                        }//endIf already calculated some sInfos

                        if (concatSend)
                          {
#ifdef _DEBUG_2
                            cout << "Rte[" << pvlProcess.getProcId() << "] CSI: "
                                 << " Concatinating Send!!" << endl;
#endif
                            sendInfos[numSrcSends-1].byteSize +=
                                segSize * m_eltSize;
                          }
                        else
                          {
                            if (m_trait == STATIC_ROUTE)
                              {
                                sInfo.addr     = segAddr;
                                sInfo.destAddr = segAddrDest;
                              }
                            else
                              {
                                sInfo.addr = static_cast<char *>(segAddr)-
                                    locSrcBlkOffset;
                                sInfo.destAddr = static_cast<char *>
                                    (segAddrDest)-locDestBlkOffset;
                              }

                            sInfo.byteSize        = segSize * m_eltSize;
                            sInfo.destPfIdx       = dest;
                            sInfo.sendIsLocal     = false;

                            sInfo.destAddrOffset  = (blkStartIdx * m_eltSize) -
                                locDestBlkOffset;
                            sInfo.serStartIdx     = segSerIdx;
                            sInfo.packingRequired =
                                false;//No packing support Yet...
                            sInfo.packInfo        = NULL;

#ifdef _DEBUG_2
                            cout << "Rte[" << pvlProcess.getProcId() << "] CSI: "
                                 << "sendLocal="    << sInfo.sendIsLocal
                                 << " Dest="        << sInfo.destPfIdx
                                 << " segAddr="     << sInfo.addr
                                 << " dest Addr="     << sInfo.destAddr
                                 << " serStartIdx=" << sInfo.serStartIdx
                                 << " byteSize="    << sInfo.byteSize
                                 << endl;
#endif
                            sendInfos.push_back(sInfo);
                          }//endIf concatenate send

                    }//endFor each Segment
                }//endIf real destination

                for (i=0; i<m_numDims; i++) {
                    if (interFalls[i] != NULL)
                        delete interFalls[i];
                    else
                        break;
                }//endFor delete each Falls

            }//endFor each possible destination

            numSrcSends = sendInfos.size();
            if (numSrcSends)
              {
                m_currSrcXfer.sendInfo = new SendInfo[numSrcSends];

                // Keep track of the maximum # of sends we could do.
                //  The max # of sends is >= the # of local sends
                if (numSrcSends > maxLocalSends)
                    maxLocalSends = numSrcSends;

                for (i=0; i<numSrcSends; i++)
                    m_currSrcXfer.sendInfo[i] = sendInfos[i];

                m_currSrcXfer.numSends = numSrcSends;

                m_currSrcXfer.sendReq = new PvtolRequest[numSrcSends];

                if (numSrcSends > m_maxSends)
                    m_maxSends = numSrcSends;
              }//endIf there are some sends

      }//endIf local proc

    return;
}//end calcSendInfo()
#endif // NOT_YET


//------------------------------------------------------------------------
//  Method: calcRecvInfo()
//
//  Description: Precalculate as much info as we can for this src node.
//
//  Inputs:   1.) Src Layout
//            2.) Local address of dest's data block
//            3.) Dest Layout
//
//  Return: none
//
//   NOTE:   !!!!this only gets called if local proc is a Src!!!!
//------------------------------------------------------------------------
#ifdef NOT_YET
void Route::calcRecvInfo(const Layout &srcL,
                         const vector<const void*>& srcBlkAddrs,
                         void *destBlkAddr,
                         const Layout &destL
			)
{
    int         i;
    int         idx, dim;
    RecvInfo    rInfo;

    const DistributedLayout &destDl = destL.getDistributedLayout();
    const LocalLayout       &destLl = destL.getLocalLayout();
    const DistributedLayout &srcDl = srcL.getDistributedLayout();
    const LocalLayout       &srcLl = srcL.getLocalLayout();

    int         pfSrcProcs=1;
    int         pfDestProcs=1;
    int         sortedDestIndices[MAX_DIM];

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//     Prelim Dest Info
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    const int  *destStrides = destLl.getStrides();
    sortStrideIdx(destStrides, m_numDims, sortedDestIndices);

    const RankList & destRanks = m_destMap.getRankList();

//   Check that destination data is dense, i.e. it fastest
//     dimension's stride is = 1
//  ------------------------------------------------------
    if (destStrides[sortedDestIndices[m_numDims-1]] != 1)
    {
        cerr << "Route: min stride NOT 1, ="
             << destStrides[sortedDestIndices[m_numDims-1]]
             << ", Not supported yet" << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
    }

    for (i=0; i<m_numDims; i++) {
        if (destStrides[i] <= 0)
        {
            cerr << "Route: unsupported stride=" << destStrides[i]
                 << endl;
            throw NotImplementedYet(__FILE__, __LINE__);
        }
    }

    vector<int> locIndexTuple;
    (m_destMap.getGrid()).findIndex(locIndexTuple);

    vector<Pitfalls>  destPF;
    int                          destNumP[MAX_DIM];
    int                          sortedDimLen[MAX_DIM];

    for (i=0; i<m_numDims; i++) {
        const Pitfalls& pf = destDl.getPitfalls(i);

        destNumP[i]  = pf.getP();         // save # Ps per dim

        destPF.push_back(pf);

        sortedDimLen[sortedDestIndices[i]] =
            destDl.getLength(sortedDestIndices[i]);
    }//endFor each Dim in Dest

    NTuple  liTuple(m_numDims, destNumP);
    for (i=0; i<m_numDims; i++)
        liTuple[i] = locIndexTuple[i];  // put into NTuple

//   Calculate the iproc of this dest relative to the PITFALLS
//  ---------------------------------------------------------
    int  pfIproc = -1;// iproc relative to the PITFALLS

    pfIproc = liTuple.serialIndex();

#ifdef _DEBUG_3
    cout << "Route[" << pvlProcess.getProcId() << "], in calcRecvInfo"
         << " my pfIproc=" << pfIproc
         << endl;
#endif

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//     Prelim Src Info
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    int              srcNumP[MAX_DIM];

    const RankList & srcRanks = m_srcMap.getRankList();

    vector<Pitfalls>  srcPF;

    for (i=0; i<m_numDims; i++) {
        const Pitfalls& pf = srcDl.getPitfalls(i);

        pfSrcProcs *= pf.getP();
        srcNumP[i]  = pf.getP();

        srcPF.push_back(pf);
    }//endFor each Dim in Src

    if (srcRanks.getNumRanks() != pfSrcProcs)
    {
        cerr << "PvtolRoute: Map ProcSet != PITFALLS ProcSet, not supported!"
             << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
    }

//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//     Determine what will be sent here and where
//  will it come from.
//  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    NTuple destTuple(m_numDims, destNumP);

    if (m_destMap.isLocal())
      {
      DestXferInfo       rXferInfo;
      vector<RecvInfo>   recvInfos;
      NTuple             srcTuple(m_numDims, srcNumP);
      int                numDestSends;
      vector<Falls>     *interFalls[MAX_DIM];
      int                segCoordLen[MAX_DIM];
      int                destId = destRanks.getRank(dest);

      for (int src=0; src<pfSrcProcs; src++, srcTuple++) {
          int  srcId = srcRanks.getRank(src);
          const void* srcBlkAddr = srcBlkAddrs[src];

          // Find out if there is a FALLS intersection, which
          //  indicates this dest must receive something from this src
          //  There is an intersection at every dimension if there
          //  is data to be communicated.
          //-----------------------------------------------------------
          bool realSrc = true;
          int  mult=1;

          for (dim=0; dim<m_numDims; dim++) {
              //  Put dims in expected order
              idx = sortedDestIndices[dim];
              interFalls[dim] = NULL;

              Falls sf = srcPF[idx].getFalls(srcTuple[idx]);
              Falls df = destPF[idx].getFalls(destTuple[idx]);
              bool srcHasData = (srcDl.getLocalLength(dim, srcId) > 0);
              bool destHasData = (destDl.getLocalLength(dim, destId) > 0);

              if ((!srcHasData) || (!destHasData) || (!intersects(df, sf)))
                {
                    realSrc = false;
                    break;
                }
               else
                {
                    vector<Falls> *vfp = new vector<Falls>;
                    *vfp = intersection(sf, df);

#ifdef _DEBUG_2
                    cout << "Route[" << pvlProcess.getProcId() << "] CRI: "
                         << " FallsInters[dim=" << dim <<"] Src=" << src
                         << " numFalls=" << vfp->size() << " 1st: "
                         << " L=" << (*vfp)[0].getL()
                         << " R=" << (*vfp)[0].getR()
                         << " S=" << (*vfp)[0].getS()
                         << " N=" << (*vfp)[0].getN()
                         << endl;

                    if (vfp->size() > 1)
                      {
                        cout << "Route[" << pvlProcess.getProcId()
                             << "] CRI: "
                             << " FallsInters[dim=" << dim <<"] Src=" << src
                             << " numFalls=" << vfp->size() << " 2nd: "
                             << " L=" << (*vfp)[1].getL()
                             << " R=" << (*vfp)[1].getR()
                             << " S=" << (*vfp)[1].getS()
                             << " N=" << (*vfp)[1].getN()
                             << endl;
                      }
#endif
                    interFalls[dim] = vfp;
                    segCoordLen[dim] = 0;

                    for (i=0; i<interFalls[dim]->size(); i++) {
                        if (dim == (m_numDims-1))
                          {//                      Fastest Dim
                            segCoordLen[dim] +=
                                (*(interFalls[dim]))[i].getN();
                          }
                         else
                          {//                  NOT Fastest Dim
                            segCoordLen[dim] +=
                                    ((*(interFalls[dim]))[i].getR() -
                                     (*(interFalls[dim]))[i].getL() + 1) *
                                    (*(interFalls[dim]))[i].getN();
                          }
                    }//endFor each Falls at this dim

                    if (dim < (m_numDims-1))
                            mult *= segCoordLen[dim];
                }
          }//end check for FALLs intersection

          if (realSrc)
            {
              NTuple segStartIdx(m_numDims, sortedDimLen);
              NTuple segTuple(m_numDims, segCoordLen);
              NodeId destId  = destRanks.getRank(dest);
              int    numSegs = segCoordLen[m_numDims-1] * mult;
              int    locSegIdx[MAX_DIM];
              for (i=0; i<MAX_DIM; i++)
                        locSegIdx[i] = 0;
              int    locDestBlkOffset =
                        destLl.getLocalBlockIndex(locSegIdx) * m_eltSize;

              for (i=0; i<MAX_DIM; i++)
	                     locSegIdx[i] = 0;
              int locSrcBlkOffset =
                        srcLl.getLocalBlockIndex(locSegIdx) * m_eltSize;

#ifdef _DEBUG_2
              cout << "  !##dest##! locDestBlkOffset=" << locDestBlkOffset
                   << endl;
              if (locSrcBlkOffset)
                        cout << "  !##dest##! locSrcBlkOffset="
                             << locSrcBlkOffset << endl;
#endif


              for (int seg=0; seg<numSegs; seg++, segTuple++) {
                  int   segSize = getSegSize(segTuple, interFalls);

                  getSegStart(segTuple, interFalls, segStartIdx);

                  //  Determine the local address of the 1st element of
                  //   this segment.
                  //----------------------------------------------------
                  int  blkStartIdx;

                  destDl.globalToLocal(segStartIdx.getCoord(),
                                       destId,
                                       locSegIdx);
                  blkStartIdx = destLl.getLocalBlockIndex(locSegIdx);

                  void *segAddr = static_cast<char *>(destBlkAddr) +
                            (blkStartIdx * m_eltSize);

                  //  Determine the serial index of the 1st elenment of
                  //   this segment.
                  //----------------------------------------------------
                  int  segSerIdx = segStartIdx.serialIndex();

                  // src Addr: we can compute the offset from the
                  //     start of block here. This is only
                  //     used to decide if we can concatenate
                  //     multiple sends to the same destination.
                  //----------------------------------------------------
                  srcDl.globalToLocal(segStartIdx.getCoord(),
                                      srcId,
                                      locSegIdx);

                  void *segAddrSrc = (char *)(srcBlkAddr) +
                            (srcLl.getLocalBlockIndex(locSegIdx) * m_eltSize);
                  blkStartIdx = srcLl.getLocalBlockIndex(locSegIdx);

                  bool   concatRecv = false;
                  char  *nextAddress = NULL;

                  numDestSends = recvInfos.size();

                  if (numDestSends &&
                      (recvInfos[numDestSends-1].srcPfIdx == src)
                      )
                    {//   Check if we should just add on to another recv

                      // To correctly calculate if receives are
                      // concatinated, nextAddress must include
                      // localDestBlkOffset.
                      nextAddress = static_cast<char *>
                                (recvInfos[numDestSends-1].addr) +
                                locDestBlkOffset ;

                      nextAddress += recvInfos[numDestSends-1].byteSize;

                      if (nextAddress == segAddr)
                        {//   same dest side address
#ifdef _DEST_OFF_CALC_OK
                          int srcOff =
                                    recvInfos[numDestSends-1].srcAddrOffset;
                          srcOff += recvInfos[numDestSends-1].byteSize;

                          if (srcOff == (blkStartIdx * m_eltSize))
                                    concatRecv = true;
#else
                          int idxDiff;
                          idxDiff=segSerIdx-
                                    recvInfos[numDestSends-1].serStartIdx;
                          if (idxDiff ==
                                    (recvInfos[numDestSends-1].byteSize /
                                     m_eltSize) )
                                 concatRecv = true;
#endif // _DEST_OFF_CALC_OK
                        }//endIf seg is at next addr
                    }//endIf already calculated some sInfos

                  if (concatRecv)
                    {
#ifdef _DEBUG_2
                      cout << "Rte[" << pvlProcess.getProcId() << "] CRI: "
                           << " Concatinating Recv!!" << endl;
#endif
                      recvInfos[numDestSends-1].byteSize +=
                                segSize * m_eltSize;
                    }
                  else
                    {
                      if (m_trait == STATIC_ROUTE)
                        {
                          rInfo.addr    = segAddr;
                          rInfo.srcAddr = segAddrSrc;
                        }
                      else
                        {
                          rInfo.addr    = static_cast<char *>(segAddr) -
                                    locDestBlkOffset;
                          rInfo.srcAddr = static_cast<char *>(segAddrSrc) -
                                    locSrcBlkOffset;
                        }

                      rInfo.byteSize      = segSize * m_eltSize;
                      rInfo.srcPfIdx      = src;
                      rInfo.unpackingRequired = false;//No packing support Yet
                      rInfo.recvIsLocal   = false;
                      rInfo.packInfo      = NULL;
                      rInfo.srcAddrOffset = blkStartIdx * m_eltSize;
                      rInfo.serStartIdx   = segSerIdx;

                      recvInfos.push_back(rInfo);
#ifdef _DEBUG_2
                      cout << "Rte[" << pvlProcess.getProcId() << "] CRI: "
                           << "recvLocal=" << rInfo.recvIsLocal
                           << " Src=" << rInfo.srcPfIdx
                           << " byteSize=" << rInfo.byteSize
                           << " loc addr=" << rInfo.addr
                           << " src addr=" << rInfo.srcAddr
                           << endl;
#endif
                    }//endif we must concatenate
              }//endFor each send segment

            }//endIf this is a real src

          for (i=0; i<m_numDims; i++) {
              if (interFalls[i] != NULL)
                  delete interFalls[i];
              else
                  break;
          }//endFor delete each Falls
      }//endFor each possible src

      numDestSends = recvInfos.size();
      if (numDestSends)
        {
          m_currDestXfer.recvInfo = new RecvInfo[numDestSends];

          for (i=0; i<numDestSends; i++)
                    m_currDestXfer.recvInfo[i] = recvInfos[i];

          m_currDestXfer.numRecvs = numDestSends;

          m_currDestXfer.recvReq = new PvtolRequest[numDestSends];

          if (numDestSends > m_maxRecvs)
                    m_maxRecvs = numDestSends;

        }//endIf there are some sends
      }//endIf local proc

    return;
}//end calcRecvInfo()
#endif // NOT_YET


//------------------------------------------------------------------------
//  Method: procXferStructs()
//
//  Description: Process the transfer structures we built earlier
//
//  Inputs: there are No formal arguments.
//          it uses m_currSrcXfer & m_currDestXfer
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::procXferStructs()
{
    int            i, j;
    int            persist;
    int            destRank, srcRank;
    PvtolProgram   prog;
    //TaskBase&      ct       = prog.getCurrentTask();
    //int            myRank   = ct.getGlobalThreadRank();
    //ProcId         myProcId = prog.getProcId();

    if (m_currSrcXfer.sendInfo != NULL)
      {
	// set up each send
        //const RankList &destRanks =  m_destMap->getRankList();

        for (i=0; i<m_currSrcXfer.numSends; i++) {
#ifdef NOT_YET
	      destRank = m_commScopePtr->rank(
                    destRanks[m_currSrcXfer.sendInfo[i].destPfIdx]);
#endif // NOT_YET
	      m_currSrcXfer.sendInfo[i].sendIsLocal = false;
	      m_currSrcXfer.sendInfo[i].destRank    = destRank;
        }//endFor each send

        if (m_trait == STATIC_ROUTE)
          {
                // use the sendInfos to build the persisant sends
                int numSendReqs = m_currSrcXfer.numSends;

                if (numSendReqs)
                {
                    for (persist=0, j=0; j<m_currSrcXfer.numSends; j++) {
		      m_commScopePtr->sendInit(
                                m_currSrcXfer.sendInfo[j].addr,
                                m_currSrcXfer.sendInfo[j].destAddr,
                                m_currSrcXfer.sendInfo[j].byteSize,
                                m_currSrcXfer.sendInfo[j].destRank,
                                m_tag,
                                m_currSrcXfer.sendReq[persist]);
                            persist++;
                    }//endFor each send
#ifdef _DEBUG_2
                    cout << "Rte[" << pvlProcess.getProcId() << "] pXS: "
                         << "Built " << persist << " Persistent sends"
                         << endl;
#endif
                }//endIf there are sendReqs to build
          }//endIf Static
      }//endIf this is a src

    if (m_currDestXfer.recvInfo != NULL)
      {
	//  set up each receive
        for (i=0; i<m_currDestXfer.numRecvs; i++) {
#ifdef NOT_YET
	        srcRank = m_commScopePtr->rank(
                    srcRanks[m_currDestXfer.recvInfo[i].srcPfIdx]);
#endif // NOT_YET
		m_currDestXfer.recvInfo[i].recvIsLocal = false;
		m_currDestXfer.recvInfo[i].srcRank     = srcRank;
        }//endFor each recv

        if (m_trait == STATIC_ROUTE)
          {
                // use the recvInfos to build the persisant sends
                int   numRecvReqs = m_currDestXfer.numRecvs;

                if (numRecvReqs)
		  {
                    for (persist=0, j=0; j<m_currDestXfer.numRecvs; j++) {
		      m_commScopePtr->recvInit(
                                m_currDestXfer.recvInfo[j].srcAddr,
                                m_currDestXfer.recvInfo[j].addr,
                                m_currDestXfer.recvInfo[j].byteSize,
                                m_currDestXfer.recvInfo[j].srcRank,
                                m_tag,
                                m_currDestXfer.recvReq[persist]);
                            persist++;
                    }//endFor each recv
                }//endIf there are any RecvReqs to build
          }//endIf Static
      }//endIf this is a destination

    return;
}//end procXferStructs()

//------------------------------------------------------------------------
//  Method: send(int srcOff, int destOff)
//
//  Description: Blocking data send/recv call
//
//  Inputs: two integers a src and destination offset
//
//  Return: none, upon return the send has completed
//
//------------------------------------------------------------------------
void Route::send(int srcOff, int destOff)
{
    switch (m_trait) {
        case STATIC_ROUTE:
            this->staticSend();
            break;

        case NON_STATIC_ROUTE:
            this->nonStaticSend(srcOff, destOff);
            break;

        case ALL_MOVEMENT_LOCAL_CONTIGUOUS:
        case ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_ROW:
        case ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_COL:
            this->localCopy(srcOff, destOff);
            break;

        case NO_LOCAL_DATA_MOVEMENT:
            break;// DO Nothing

        default:
            cerr << "RteSend: unknown trait " << m_trait << endl;
            throw Exception("PvlRoute: send() FAILED!", __FILE__, __LINE__);
    }//end case of trait

    return;
}//end send(int, int)


//------------------------------------------------------------------------
//  Method: isend(int srcOff, int destOff, SendRequest &req)
//
//  Description: Non-Blocking data send/recv
//
//  Inputs: two dummy integers and a reference to a SendRequest.
//
//  Return: none function return.
//          the user uses the SendRequest to determine when the
//          send has completed.
//
//------------------------------------------------------------------------
void Route::isend(int srcOff, int destOff, SendRequest &req)
{
    switch (m_trait) {
        case STATIC_ROUTE:
            this->iStaticSend(req);
            break;

        case NON_STATIC_ROUTE:
            this->iNonStaticSend(srcOff, destOff, req);
            break;

        case ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_ROW:
        case ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_COL:
        case ALL_MOVEMENT_LOCAL_CONTIGUOUS:
            this->localCopy(srcOff, destOff);
            req.setDone();
            break;

        case NO_LOCAL_DATA_MOVEMENT:
            req.setDone();
            break;// DO Nothing

        default:
            cerr << "RteiSend: unknown trait " << m_trait << endl;
            throw Exception("PvlRoute: isend() FAILED!", __FILE__, __LINE__);
    }//end case of trait
    return;
}//end isend(int, int, SendRequest&)


//------------------------------------------------------------------------
//  Method: localCopy()
//
//  Description: copy the data, no communication is required.
//
//  Inputs: two dummy integers and a reference to a SendRequest.
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::localCopy(int srcOff, int destOff)
{
#ifdef _DEBUG_2
    cout << "Route[" << pvlProcess.getProcId() << "], in localCopy()"
         << endl;
#endif

#ifdef _OFFSETS_NOT_IMPL
    if (srcOff || destOff)
    {
        cerr << "Route[" << pvlProcess.getProcId() << "], in localCopy()"
             << " NO curr support for offsets!" << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
    }
#endif

    if (m_isSrc)
    {//              Note: Srcs and Dests are the same node
        if (m_trait == ALL_MOVEMENT_LOCAL_CONTIGUOUS)
        {
	    char   *srcAddr, *destAddr;

            srcAddr = static_cast<char *>(const_cast<void *>(m_localSrcAddr)) +
                (srcOff*m_eltSize);
            destAddr = static_cast<char *>(m_localDestAddr) +
                (destOff*m_eltSize);

            memcpy(destAddr, srcAddr, m_sendCount);
        }
        else if (m_trait == ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_ROW)
        {
	    destOff *= m_eltSize;
	    srcOff  *= m_eltSize;

            for (int i=0; i<m_numLocXfers; i++) {
	        memcpy(static_cast<char*>(m_locXferInfo[i].destAddr) + destOff,
		       static_cast<char*>(const_cast<void*>(m_locXferInfo[i].srcAddr)) + srcOff,
                       m_locXferInfo[i].byteSize);
            }//endFor each send
        }
	else if (m_trait == ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_COL)
	{
	    if (m_eltSize == sizeof (long))
	    {
	        long   *srcAddr, *destAddr;

	        srcAddr = static_cast<long*>(const_cast<void *>(m_localSrcAddr)) + srcOff;
	        destAddr = static_cast<long*>(m_localDestAddr) + destOff;

	        for (int i=0; i<m_numLocXfers; i++) {
  	            *destAddr = *srcAddr;
	            destAddr += m_destRowStride;
	            srcAddr  += m_srcRowStride;
	        }//endFor each copy
	    }
#ifdef NOT_YET
	    else if (m_eltSize == sizeof (Complex<long>))
	    {
	        Complex<long>   *srcAddr, *destAddr;

	        srcAddr = static_cast<Complex<long>* >(const_cast<void *>(m_localSrcAddr)) + srcOff;
	        destAddr = static_cast<Complex<long>* >(m_localDestAddr) + destOff;

	        for (int i=0; i<m_numLocXfers; i++) {
  	            *destAddr = *srcAddr;
	            destAddr += m_destRowStride;
	            srcAddr  += m_srcRowStride;
	        }//endFor each copy
	    }
	    // this should be fast too.
	    else if (m_eltSize == sizeof (Complex<double>))
	    {
	        Complex<double>   *srcAddr, *destAddr;

	        srcAddr = static_cast<Complex<double>* >(const_cast<void *>(m_localSrcAddr)) + srcOff;
	        destAddr = static_cast<Complex<double>* >(m_localDestAddr) + destOff;

	        for (int i=0; i<m_numLocXfers; i++) {
  	            *destAddr = *srcAddr;
	            destAddr += m_destRowStride;
	            srcAddr  += m_srcRowStride;
	        }//endFor each copy
	    }
#endif // NOT_YET
	    else
	    {
	        // Have to use memcpy in the default cast because we don't know
	        // how big the elements are
  	        char   *srcAddr, *destAddr;
		int eltSize = m_eltSize;

	        srcAddr = static_cast<char*>(const_cast<void *>(m_localSrcAddr)) +
                    (srcOff*eltSize);
	        destAddr = static_cast<char*>(m_localDestAddr) +
                    (destOff*eltSize);

	        m_destRowStride *= eltSize;
	        m_srcRowStride  *= eltSize;

	        for (int i=0; i<m_numLocXfers; i++) {
		    memcpy(destAddr, srcAddr, eltSize);
	            destAddr += m_destRowStride;
	            srcAddr  += m_srcRowStride;
	        }//endFor each copy
	    }
	}

    }//endIf this is a Src and Dest

    return;
}//end localCopy()


//------------------------------------------------------------------------
//  Method: staticSend()
//
//  Description: Sends the data. The send was set up previously as a
//               "static send". By static we mean that no offsets from
//               the originally specified data block(s) are used.
//               This is a "Blocking Send", when it returns the data
//           may be reused.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::staticSend()
 {
     int    i;
     PvtolStatus stat;

//     Post the sends and the recvs
//    -------------------------------------------------*
#ifdef _DEBUG_2
     cout << "Rte[" << pvlProcess.getProcId() << "], SS-NonDynamic"
              << endl;
#endif
     int nonLocalSends;
     int nonLocalRcvs;
     if (m_isDest)
       {
          nonLocalRcvs = m_currDestXfer.numRecvs;

          for (i=0; i<nonLocalRcvs; i++) {
                 (m_currDestXfer.recvReq[i]).start();
#ifdef _DEBUG_2
                 cout << "Rte[" << pvlProcess.getProcId() << "], "
                      << "started recv " << i
                      << endl;
#endif
          }//endFor each receive to post
       }//endIf this is a Dest

     if (m_isSrc)
       {
          nonLocalSends = m_currSrcXfer.numSends;

          for (i=0; i<nonLocalSends; i++) {
                 (m_currSrcXfer.sendReq[i]).start();
#ifdef _DEBUG_2
                 cout << "Rte[" << pvlProcess.getProcId() << "], "
                      << "started send " << i
                      << endl;
#endif
          }//endFor each send to make

       }//endIf this is a Src

//     WAIT for the completion of the sends & recvs
//    -------------------------------------------------*
     if (m_isDest)
       {
         if (nonLocalRcvs)
           {
             for (i=0; i<nonLocalRcvs; i++) {
                     (m_currDestXfer.recvReq[i]).wait(stat);
             }
           }
       }//endIf this is a Dest

     if (m_isSrc)
       {
         if (nonLocalSends)
           {
             for (i=0; i<nonLocalSends; i++) {
                     (m_currSrcXfer.sendReq[i]).wait(stat);
             }//endFor each send to make
           }
       }//endIf this is a Src

     return;
 }//end staticSend()


//------------------------------------------------------------------------
//  Method: nonStaticSend()
//
//  Description: Sends the data. This version of the send accepts offsets.
//               Because of this ability to change the offsets this
//               version of send would be expected to be slower than the
//               staticSend. How much slower depends on the underlying
//               transport mechanism.
//               This is a "Blocking Send", when it returns the data buffs
//           have been sent/recvd as appropriate for the local proc.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::nonStaticSend(int srcOff, int destOff)
{
    int     i;
    char   *srcAddr, *destAddr;

#ifdef _DEBUG_2
    cout << "Route[" << pvlProcess.getProcId() << "], in nonStaticSend()"
         << endl;
#endif
    if (m_destIsLocal && (m_currDestXfer.recvInfo != NULL))
    {
        for (i=0; i<m_currDestXfer.numRecvs; i++) {
	  destAddr = static_cast<char *>
	    (m_currDestXfer.recvInfo[i].addr) + (destOff * m_eltSize);
	  srcAddr = static_cast<char *>
	    (m_currDestXfer.recvInfo[i].srcAddr) + (srcOff * m_eltSize);
#ifdef _DEBUG_2
	  cout << "RteNSS[" << pvlProcess.getProcId() << "], irecv" << i
	       << " destAdd=" << static_cast<void *>(destAddr)
	       << " srcAdd=" << static_cast<void *>(srcAddr)
	       << " size=" << m_currDestXfer.recvInfo[i].byteSize
	       << " srcRank=" << m_currDestXfer.recvInfo[i].srcRank
	       << " tag=" << m_tag
	       << endl;
#endif
	  m_commScopePtr->irecv(srcAddr,
			    destAddr,
			    m_currDestXfer.recvInfo[i].byteSize,
			    m_currDestXfer.recvInfo[i].srcRank,
			    m_tag,
			    m_currDestXfer.recvReq[i]);
        }//endFor each receive
    }//endIf this is a Dest

    if (m_srcIsLocal && (m_currSrcXfer.sendInfo != NULL))
    {
        for (i=0; i<m_currSrcXfer.numSends; i++)
        {
	  srcAddr = static_cast<char *>
	    (m_currSrcXfer.sendInfo[i].addr) + (srcOff * m_eltSize);
	  destAddr = static_cast<char *>
	    (m_currSrcXfer.sendInfo[i].destAddr) +
	    (destOff * m_eltSize);

#ifdef _DEBUG_2
	  cout << "RteNSS[" << pvlProcess.getProcId() << "], send" << i
	       << " destAdd=" << static_cast<void *>(destAddr)
	       << " srcAdd=" << static_cast<void *>(srcAddr)
	       << " size=" << m_currSrcXfer.sendInfo[i].byteSize
	       << " destRank=" << m_currSrcXfer.sendInfo[i].destRank
	       << " tag=" << m_tag
	       << endl;
#endif
	  m_commScopePtr->send(srcAddr,
			   destAddr,
			   m_currSrcXfer.sendInfo[i].byteSize,
			   m_currSrcXfer.sendInfo[i].destRank,
			   m_tag);
        }//endFor each send

    }//endIf this is a Src

    if (m_destIsLocal && (m_currDestXfer.recvInfo != NULL))
    {
        PvtolStatus   stat;

        for (i=0; i<m_currDestXfer.numRecvs; i++) {
	  (m_currDestXfer.recvReq[i]).wait(stat);
        }//endFor each receive
    }//endIf this is a Dest

    return;
}//end nonStaticSend()


//------------------------------------------------------------------------
//  Method: iNonStaticSend()
//
//  Description: Sends the data. This version of the send accepts offsets.
//               Because of this ability to change the offsets this
//               version of send would be expected to be slower than the
//               staticSend. How much slower depends on the underlying
//               transport mechanism.
//               This is a "NonBlocking Send", when it returns the data
//           buffers may still be in use.
//
//  Inputs: integer offsets into the blocks of where the src and dest
//          buffers are.
//          a SendRequest ref which, upon return, can be interogated as
//             to the status of the comm.
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::iNonStaticSend(int srcOff, int destOff, SendRequest &sendReq)
{
    int     i, numIssued;
    char   *srcAddr, *destAddr;

#ifdef _DEBUG_1
    PvtolProgram   prog;
    int procid   = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid     = ct.getTaskID();
    int threadid   = ct.getGlobalThreadRank();
    cout << "Rte[" << procid << "," << taskid << "," << threadid
         << "], in iNonStaticSend()"
         << endl;
#endif
    if (!sendReq.preset())
        sendReq.setup(*this);

    sendReq.setDone();

    if (m_destIsLocal && (m_currDestXfer.recvInfo != NULL))
    {
        for (i=0, numIssued=0; i<m_currDestXfer.numRecvs; i++) {
	  destAddr = static_cast<char *>
	    (m_currDestXfer.recvInfo[i].addr) + (destOff * m_eltSize);
	  srcAddr = static_cast<char *>
	    (m_currDestXfer.recvInfo[i].srcAddr) + (srcOff * m_eltSize);
	  m_commScopePtr->irecv(srcAddr,
			    destAddr,
			    m_currDestXfer.recvInfo[i].byteSize,
			    m_currDestXfer.recvInfo[i].srcRank,
			    m_tag,
			    sendReq.m_recvRequest[numIssued]);
	  ++numIssued;

#ifdef _DEBUG_1
         cout << "Rte[" << procid << "," << taskid << "," << threadid
              << "], just did irecv "
	      << " destAddr=" << (int)destAddr
	      << " src=" << m_currDestXfer.recvInfo[i].srcRank
	      << " tag=" << m_tag
              << endl;
#endif
	  sendReq.setNotDone();
        }//endFor each receive

        sendReq.m_numRecvs = numIssued;
    }//endIf this is a Dest

    if (m_srcIsLocal && (m_currSrcXfer.sendInfo != NULL))
    {
        for (i=0, numIssued=0; i<m_currSrcXfer.numSends; i++) {
	  srcAddr = static_cast<char *>
	    (m_currSrcXfer.sendInfo[i].addr) + (srcOff * m_eltSize);
	  destAddr = static_cast<char *>
	    (m_currSrcXfer.sendInfo[i].destAddr) +
	    (destOff * m_eltSize);
	  m_commScopePtr->isend(srcAddr,
			    destAddr,
			    m_currSrcXfer.sendInfo[i].byteSize,
			    m_currSrcXfer.sendInfo[i].destRank,
			    m_tag,
			    sendReq.m_sendRequest[numIssued]);
#ifdef _DEBUG_1
         cout << "Rte[" << procid << "," << taskid << "," << threadid
              << "], just did isend "
	      << " srcAddr=" << (int)srcAddr
	      << " dest=" << m_currSrcXfer.sendInfo[i].destRank
	      << " tag=" << m_tag
              << endl;
#endif
	  ++numIssued;
	  sendReq.setNotDone();
        }//endFor each send

        sendReq.m_numSends = numIssued;

    }//endIf this is a Src

    return;
}//end iNonStaticSend()

//------------------------------------------------------------------------
//  Method: iStaticSend()
//
//  Description: Sends the data. The send was set up previously as a
//               "static send". By static we mean that no offsets from
//               the originally specified data block(s) are used.
//       This is a "Non-Blocking Send", when it returns the
//           data may not have been sent yet. The user must interogate
//       the SendRequest object to find out when the send
//       completes.
//
//  Inputs: a reference to a SendRequest.
//
//  Return: none
//
//------------------------------------------------------------------------
void Route::iStaticSend(SendRequest &req)
{
    int     i;

#ifdef _DEBUG_1
    PvtolProgram   prog;
    int procid   = prog.getProcId();
    TaskBase &ct = prog.getCurrentTask();
    int taskid     = ct.getTaskID();
    int threadid   = ct.getGlobalThreadRank();
    cout << "Rte[" << procid << "," << taskid << "," << threadid
         << "], in iStaticSend()"
         << endl;
#endif

    if (!req.preset())
        req.setup(*this);

    if (m_isDest)
      {
            int  numRecvReqs = req.numRecvs();
#ifdef _DEBUG_1
         cout << "Rte[" << procid << "," << taskid << "," << threadid
              << "], this is dest"
              << endl;
#endif

            for (i=0; i<numRecvReqs; i++) {
                (req.m_recvRequest[i]).start();
            }//endFor each receive to post

            if (numRecvReqs)
                req.setNotDone();// mark the SendRequest as NOT-Done
            req.m_numRecvs = numRecvReqs;
      }//endIf this is a Dest

    if (m_isSrc)
      {
            int  numSendReqs = req.numSends();

#ifdef _DEBUG_1
         cout << "Rte[" << procid << "," << taskid << "," << threadid
              << "], this is src"
              << endl;
#endif

            for (i=0; i<numSendReqs; i++) {
                (req.m_sendRequest[i]).start();
#ifdef _DEBUG_2
                cout << "Route[" << pvlProcess.getProcId() << "] iStaticSend() "
                     << "started persist send"<< endl;
#endif
            }//endFor each send to make

            if (numSendReqs)
                req.setNotDone();// mark the SendRequest as NOT-Done

      }//endIf this is a Src

    return;
}//end iStaticSend()


//------------------------------------------------------------------------
//  Method: getSegSize()
//
//  Description: sets up an NTuple with the start of a segment
//
//  Inputs: NTuple representing which segment
//          vector of arrays to pointers to Falls objects, which hold
//            the intersection falls.
//
//  Return: an integer representing the # of elements in the segment
//
//------------------------------------------------------------------------
int Route::getSegSize(NTuple        &segTup,
                      vector<Falls> *interFalls[])
{
    int   size=0;
    int   i;
    int   numDims=segTup.getDims();
    int   numFalls=interFalls[numDims-1]->size();
    int   segCount=0;

    for (i=0; i<numFalls; i++) {
        segCount += (*(interFalls[numDims-1]))[i].getN();
        if (segCount >= segTup[numDims-1])
        {
            size = (*(interFalls[numDims-1]))[i].getR()  -
                (*(interFalls[numDims-1]))[i].getL()  + 1;
        }
    }//endFor each falls at fastest dim

#ifdef PVL_DEVELOP
    if (size < 1)
    {
        cerr << "Rte[" << pvlProcess.getProcId()
             << "], getSegSize(): FAILED!!" << endl << endl;

        throw Exception("PvlRoute: getSegSize() FAILED!",
                        __FILE__, __LINE__);
    }
#endif
    return(size);
}//end getSegSize()


//------------------------------------------------------------------------
//  Method: getSegStart()
//
//  Description: sets up an NTuple with the start of a segment
//
//  Inputs: NTuple representing which segment
//          vector of arrays to pointers to Falls objects, which hold
//            the intersection falls.
//
//  Return: an NTuple is filled in with the coordinates of the start of
//         the segment.
//
//   NOTE: This routine the dimensions have been arranged in its args
//       such that the rightmost dim is the one varying fastest.
//           i.e. Row major ordering!
//------------------------------------------------------------------------
void Route::getSegStart(NTuple        &segTup,
                        vector<Falls> *interFalls[],
                        NTuple        &segStart)
{
    int   j, numDims=segTup.getDims();
    int   fastestDim = numDims-1;// We are ASSUMING row-major!
    int   numFalls;
    int   prevSegsForDim;
    int   segIndex;
    int   segLen;
    int   offsetIntoSeg;
    unsigned int   theLastFallsSeg;
    unsigned int   dimSeg;

#ifdef _DEBUG_2
    cout << "Rte[" << pvlProcess.getProcId() << "] gSS: "
         << "input segTup=" << segTup << endl;
#endif
    for (int dim=0; dim<numDims; dim++) {
        vector<Falls> &ifv = *(interFalls[dim]);
        numFalls    = ifv.size();
        prevSegsForDim = 0;
        dimSeg = static_cast<unsigned int>(segTup[dim]);

        for (j=0; j<numFalls; j++) {
//        First determine which FALLS  we are on
//         Then determine which line segment in that falls
//        -----------------------------------------------------------*
#ifdef _DEBUG_2
            cout << "Rte[" << pvlProcess.getProcId() << "] gSS: "
                 << "dim=" << dim << " ifv[" << j << "] is " << ifv[j]
                 << endl;
#endif
            segLen = ifv[j].getR() - ifv[j].getL() + 1;
            segIndex = (dimSeg-prevSegsForDim) / segLen;

            if (dim == fastestDim)
            {             // Fastest Dimension
                if ((prevSegsForDim + ifv[j].getN()) <= dimSeg)
                {
                    prevSegsForDim += ifv[j].getN();
                }
                else
                {
                    segStart[dim] = ifv[j].getL() + (segIndex * ifv[j].getS());
                    break;//  Got it!
                }
            }
            else
            {             // NOT Fastest Dimension
                theLastFallsSeg = (static_cast<unsigned>(prevSegsForDim + (segLen * ifv[j].getN())));

                if (theLastFallsSeg > dimSeg)
                {//   the segment is in this falls
                    offsetIntoSeg = dimSeg % segLen;
                    segStart[dim] = ifv[j].getL()    +
                        offsetIntoSeg    +
                        (segIndex * ifv[j].getS());
                    break;//  Got it!
                }
                else
                {
                    prevSegsForDim += segLen * ifv[j].getN();
                }
            }//endIf Not fastest varying dim
        }//endFor each Falls
    }//endFor each dimension

#ifdef _DEBUG_2
    cout << "Rte[" << pvlProcess.getProcId() << "] gSS: "
         << "input segTup=" << segTup
         << " and output segStart=" << segStart << endl;
#endif

    return;
}//end getSegStart()

//------------------------------------------------------------------------
//  Method: numSrcs
//
//  Description: returns the number of sources for the local node.
//
//  Inputs: none
//
//  Return: int, number of sources
//
//------------------------------------------------------------------------
int Route::numSrcs(void) const
{
    int    num=0, i, j, rank;

    if (m_numLocXfers)
        num = 1;

    if (m_destIsLocal && (m_currDestXfer.recvInfo != NULL))
    {
        for (i=0; i<m_currDestXfer.numRecvs; i++) {
            if (m_currDestXfer.recvInfo[i].recvIsLocal)
                continue;

            rank = m_currDestXfer.recvInfo[i].srcRank;
            for (j=0; j<i; j++) {
                if (m_currDestXfer.recvInfo[j].srcRank == rank)
                    break; // we already saw this one.
            }//end j loop

            if (j >= i)
                ++num;
        }//end i loop
    }//endIf any srcs

    return(num);
}//end numSrcs()

//------------------------------------------------------------------------
//  Method: numDests
//
//  Description: returns the number of destinations for the local node.
//
//  Inputs: none
//
//  Return: int, number of destinations
//
//------------------------------------------------------------------------
int Route::numDests(void) const
{
    int    num=0, i, j, rank;

    if (m_numLocXfers)
        num = 1;

    if (m_srcIsLocal && (m_currSrcXfer.sendInfo != NULL))
    {
        for (i=0; i<m_currSrcXfer.numSends; i++) {
            if (m_currSrcXfer.sendInfo[i].sendIsLocal)
                continue;

            rank = m_currSrcXfer.sendInfo[i].destRank;
            for (j=0; j<i; j++) {
                if (m_currSrcXfer.sendInfo[j].destRank == rank)
                    break; // we already saw this one.
            }//end j loop

            if (j >= i)
                ++num;
        }//end i loop
    }//endIf any dests

    return(num);
}//end numDests()


//------------------------------------------------------------------------
//  Method: srcRanks
//
//  Description: provides list of ranks which are sources for local node
//
//  Inputs: int *, points to an array of ints to be filled in
//
//  Return: int, number of sources
//
//  NOTE!! : It is the user's responsibility that the array is big enough.
//
//------------------------------------------------------------------------
int Route::srcRanks(int *rankList) const
{
    int    num=0, i, j, rank;
    PvtolProgram   prog;
    TaskBase&      ct=prog.getCurrentTask();

    if (m_numLocXfers)
        rankList[num++] = ct.getGlobalThreadRank();

    if (m_destIsLocal && (m_currDestXfer.recvInfo != NULL))
    {
      for (i=0; i<m_currDestXfer.numRecvs; i++) {
	if (!m_currDestXfer.recvInfo[i].recvIsLocal)
	  {
            rank = m_currDestXfer.recvInfo[i].srcRank;
            for (j=0; j<i; j++) {
	      if ((m_currDestXfer.recvInfo[j].srcRank == rank)   &&
		  (!m_currDestXfer.recvInfo[j].recvIsLocal) )
		break; // we already saw this one.
            }//end j loop

            if (j >= i)
	      rankList[num++] = m_currDestXfer.recvInfo[i].srcRank;
	  } //endIf recv is non-local
      }//end i loop
    }//endIf any srcs

    return(num);
}//end srcRanks()


//------------------------------------------------------------------------
//  Method: srcRanks
//
//  Description: returns a list of the sources for the local node.
//
//  Inputs: int, Rank of the destination node whose sources will be
//                  determined
//          int *, points to an array of ints to be filled in
//
//  Return: int, number of sources
//
//  NOTE!! : It is the user's responsibility that the array is big enough.
//
//------------------------------------------------------------------------
int Route::srcRanks(int dest, int *rankList) const
{
    // Figure out the index of the dest in the map
    vector<int> destIndex;
#ifdef NOT_YET
    (m_destMap->getGrid()).findIndex(dest, destIndex);
#endif // NOT_YET

    // Build an index used to walk through all the src indices
    int numDims = m_srcPitfalls.size();
    scoped_array<int> srcNumP(new int[numDims]);
    int numNodes = 1;
    for (int i = 0; i < numDims; i++)
    {
        srcNumP[i] = m_srcPitfalls[i].getP();
        numNodes *= m_srcPitfalls[i].getP();
    }
    NTuple srcIndex(numDims, srcNumP.get());

    // For each possible src node, see if the src and dest FALLS
    // intersect for every dimension.  If so, then the node is really a src.
    int numSrcsForDest = 0;
    for (int i = 0; i < numNodes; i++)
    {
        bool isSrc = true;
        for (int dim = 0; dim < numDims; dim++)
        {
            if (isSrc &&
                !intersects(m_srcPitfalls[dim].getFalls(srcIndex[dim]),
                            m_destPitfalls[dim].getFalls(destIndex[dim])))
            {
                // This one is not a src.
                isSrc = false;
            }
        }

        // If the processor at this index is a src, figure out its rank in
        // the task, and store it in the user array.
        if (isSrc)
        {
#ifdef NOT_YET
            const Node& theNode = dynamic_cast<const Node&>
                (m_srcMap->getGrid().getGridElement(srcIndex.serialIndex()));
            rankList[numSrcsForDest] = m_commScopePtr->rank(theNode.getId());
#endif // NOT_YET
            ++numSrcsForDest;
        }
        ++srcIndex;
    }
    return numSrcsForDest;
}//end srcRanks()


//------------------------------------------------------------------------
//  Method: destRanks
//
//  Description: returns a list of the destinations for the local node.
//
//  Inputs: int *, points to an array of ints to be filled in
//
//  Return: int, number of destinations
//
//  NOTE!! : It is the user's responsibility that the array is big enough.
//
//------------------------------------------------------------------------
int Route::destRanks(int *rankList) const
{
    int    num=0, i, j, rank;
    PvtolProgram   prog;
    TaskBase&      ct=prog.getCurrentTask();

    if (m_numLocXfers)
        rankList[num++] = ct.getGlobalThreadRank();

    if (m_srcIsLocal && (m_currSrcXfer.sendInfo != NULL))
    {
        for (i=0; i<m_currSrcXfer.numSends; i++) {
	  if (!m_currSrcXfer.sendInfo[i].sendIsLocal)
	    {
	      rank = m_currSrcXfer.sendInfo[i].destRank;
	      for (j=0; j<i; j++) {
                if ((m_currSrcXfer.sendInfo[j].destRank == rank)  &&
                    (!m_currSrcXfer.sendInfo[j].sendIsLocal) )
		  break; // we already saw this one.
	      }//end j loop

	      if (j >= i)
                rankList[num++] = m_currSrcXfer.sendInfo[i].destRank;
	    }//endIf send is not local
        }//end i loop
    }//endIf any dests

    return(num);
}//end destRanks()

//------------------------------------------------------------------------
//  Method: getTag()
//
//  Description: returns internal tag used by the Route
//
//  Returns: the tag
//
//  Note: this is an internal method to be used only by PVL objects
//  (e.g. conduit) that need to link multiple Transfers/Routes on the
//  same pipe.  This requires exact knowledge of the order of sends.
//------------------------------------------------------------------------

int Route::getTag() const
  {
    return m_tag;
  } // end getTag()


//------------------------------------------------------------------------
//  Method: setTag()
//
//  Description: changes internal tag used by the Route
//
//  Inputs: int, the replacement tag
//
//  Returns: nothing
//
//  Note: this is an internal method to be used only by PVL objects
//  (e.g. conduit) that need to link multiple Transfers/Routes on the
//  same pipe.  This requires exact knowledge of the order of sends.
//
//  Because this affects the allocation of tags, it must
//  be done in a SPMD way across the task in which the route
//  was created.
//------------------------------------------------------------------------

void Route::setTag(int tag)
  {
    // return the old tag
    m_commScopePtr->returnTag(m_tag);

    // and keep this one
    m_tag = tag;
  } // end setTag()


//------------------------------------------------------------------------
//  Method: sortStrideIdx()
//
//  Description: Sorts an array of strides by building an array of
//                sorted indices.
//
//  Inputs: pointer to list of integer strides
//          length of stride list
//          pointer to integer list where sorted indices will be stored
//
//  Return: the idxArr has the set of sorted indices
//
//------------------------------------------------------------------------
void Route::sortStrideIdx(const int *strides, int len, int *idxArr) const
{
    bool  *used = new bool[len];
    int   i, j;

    for (i=0; i<len; i++)
        used[i] = false;

    int maxStr, maxStrIdx;
    for (i=0; i<len; i++) {
        for (maxStrIdx=-1, maxStr=0, j=0; j<len; j++) {
            if (!used[j] && (strides[j] > maxStr))
            {
                maxStrIdx = j;
                maxStr = strides[j];
            }
        }//endFor each stride

        idxArr[i] = maxStrIdx;
        used[maxStrIdx] = true;
    }//endFor each elt in sorted index array

    delete[] used;
    return;
}//end sortStrideIdx()


//------------------------------------------------------------------------
//  Method: destructor
//
//  Description: destroy the Route obj.
//
//  Inputs: none
//
//  Return: none
//
//------------------------------------------------------------------------
Route::~Route() throw()
{
    int    numElts;
    unsigned int i;

#ifdef _DEBUG_3
    cout << "Route[" << pvlProcess.getProcId() << "], entering Route destruct"
         << endl;
#endif
    numElts = m_sendReqs.size();

    if (numElts)
    {
        list<SendRequest *>::iterator  iter=m_sendReqs.begin();
        for (i=0; i<m_sendReqs.size(); i++, iter++) {
            (*iter)->disassociate(*this);
        }

        m_sendReqs.clear();
    }//endIf any associated send-reqs

    if (m_currSrcXfer.sendReq != NULL)
            delete[] m_currSrcXfer.sendReq;

    if (m_currSrcXfer.sendInfo != NULL)
            delete[] m_currSrcXfer.sendInfo;

    if (m_currDestXfer.recvReq != NULL)
            delete[] m_currDestXfer.recvReq;

    if (m_currDestXfer.recvInfo != NULL)
            delete[] m_currDestXfer.recvInfo;

    if (m_locXferInfo != NULL)
        delete[] m_locXferInfo;

    if (m_srcMap)
        delete m_srcMap;

    if (m_destMap)
        delete m_destMap;

    // return the communications tag
    PvtolProgram   prog;
    TaskBase&      ct=prog.getCurrentTask();
    CommScope&     cs=ct.getCommScope();
    cs.returnTag(m_tag);

#ifdef _DEBUG_3
    cout << "Route[" << pvlProcess.getProcId() << "], leaving Route destruct"
         << endl;
#endif
    return;
}


//------------------------------------------------------------------------
//  Method: transposeLengths
//
//  Description:
//     When the TRANSPOSE_102 flag is set this method will reorder
//     the inputted unsigned int array.
//
//  Inputs:
//
//     lengths      integer array to modify if necessary.
//
//     flags        Route flags that are used in the Route constructor.
//
//------------------------------------------------------------------------
void Route::transposeLengths( unsigned int* lengths, Flags flags )
{
    if ( flags & TRANSPOSE_102 )
    {
        unsigned int newRow = lengths[0];
        lengths[0] = lengths[1];
        lengths[1] = newRow;
    }
    return;
}


}//end namespace

