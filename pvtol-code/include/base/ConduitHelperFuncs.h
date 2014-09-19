/**
 *    File: ConduitHelperFuncs.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *    March 2008: Ported from PVL by Jim Daly
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Method definitions of the Conduit Helper Functions
 *
 *  $Id: ConduitHelperFuncs.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOL_CONDUITHELPERFUNCS_H
#define PVTOL_CONDUITHELPERFUNCS_H

#include <PvtolBasics.h>
#include <string>
#include <typeinfo>

namespace ipvtol
{

/////////////////////////////////////////////////////////////////////////////
// class TagSeqNumWrapper
/////////////////////////////////////////////////////////////////////////////

template <class TAGTYPE, bool USE_EOC> class TagSeqNumWrapper;

template <class TAGTYPE>
class TagSeqNumWrapper<TAGTYPE, true>
{
    public:
       TAGTYPE    m_tagVal;
       SeqNum     m_seqNum;

       inline void setSeqNum(int newSeqNum);
       inline int  getSeqNum();
};


//           I N L I N E      Methods
//--------------------------------------------------
template <class TAGTYPE>
inline void TagSeqNumWrapper<TAGTYPE, true>::setSeqNum(int newSeqNum)
{
    m_seqNum = newSeqNum;
}

template <class TAGTYPE>
inline int TagSeqNumWrapper<TAGTYPE, true>::getSeqNum()
{
    return m_seqNum;
}

template <class TAGTYPE>
class TagSeqNumWrapper<TAGTYPE, false>
{
    public:

    TAGTYPE m_tagVal;

    inline void setSeqNum(int newSeqNum);
    inline int  getSeqNum();
};

template <class TAGTYPE>
inline void TagSeqNumWrapper<TAGTYPE, false>::setSeqNum(int newSeqNum)
{
    // Sequence number ignored in this case
}

template <class TAGTYPE>
inline int TagSeqNumWrapper<TAGTYPE, false>::getSeqNum()
{
    return 0;
}


}  //end namespace


#endif // PVTOL_CONDUITHELPERFUNCS_H

