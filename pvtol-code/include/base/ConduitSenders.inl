/**
 * File: ConduitSenders.inl
 *
 *    March 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-19 13:45:17 -0500 (Thu, 19 Feb 2009) $
 *  \version $LastChangedRevision: 939 $
 *  \brief   Non-inline methods of Conduit DataTagSender and EocSender
 *
 *  $Id: ConduitSenders.inl 939 2009-02-19 18:45:17Z jav5880 $
 *
 * Author: Glenn Schrader
 */
#ifndef PVTOL_ConduitSenders_cc
#define PVTOL_ConduitSenders_cc

#include <PvtolBasics.h>
#include <PvtolProgram.h>
#include <SendRequest.h>
#include <Transfer.h>
#include <ConduitSenders.h>

#include <stdio.h>
#include <utility>

#define PVTOL_DEBUG_1

namespace ipvtol
{

using namespace std;

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::DataTagSender(
                                                SenderConfig & connection) :
            m_connection(connection),
            m_tagRequestDone(false),
            m_ddoRequestDone(false)
{
    if (connection.getDataSize() > 0)
            m_sendRequest.setup(connection.getRoute());
  
    m_tagRequestCheckIndex = 0;

    const vector< pair<int, int> > & tagTransferNodeList =
            connection.getTagTransferNodeList();

    const vector< shared_ptr<Transfer > > & tagTransfers =
            connection.getTagTransfers();

    PvtolProgram   prog;
    ProcId myProcId = prog.getProcId();

    for (unsigned i = 0; i < tagTransfers.size(); i++)
    {
#ifdef PVTOL_DEBUG_2
	 cout << "DTS[" << myProcId << "]:"
	      << " construct in tagTransfers loop."
	      << endl;
#endif // PVTOL_DEBUG_2
        // if transfer i is local to us in any way, save a structure
        // describing it and build a corresponding SendRequest
        if (myProcId == tagTransferNodeList[i].first ||
            myProcId == tagTransferNodeList[i].second)
        {
#ifdef PVTOL_DEBUG_2
	 cout << "DTS[" << myProcId << "]:"
	      << " construct in tagTransferNodeList match procid."
	      << endl;
#endif // PVTOL_DEBUG_2
            TagTransferInfo currentTagTransfer;
            currentTagTransfer.m_index = i;
            currentTagTransfer.m_fromNode = tagTransferNodeList[i].first;
            currentTagTransfer.m_toNode = tagTransferNodeList[i].second;
            currentTagTransfer.m_sendRequest.reset(
                new SendRequest(*tagTransfers[i]) );
            m_tagInfo.push_back(currentTagTransfer);
        }
    }

    return;
}//end Constructor()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::~DataTagSender()
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::beginComm()
{
    // send DDO
    if (m_connection.getDataSize() > 0)
      {
         m_connection.send(m_sendRequest, m_srcBufferIndex, m_dstBufferIndex);
         m_ddoRequestDone = false;
      }
     else if (m_tagInfo.size() == 0)
      {
         m_ddoRequestDone = true;
	 PvtolProgram prog;
	 int myId = prog.getProcId();
#ifdef PVTOL_DEBUG_1
	 cout << endl << endl << "DTS[" << myId << "]:"
	      << " taginfo size 0 set m_ddoRequestDone to true."
	      << endl << endl;
#endif // PVTOL_DEBUG_1
      }

    // send tags if any
    for (unsigned i = 0; i < m_tagInfo.size(); i++)
    {
        m_connection.sendTag(m_tagInfo[i].m_index,
                             *m_tagInfo[i].m_sendRequest,
                             m_srcBufferIndex,
                             m_dstBufferIndex);
        m_tagRequestDone = false;
    }
    m_tagRequestCheckIndex = 0;

    return;
}//end beginComm()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
bool DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::testCommComplete()
{
    if (!m_ddoRequestDone)
    {
        // try to finish sending the DDO
        m_ddoRequestDone = m_sendRequest.test();
        if (!m_ddoRequestDone)
        {
            // can't finish it now
            return false;
        }
    }
                
    // we've finished the DDO; now try the tags
    while (m_tagRequestCheckIndex < m_tagInfo.size()) {
        if (m_tagInfo[m_tagRequestCheckIndex].m_sendRequest->test())
          {
            // finished this tag
            m_tagRequestCheckIndex++;
          }
         else
          {
            // can't finish the tags
            return false;
          }
    }//endWhile

    // finished everything
    m_tagRequestDone = true;

    return true;
}//end testCommComplete()

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::waitForTag()
{
    if (!m_tagRequestDone)
      {
        for (int i=0; i < m_tagInfo.size(); i++) {
               m_tagInfo[i].m_sendRequest->wait();
        }//endFor

        m_tagRequestDone = true;
      }

    return;
}//end testCommComplete()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::cancelComm()
{
    if (!m_ddoRequestDone)
    {
        // cancel sending the DDO
        m_sendRequest.cancel();
    }
                
    // we've canceled the DDO; now do the tags
    for (unsigned int i = 0; i < m_tagInfo.size(); i++)
    {
        m_tagInfo[i].m_sendRequest->cancel();
    }

}//end cancelComm()
   

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
EocSender<DATATYPE, TAGTYPE, USE_EOC>::EocSender(SenderConfig & connection) :
            m_connection(connection)
{
    const vector< pair<int, int> > & eocTransferNodeList =
            connection.getEocTransferNodeList();

    const vector< shared_ptr<Transfer > > & eocTransfers =
            connection.getEocTransfers();

    PvtolProgram   prog;
    ProcId myProcId = prog.getProcId();

    for (unsigned i = 0; i < eocTransfers.size(); i++)
    {
        // if transfer i is local to us in any way, save a structure
        // describing it and build a corresponding SendRequest
        if (myProcId == eocTransferNodeList[i].first ||
            myProcId == eocTransferNodeList[i].second)
        {
            EocTransferInfo currentEocTransfer;
            currentEocTransfer.m_index = i;
            currentEocTransfer.m_fromNode = eocTransferNodeList[i].first;
            currentEocTransfer.m_toNode = eocTransferNodeList[i].second;
            currentEocTransfer.m_sendRequest.reset(
                new SendRequest(*eocTransfers[i]) );
            m_eocInfo.push_back(currentEocTransfer);
        }
    }
}//end EocSender() construct

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
EocSender<DATATYPE, TAGTYPE, USE_EOC>::~EocSender()
{
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
void EocSender<DATATYPE, TAGTYPE, USE_EOC>::beginComm()
{
    for (unsigned i = 0; i < m_eocInfo.size(); i++)
    {
        m_connection.sendEoc(m_eocInfo[i].m_index,
                             *m_eocInfo[i].m_sendRequest,
                             m_srcBufferIndex,
                             m_dstBufferIndex);
    }
    m_eocRequestCheckIndex = 0;
}//end EocSender beginComm()


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
bool EocSender<DATATYPE, TAGTYPE, USE_EOC>::testCommComplete()
{
    while (m_eocRequestCheckIndex < m_eocInfo.size()) {
        if (m_eocInfo[m_eocRequestCheckIndex].m_sendRequest->test())
          { // finished this eoc
            m_eocRequestCheckIndex++;
          }
        else
          { // can't finish
            return false;
          }
    }//endWhile

    return true; // finished all EOC transfers
}//end EocSender testCommComplete()
   
}  //end namespace

#endif // PVTOL_ConduitSenders_cc
