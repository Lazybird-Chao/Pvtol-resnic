/**
 *    File: XferDealer.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   This class deals out transfer Ids starting with 1
 *
 *  $Id: XferDealer.h 938 2009-02-18 17:39:52Z ka21088 $
 */
#ifndef XFERDEALER_H_
#define XFERDEALER_H_

#include <PvtolBasics.h>

namespace ipvtol {


class XferDealer
{
   public:
      XferDealer();
      int getDealtValue();

   private:
      int    m_index;
};

inline
XferDealer::XferDealer() : m_index(0)
  {return; }

inline
  int XferDealer::getDealtValue()
  {return(++m_index); }

}
#endif /*XFERDEALER_H_*/

