/**
 * File: RouteTmpl.inl
 *    May 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Non-inline methods of the Route class which are Templated.
 *    A Route object provides the means of doing a real time data re-org.
 *    It is capable of redistributing a data object which is distributed
 *    accross one set of nodes to one distributed on another set of nodes.
 *    Both the starting and ending distributions must be described via
 *    PVTOL Maps.
 *
 *  See Also: Route.cc which contains all the non-templated Route code
 *
 *  $Id: RouteTmpl.inl 945 2009-02-24 21:28:21Z jav5880 $
 *
 *   Author - Jim Daly June 2000
 */
#ifndef _RouteTmpl_CC
#define _RouteTmpl_CC

#include <PvtolProgram.h>
#include <Route.h>
#include <unistd.h>


namespace ipvtol
{
    using std::string;

//------------------------------------------------------------------------
//  Method:     Constructor
//      Route(DistributedViewBase<T> &srcView,
//            DistributedViewBase<T> &destView,
//        RouteDirective rd=DEFAULT_DIR);
//
//  Description: Constructor
//
//  Inputs:
//
//------------------------------------------------------------------------
//  template<dimension_type N, typename T, class Block, class Map>
//Route<N, T, Block, Map>::Route(const HierArray<N, T, Block, Map& > srcArr,
//                               const HierArray<N, T, Block, Map& > destArr,
//                               Flags flags=DEFAULT_FLAG)
#ifdef INCLUDE_TEMPLATE_MAPS
  template<dimension_type N, typename T, class Block1, class Map1>
                                      //   class Block2, class Map2>
Route::Route(const HierArray<N, T, Block1, Map1& >* srcArr,
             const HierArray<N, T, Block1, Map1& >* destArr,
             Flags flags) :
#else // do not INCLUDE_TEMPLATE_MAPS
  template<dimension_type N, typename T, class Block1>
Route::Route(const HierArray<N, T, Block1 >* srcArr,
             const HierArray<N, T, Block1 >* destArr,
	     const DataMap & srcMap,
	     const DataMap & destMap,
	     bool  srcIsLocal,
	     bool  destIsLocal,
	     const vector<int>& srcProcList,
	     const vector<int>& destProcList,
             Flags flags) :
#endif // INCLUDE_TEMPLATE_MAPS
    m_eltSize(sizeof(T)),
    m_isSrc(false),
    m_isDest(false),
    m_sendCount(0),
    m_recvdCount(0),   
    m_trait(NULL_TRAIT),
    m_locXferInfo(NULL),
    m_numLocXfers(0),
    m_srcPitfalls(N),
    m_destPitfalls(N),
    m_numDims(N),
    m_maxSends(0),
    m_maxRecvs(0)
{
    PvtolProgram   prog;
    TaskBase& ct   = prog.getCurrentTask();
    m_commScopePtr = &(ct.getCommScope());

#ifdef NOT_YET
    const Layout &srcL = srcArr->getLayout();
    const Layout &destL = destArr->getLayout();

    m_srcMap  = srcArr->getMap().clone();
    m_destMap = destArr->getMap().clone();
#else
    m_srcMap  = new DataMap(srcMap);
    m_destMap = new DataMap(destMap);
#endif // NOT_YET
    
    m_srcIsLocal = srcIsLocal;
    m_destIsLocal = destIsLocal;

    // allocate communications tag for this route -- must be SPMD
    if (!(TAG_WILL_BE_SUPPLIED & flags))
               m_tag = m_commScopePtr->getNextTag();
#ifdef PVTOL_DEVELOP
    if (m_numDims > 2)
      {
        cerr << "PvtolRoute: does NOT support more than 2 Dimensions, yet"
             << endl;

        throw NotImplementedYet(__FILE__, __LINE__);
      }
#endif // PVTOL_DEVELOP

    if (flags & STATIC)
    {
      if (srcArr != NULL)
        m_localSrcAddr  = (void *)srcArr->localPointer();
       else
        m_localSrcAddr  = NULL;

      if (destArr != NULL)
        m_localDestAddr = (void *)destArr->localPointer();
       else
        m_localDestAddr = NULL;
    }
    else
    {
      // m_localSrcAddr  = srcArr->getBlock().getLocalAddr();
      // m_localDestAddr = destArr->getBlock().getLocalAddr();
      //
      //  these addrs  must be adjusted to beginning of block
      if (srcArr != NULL)
        m_localSrcAddr  = (void *)srcArr->localPointer();
       else
        m_localSrcAddr  = NULL;

      if (destArr != NULL)
        m_localDestAddr = (void *)destArr->localPointer();
       else
        m_localDestAddr = NULL;
    }

    if (m_srcIsLocal && (m_localSrcAddr != NULL))
             m_isSrc = true;

    if (m_destIsLocal && (m_localDestAddr != NULL))
             m_isDest = true;

#ifdef NOT_YET
//   store the Pitfalls
//  ============================================================*
    for (int i = 0; i < m_numDims; i++)
    {
        m_srcPitfalls[i] = srcL.getDistributedLayout().getPitfalls()[i];
        m_destPitfalls[i] = destL.getDistributedLayout().getPitfalls()[i];
    }
#endif // NOT_YET

//   Setup the traits of this Route
//  ============================================================*
    if ((m_localSrcAddr == NULL) && (m_localDestAddr == NULL))
    {//         Nothing in Src or Dest is Local
        m_trait = NO_LOCAL_DATA_MOVEMENT;
    }
#ifdef NOT_YET
    else if (*m_srcMap == *m_destMap)
    {
        if ((flags & STATIC) && (m_localSrcAddr == m_localDestAddr))
        {//       do Nothing
            m_trait = NO_LOCAL_DATA_MOVEMENT;
        }
        else
        {//       memcpy(s) will suffice
            bool    allDataContiguous = true;

//         ------------------------------------------------------*
//         Some Current Assumptions: I know "when you ASSUME..."
//               at most 2 dimensions
//               Row Major data ordering
//         ------------------------------------------------------*
            if (m_numDims > 1)
            {
                int colStride = srcArr->stride(0);
                int lengthOfRow = srcArr->getLocalLength(1);
                
                if (lengthOfRow != colStride)
                    allDataContiguous = false;
                
                lengthOfRow = destArr->getLocalLength(1);
                colStride = destArr->stride(0);

                if (lengthOfRow != colStride)
                    allDataContiguous = false;
            }//endIf more than 1 dimension
            else if (m_numDims == 1)
            {
                if (srcArr->stride(0) != 1)
                    allDataContiguous = false;

                if (destArr->stride(0) != 1)
                    allDataContiguous = false;
            }

            if (allDataContiguous)
            {
                m_trait       = ALL_MOVEMENT_LOCAL_CONTIGUOUS;
                m_sendCount   = srcArr->endLocalEl() - srcArr->beginLocalEl();
                m_sendCount  *= m_eltSize; // convert to bytes
                m_numLocXfers = 1;
            }//endIf data contiguous
            else if (!allDataContiguous && m_numDims == 1)
            { // copying a strided vector element by element
                m_trait         = ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_COL;
                m_numLocXfers   = srcArr->getLocalLength(0);
		m_srcRowStride  = srcArr->stride(0);
		m_destRowStride = destArr->stride(0);
            }//endIf data is not contiguous and is a Vector
            else
	    { // setup to use memcpy for each row
                m_trait       = ALL_MOVEMENT_LOCAL_NOT_CONTIGUOUS_ROW;
                m_sendCount   = srcArr->getLocalLength(0) *
                                      srcArr->getLocalLength(1);
                m_numLocXfers = srcArr->getLocalLength(0);// 1 xfer for each row
                m_locXferInfo = new LocalXferInfo[m_numLocXfers];
               
		m_srcRowStride  = srcArr->stride(0);
		m_destRowStride = destArr->stride(0);
 
                const T *startSrcAddr  = static_cast<T*>(const_cast<void*>(m_localSrcAddr));
                T       *startDestAddr = static_cast<T*>(m_localDestAddr);

                for (int i=0; i<m_numLocXfers; i++) {
                    const T *sa = startSrcAddr + (i * m_srcRowStride);
                    T       *da = startDestAddr + (i * m_destRowStride);
                    
                    m_locXferInfo[i].srcAddr = static_cast<const void *>(sa);
                    m_locXferInfo[i].destAddr = static_cast<void *>(da);
                    m_locXferInfo[i].byteSize = m_eltSize *
                                               srcArr->getLocalLength(1);
                }//endFor each xfer
            }//endIf data is not contiguous and is not a Vector
        }//endIf all is local
    }
#endif // NOT_YET
    else
    {
//      ------------------------------------------------------------*
//      first check that this does not involve submatrices split
//        along the rows. This causes the local data to be non
//        contiguous - i.e. there is a local jump between rows" -
//        and our code does not take that into account.
//      ------------------------------------------------------------*
        if (m_numDims > 1)
        {
            bool allDataContiguous = true;

            int lengthOfRow;
            int colStride;

            if (m_isSrc)
            {
#ifdef NOT_YET
                lengthOfRow = srcArr->getLocalLength(1);
#else
                lengthOfRow = 0;
#endif // NOT_YET
                colStride = srcArr->stride(0);

                if (lengthOfRow != colStride)
                    allDataContiguous = false;
            }

            if (m_isDest)
            {
#ifdef NOT_YET
                lengthOfRow = destArr->getLocalLength(1);
#endif // NOT_YET
                colStride = destArr->stride(0);

                if (lengthOfRow != colStride)
                    allDataContiguous = false;
            }

        }//endIf more than 1 dimension
        if (flags & STATIC)
            m_trait = STATIC_ROUTE;
        else
            m_trait = NON_STATIC_ROUTE;

        //  Setup remote communications
        //|||||||||||||||||||||||||||||||||||||||||||||||||||||||
        if (m_isSrc)
          {
	    if ((m_srcMap->getNumRanks() == 1)
		             &&
	        (m_destMap->getNumRanks() == 1))
              {
	        simpleSendInfo(m_localSrcAddr,
	                       m_localDestAddr,
			       srcArr->size(),
			       destProcList[0]
		              );
	      }
#ifdef NOT_YET
             else
              {
                const void *blkAddrSrc = srcArr->getBlock().getLocalAddr();

                vector<NodeId> destNodes(destArr->getMap().getNodes());
                vector<void*> blkAddrsDest(destNodes.size());
                for (int i = 0; i < destNodes.size(); i++)
                  {   
                     blkAddrsDest[i] =
                           destArr->getBlock().getAddr(destNodes[i]);
                  }

                calcSendInfo(srcL, blkAddrSrc, blkAddrsDest, destL);
	      }
#endif // NOT_YET
          }//endIf a Src

        if (m_isDest)
          {
	    if ((m_srcMap->getNumRanks() == 1)
		             &&
	        (m_destMap->getNumRanks() == 1))
              {
	        simpleRecvInfo(m_localSrcAddr,
	                       m_localDestAddr,
			       destArr->size(),
			       srcProcList[0]
		              );
	      }
#ifdef NOT_YET
             else
              {
                void *blkAddrDest = destArr->getBlock().getLocalAddr();
 
                vector<NodeId> srcNodes(srcArr->getMap().getNodes());
                vector<const void*> blkAddrsSrc(srcNodes.size());
                for (int i = 0; i < srcNodes.size(); i++) {
                    blkAddrsSrc[i] = srcArr->getBlock().getAddr(srcNodes[i]);
                }

                calcRecvInfo(srcL, blkAddrsSrc, blkAddrDest, destL);
	      }
#endif // NOT_YET
          }//endIf a dest
    }//end setting up traits
    
    if ((m_srcMap->getNumRanks() > 1)
	             ||
        (m_destMap->getNumRanks() > 1))
       {
          procXferStructs();
       }
    
    return;
}//end construct

}//end namespace

#endif // _RouteTmpl_CC

