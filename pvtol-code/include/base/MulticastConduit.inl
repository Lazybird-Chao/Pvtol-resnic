/**
 * File: MulticastConduit.inl 
 *    January 2009 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Methods of the MulticastConduit, McInsertIf & McExtractIf
 *             The MulticastConduit class provides a means to multicast
 *             non-distributed data objects without pointers or virtual
 *             methods from a node in a source task to one or more nodes in
 *             a destination task.
 *
 *     See Also: MulticastConduit.h
 *
 * Author: Eddie Rutledge - written in Jan 2000
 *
 *  $Id: MulticastConduit.inl 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_MulticastConduit_inl
#define PVTOL_MulticastConduit_inl

#include <PvtolBasics.h>
#include <PvtolProgram.h>
#include <MulticastConduit.h>

namespace ipvtol
{
  //------------------------------------------------------------------

  template<class DATATYPE>
  MulticastConduit<DATATYPE>::MulticastConduit(const string& name) 
    : m_internalCond(name)
  {
  }

  template<class DATATYPE>
  MulticastConduit<DATATYPE>::~MulticastConduit() throw()
  {
  }

  template<class DATATYPE>
  McInsertIf<DATATYPE> MulticastConduit<DATATYPE>::getInsertIf(void)
  {
    McInsertIf<DATATYPE> iif(*this);
    return iif;
  }

  template<class DATATYPE>
  void MulticastConduit<DATATYPE>::setupSrc(const Map& srcMap, int depth)
  {
    unsigned int len[] = {0};
    if (srcMap.getNumRanks() != 1)
      {
          throw Exception("McCdt: src must be a single rank.",
	                  __FILE__, __LINE__);
      }

    DataMap *themap = makeDataMap(srcMap);
    m_internalCond.getInsertIf().setup(*themap, depth, len);
    delete themap;

    return;
  }

  //-------------------------------------------------------
  // destination end setup methods
  //-------------------------------------------------------

  template <class DATATYPE>
  McExtractIf<DATATYPE> 
  MulticastConduit<DATATYPE>::getExtractIf(void)
  {
    McExtractIf<DATATYPE> eif(*this);
    return eif;
  };

  template<class DATATYPE>
  void MulticastConduit<DATATYPE>::setupDest(const Map& dstMap, int depth)
  {
    unsigned int len[] = {0};
    DataMap *themap = makeDataMap(dstMap);
    m_internalCond.getExtractIf().setup(*themap, depth, len);
    delete themap;

    return;
  }

  //-------------------------------------------------------

  template<class DATATYPE>
  void MulticastConduit<DATATYPE>::setupComplete(void)
  {
    m_internalCond.setupComplete();
  }

  //------------------------------------------------------------------

  template <class DATATYPE>
  bool MulticastConduit<DATATYPE>::insertAvailable()
  {
    return m_internalCond.getInsertIf().available();
  }

  template <class DATATYPE>
  DATATYPE& MulticastConduit<DATATYPE>::getInsertHandle()
  {
    return m_internalCond.getInsertIf().getTagHandle();
  }

  template <class DATATYPE>
  void MulticastConduit<DATATYPE>::insert()
  {
    // Vector=scalar assignment
    m_internalCond.getInsertIf().insert();
  }

  //------------------------------------------------------------------

  template <class DATATYPE>
  bool MulticastConduit<DATATYPE>::extractReady()
  {
    return m_internalCond.getExtractIf().ready();
  }

  template <class DATATYPE>
  DATATYPE& MulticastConduit<DATATYPE>::getExtractHandle()
  {
    return m_internalCond.getExtractIf().getTagHandle();
  }

  template <class DATATYPE>
  void MulticastConduit<DATATYPE>::release()
  {
    m_internalCond.getExtractIf().release();
  }

  //------------------------------------------------------------------
  template <class DATATYPE>
  DataMap* MulticastConduit<DATATYPE>::makeDataMap(const Map& theMap)
  {
     int   i, j;
     PvtolProgram  prog;
     TaskBase &ct = prog.getCurrentTask();
     TaskMap& tm  = ct.getMap();
     int  numRanks = theMap.getNumRanks();
     Grid grid(numRanks);
     vector<int>  inmapprocs;
     mapToProcList(theMap, inmapprocs);
     vector<int>  taskprocs;
     mapToProcList(tm, taskprocs);
     vector<int>  ranks;

     for (i=0; i<inmapprocs.size(); i++) {
        int mapproc = inmapprocs[i];
        for (j=0; j<taskprocs.size(); j++) {
	    if (mapproc == taskprocs[j])
	                break;
        }//endFor j

	ranks.push_back(j);
     }//endFor i

     RankList rl(ranks);

     DataDistDescription distDescription(BlockDist(0), BlockDist(0));
     DataMap*  dm = new DataMap(DataMap::RUNTIME_MAP, rl,
                                grid, distDescription);

    return(dm);
  }//end makeDataMap()


  template <class DATATYPE>
void MulticastConduit<DATATYPE>::mapToProcList(const Map& map,
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
    int         rank, num;

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

           num = oldlist.size();
           for (i=0; i<num; i++) {
             rank = oldlist[i];
             for (j=0; j<i; j++) {
               if (rank == oldlist[j])
                       break;
             }//endFor j

             if (i == j)
                  newlist.push_back(rank);
                  //  newlist.push_back(tsklist[rank]);
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


}

#endif //  PVTOL_MulticastConduit_inl

