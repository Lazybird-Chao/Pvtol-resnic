/**
 *  CdtLocalXferData.h
 *
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  Jim Daly, $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $	
 *  \version $LastChangedRevision: 938 $	
 *  \brief   Definition of CdtLocalXferData class
 *           This provides synchronization for Conduits local Transfers
 *
 *  $Id: CdtLocalXferData.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef CONDUITLOCALXFERDATA_H_
#define CONDUITLOCALXFERDATA_H_

#include <SendRequest.h>
#include <pthread.h>
namespace ipvtol
{

struct CdtLocalXferData
{
  enum XferType { NONE=0, SYNC=1, ASYNC=2 };

	pthread_mutex_t           clxdMutex;
	pthread_mutex_t           clxdCvMutex;
	pthread_cond_t            clxdCond;
	void                     *srcAddr;
	void                     *destAddr;
	PvtolXferType             sendType;
	int                       insertPosted;
	int                       eocPosted;
	int                       insertWaiting;
	int                       extractWaiting;
	bool                      noBuffAvailable;
	SendRequest              *sReq;
};

}//end namespace

#endif /*CONDUITLOCALXFERDATA_H_*/
