/**
 *   File: PvtolRequest.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   Non-inline methods of the PvtolRequest & Copy Request classes
 *            A PvtolRequest object is used to test for the completion of
 *	      a non-Blocking send or receive from a CommScope
 *	      It may also be used to wait for the completion of a
 *            non-Blocking send or receive.
 *
 *    CopyRequest, an alternate version of PvtolRequest used internally in
 *         pvtol development to manage communications that involved only a
 *         memcpy from source to dest
 *
 *    $Id: PvtolRequest.cc 938 2009-02-18 17:39:52Z ka21088 $
 ***************************************************************************/
#include <PvtolRequest.h>

namespace ipvtol
{

/**------------------------------------------------------------------------
 *    CopyRequest::wait()
 *	   @param CopyStatus
 *         @return void
 *------------------------------------------------------------------------*/
void CopyRequest::wait(CopyStatus& status)
{
  status.m_isSrc = m_isSrc;
  status.m_error = 0;
  if (!m_isSrc)
  {
    // This information can only be queried from the dest end
    status.m_source = m_srcRank;
    status.m_tag = m_tag;
    status.m_count = m_numBytes;
   }
}//end wait()

/**------------------------------------------------------------------------
 *    CopyRequest::test()
 *	   @param int * flagPtr
 *	   @param CopyStatus
 *         @return void
 *------------------------------------------------------------------------*/
void CopyRequest::test(int* flagPtr, CopyStatus& status)
{
  status.m_isSrc = m_isSrc;
  status.m_error = 0;
  if (!m_isSrc)
  {
    // This information can only be queried from the dest end
    status.m_source = m_srcRank;
    status.m_tag = m_tag;
    status.m_count = m_numBytes;
  }
  *flagPtr = 1;
}//end test()

}; //end namespace ipvtol

