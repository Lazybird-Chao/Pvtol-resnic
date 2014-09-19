/**
 *  Copyright (c) 2008, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  smohindra, $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $	
 *  \version $LastChangedRevision: 938 $	
 *  \brief   Definition of InterThreadXferData class
 *           This provides synchronization for transferring data between threads
 *
 *  $Id: InterThreadXferData.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef INTERTHREADXFERDATA_H_
#define INTERTHREADXFERDATA_H_

#include <SendRequest.h>
#include <pthread.h>
namespace ipvtol
{

  enum PvtolXferType { NONE=0, SYNC=1, ASYNC=2 };

struct InterThreadXferData
{
	pthread_mutex_t           itxdMutex;
	pthread_mutex_t           itxdCvMutex;
	pthread_cond_t            itxdCond;
	void                     *srcAddr;
	void                     *destAddr;
	PvtolXferType             sendType;
	bool                      firstOneIn;
	SendRequest              *sReq;
};

}

#endif /*INTERTHREADXFERDATA_H_*/
