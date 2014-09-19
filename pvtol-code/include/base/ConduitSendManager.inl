/**
 * File: ConduitSendManager.inl
 *
 *    March 2008 - ported from PVL by Jim Daly
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-24 16:28:21 -0500 (Tue, 24 Feb 2009) $
 *  \version $LastChangedRevision: 945 $
 *  \brief   Non-inline methods of Conduit SendManager
 *
 *  $Id: ConduitSendManager.inl 945 2009-02-24 21:28:21Z jav5880 $
 *
 * Author: Glenn Schrader
 */

#ifndef PVTOL_ConduitSendManager_cc
#define PVTOL_ConduitSendManager_cc

#include <Exception.h>
#include <ConduitSendManager.h>

namespace ipvtol
{

/////////////////////////////////////////////////////////////////////////////
// SendManager methods
/////////////////////////////////////////////////////////////////////////////

template <class SENDERTYPE>
SendManager<SENDERTYPE>::SendManager(
                        typename SENDERTYPE::SenderConfig & senderConfig)
        : SENDERTYPE(senderConfig),
          m_srcLocal(this->isSrcLocal()),
          m_dstLocal(this->isDstLocal()),
          m_state(IDLE),
          m_releaseCount(0)

{
}

template <class SENDERTYPE>
SendManager<SENDERTYPE>::~SendManager()
{
  if (m_state == IN_PROGRESS)
    {
      if (!this->testCommComplete())
	{
	  this->cancelComm();
	}
    }
}

template <class SENDERTYPE>
void SendManager<SENDERTYPE>::send()
{
#ifdef PVTOL_DEVELOP
    if (!m_srcLocal)
    {
        throw Exception("SendManager: send when src not local",
                        __FILE__, __LINE__);
    }

    if (m_state != IDLE && m_state != WAITING_FOR_SEND)
    {
        throw Exception("SendManager: send not appropriate for state",
                        __FILE__, __LINE__);
    }

#endif // PVTOL_DEVELOP

    if (!m_dstLocal)
    {
        // source but not destination
        this->beginComm();
        m_state = IN_PROGRESS;
    }
    else
    {
        // source and destination
        if (m_state == WAITING_FOR_SEND)
        {
            this->beginComm();
            m_state = IN_PROGRESS;
        }
        else
        {
            m_state = WAITING_FOR_RECV;
        }
    }

    return;
}//end send()

template <class SENDERTYPE>
void SendManager<SENDERTYPE>::recv()
{
#ifdef PVTOL_DEVELOP
    if (!m_dstLocal)
    {
        throw Exception("SendManager: recv when dst not local",
                        __FILE__, __LINE__);
    }

    if (m_state != IDLE && m_state != WAITING_FOR_RECV)
    {
        throw Exception("SendManager: recv not appropriate for state",
                        __FILE__, __LINE__);
    }

#endif // PVTOL_DEVELOP

    if (!m_srcLocal)
    {
        // destination but not source
        this->beginComm();
        m_state = IN_PROGRESS;
    }
    else
    {
        // destination and source
        if (m_state == WAITING_FOR_RECV)
        {
            this->beginComm();
            m_state = IN_PROGRESS;
        }
        else
        {
            m_state = WAITING_FOR_SEND;
        }
    }
}//end recv()


template <class SENDERTYPE>
bool SendManager<SENDERTYPE>::testComplete()
{
    switch (m_state)
    {
        case COMPLETE:
            return true;
            break;

        case IN_PROGRESS:

            if (this->testCommComplete())
            {
                m_state = COMPLETE;
                return true;
            }
            else
            {
                return false;
            }

            break;

        default:
            return false;
            break;
   }
}

template <class SENDERTYPE>
void SendManager<SENDERTYPE>::release()
{
#ifdef PVTOL_DEVELOP
    if (m_state != COMPLETE)
    {
        throw Exception("SendManager: release when not in COMPLETE state",
                        __FILE__, __LINE__);
    }
#endif // PVTOL_DEVELOP

    if (m_srcLocal && m_dstLocal)
    {
        // if we're both a source and a destination, wait for two releases
        // (one from the source buffer, one from the destination buffer)
        m_releaseCount++;
        if (m_releaseCount == 2)
        {
            m_releaseCount = 0;
            m_state = IDLE;
        }
    }
    else
    {
        // we're either a source or a destination, so we don't need to
        // count releases; one is enough
        m_state = IDLE;
    }

}//end release()



}//end namespace

#endif // PVTOL_ConduitSendManager_cc

