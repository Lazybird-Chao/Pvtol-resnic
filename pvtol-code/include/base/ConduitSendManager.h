/**
 *    File: ConduitSendManager.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Class definition of ConduitSendManager
 *
 *  $Id: ConduitSendManager.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 * Author: Glenn Schrader
 */

#ifndef PVTOL_CONDUITSENDMANAGER_H
#define PVTOL_CONDUITSENDMANAGER_H

#include <PvtolBasics.h>

#include <vector>
#include <string>
#include <typeinfo>


namespace ipvtol
{
using std::string;
using std::vector;


/////////////////////////////////////////////////////////////////////////////
// class SendManager
/////////////////////////////////////////////////////////////////////////////

template <class SENDERTYPE>
class SendManager : public SENDERTYPE
{
    public:

    enum SendState
    {
      IDLE,             // ptr to each of the slots in buffermanagement...
      WAITING_FOR_SEND, // Keeps track of the state of the slots in the buffer.
      WAITING_FOR_RECV, // There should be one ptr to each slot...
      IN_PROGRESS,      // 
      COMPLETE          //maa
    };

    SendManager(typename SENDERTYPE::SenderConfig & senderConfig);
    ~SendManager();

    void send();
    void recv();

    inline SendState getState();
    inline void setState(SendState);

    bool testComplete();

    void release();

    private:

    bool                m_srcLocal;
    bool                m_dstLocal;
    SendState           m_state;
    unsigned            m_releaseCount;//looks for 1 if slot is a snd or dst
                                       //  looks for 2 if the slot is both.

};


//           I N L I N E     Methods
//------------------------------------------------------
template <class SENDERTYPE>
inline typename SendManager<SENDERTYPE>::SendState
SendManager<SENDERTYPE>::getState()
{
    return m_state;
}


template <class SENDERTYPE>
inline void SendManager<SENDERTYPE>::setState(SendManager<SENDERTYPE>::SendState STATE)
{
  m_state = STATE;
} 


}//end namespace


#include <ConduitSendManager.inl> 


#endif // PVTOL_CONDUITSENDMANAGER_H


