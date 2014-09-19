/**
 *    File: PvtolBasics.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: jav5880 $
 *  \date    $LastChangedDate: 2009-02-19 13:45:17 -0500 (Thu, 19 Feb 2009) $
 *  \version $LastChangedRevision: 939 $
 *  \brief    All typdefs #defines and consts that that should be ubiquitous
 *              throughout the PVTOL code body
 *
 *  $Id: PvtolBasics.h 939 2009-02-19 18:45:17Z jav5880 $
 *
 */
#ifndef _PVTOLBASICS_H_
#define _PVTOLBASICS_H_

#ifdef _STANDARD_MPI
#include <mpi.h>
#endif // _STANDARD_MPI

#include <vector>
#include <iostream>
#include <pthread.h>
#include <assert.h>

typedef enum { LOC_INVALID=0, LOC_CPU=1, LOC_CUDA=2, LOC_OCL=3, LOC_NLOCS=4 } hTaskLoc;
typedef enum { HLENGTH=0, HWIDTH=1, HDEPTH=2, HNDIMS=3 } hTaskDims;
typedef enum { PLAT_INVALID=0, PLAT_C=1, PLAT_NVIDIA=2, PLAT_AMD=3, PLAT_INTEL=4, PLAT_NPLAT=5} hTaskPlat;

typedef struct {
   // Heterogeneous task location
   hTaskLoc location;
   // Heterogeneous task device rank
   int device;
   // Heterogeneous task process rank
   int process;
   // Heterogeneous platform identifier
   hTaskPlat platform;
} HTaskInfo;

//Find a good home for the following cell related configuration items

#ifdef _PVTOL_CELL
//Enable / disable logging 
#define _PVTOL_CELL_LOGGER 1
#endif

namespace ipvtol {

#ifdef _PVTOL_CELL
//define the number of default workers
const int DEFAULT_WORKERS=6;
const int MAX_WORKERS=6;
#endif
const int MAX_DIM=3;
const int MAX_PROCS=128;
const int MAX_TASKS=512;
const int MAX_THREADS=512;

typedef int ProcId;
typedef int RankId;

#ifdef PTHREAD_IS_STRUCT //__APPLE__
	typedef unsigned long TaskId;
	typedef unsigned long ThreadId;
#else
	typedef pthread_t TaskId;
	typedef pthread_t ThreadId;
#endif


typedef int SeqNum;
typedef long TransferId;

}//end namespace

#endif // end _PVTOLBASICS_H_

