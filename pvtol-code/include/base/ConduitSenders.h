/**
 *    File: ConduitSenders.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-19 13:45:17 -0500 (Thu, 19 Feb 2009) $
 *  \version $LastChangedRevision: 939 $
 *  \brief   Class definition of DataTagSender & EocSender classes
 *
 *  $Id: ConduitSenders.h 939 2009-02-19 18:45:17Z jav5880 $
 *
 * Author: Glenn Schrader
 *
 */
#ifndef PVTOL_CONDUITSENDERS_H
#define PVTOL_CONDUITSENDERS_H

#include <PvtolBasics.h>
#include <SendRequest.h>
#include <ConduitConnection.h>

#include <vector>
#include <string>
#include <typeinfo>

namespace ipvtol
{
using std::string;
using std::vector;



/////////////////////////////////////////////////////////////////////////////
// class DataTagSender
/////////////////////////////////////////////////////////////////////////////

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
class DataTagSender
{
    public:

    typedef Connection<DATATYPE, TAGTYPE, USE_EOC> ConnectionX;
    typedef ItemTransferInfo TagTransferInfo;
    typedef ItemTransferInfo SeqNumTransferInfo;
    typedef ConnectionX SenderConfig;

    DataTagSender(ConnectionX & connection);
    ~DataTagSender();

    inline bool isSrcLocal();
    inline bool isDstLocal();

           void beginComm();
           bool testCommComplete();

    inline void setBufferIndices(int srcBufferIndex,
                                 int dstBufferIndex);

    // These methods used by Connection to do bug-checking.  They should
    // not be used for any other purpose.
    inline int getSrcBufferIndex();
    inline int getDstBufferIndex();

    // These methods make it easier for the Endpoint to know which sequence
    // numbers to check
    inline const ConnectionX & getConnection();
    inline void waitCommComplete();
    void waitForTag();

    protected:
       void cancelComm();

    private:

    ConnectionX &            m_connection;
    int                     m_srcBufferIndex;
    int                     m_dstBufferIndex;

    SendRequest             m_sendRequest;
    bool                    m_ddoRequestDone;

    vector<TagTransferInfo> m_tagInfo;
    bool                    m_tagRequestDone;
    unsigned int            m_tagRequestCheckIndex;
};




/////////////////////////////////////////////////////////////////////////////
// class EocSender
/////////////////////////////////////////////////////////////////////////////

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
class EocSender
{
  public:
    typedef Connection<DATATYPE, TAGTYPE, USE_EOC> ConnectionX;
    typedef ItemTransferInfo EocTransferInfo;
    typedef ConnectionX SenderConfig;

    EocSender(ConnectionX & connection);
    ~EocSender();

    inline bool isSrcLocal();
    inline bool isDstLocal();

           void beginComm();
           bool testCommComplete();

    inline void setBufferIndices(int srcBufferIndex,
                                 int dstBufferIndex);

    // These methods used by Connection to do bug-checking.  They should
    // not be used for any other purpose.
    inline int getSrcBufferIndex();
    inline int getDstBufferIndex();

  protected:
    inline void cancelComm();

  private:
    ConnectionX &            m_connection;
    int                     m_srcBufferIndex;
    int                     m_dstBufferIndex;
    vector<EocTransferInfo> m_eocInfo;
    unsigned int            m_eocRequestCheckIndex;
};//end class EocSender


//       I N L I N E     Methods
//------------------------------------------

//                     DataTagSender
//==================================================
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::isSrcLocal()
{
    return m_connection.srcLocal();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::isDstLocal()
{
    return m_connection.dstLocal();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::waitCommComplete()
{
   m_sendRequest.wait();

   return;
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::setBufferIndices(
                                    int srcBufferIndex,
                                    int dstBufferIndex)
{
    m_srcBufferIndex = srcBufferIndex;
    m_dstBufferIndex = dstBufferIndex;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::getSrcBufferIndex()
{
    return m_srcBufferIndex;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::getDstBufferIndex()
{
    return m_dstBufferIndex;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline const Connection<DATATYPE, TAGTYPE, USE_EOC> &
DataTagSender<DATATYPE, TAGTYPE, USE_EOC>::getConnection()
{
    return m_connection;
}


//                     EocSender
//==================================================
template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool EocSender<DATATYPE, TAGTYPE, USE_EOC>::isSrcLocal()
{
    return m_connection.srcLocal();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline bool EocSender<DATATYPE, TAGTYPE, USE_EOC>::isDstLocal()
{
    return m_connection.dstLocal();
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void EocSender<DATATYPE, TAGTYPE, USE_EOC>::cancelComm()
{
    for (unsigned int i = 0; i < m_eocInfo.size(); i++)
    {
        m_eocInfo[i].m_sendRequest->cancel();
    }
}


template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline void EocSender<DATATYPE, TAGTYPE, USE_EOC>::setBufferIndices(
                                    int srcBufferIndex,
                                    int dstBufferIndex)
{
    m_srcBufferIndex = srcBufferIndex;
    m_dstBufferIndex = dstBufferIndex;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int EocSender<DATATYPE, TAGTYPE, USE_EOC>::getSrcBufferIndex()
{
    return m_srcBufferIndex;
}

template <class DATATYPE, class TAGTYPE, bool USE_EOC>
inline int EocSender<DATATYPE, TAGTYPE, USE_EOC>::getDstBufferIndex()
{
    return m_dstBufferIndex;
}



}//end namespace

#include <ConduitSenders.inl>

#endif // PVTOL_CONDUITSENDERS_H

