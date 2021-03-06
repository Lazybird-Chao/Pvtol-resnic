/**
 *
 *  \file    hUtil.h
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: $
 *  \version $LastChangedRevision: $
 *  \brief   Function declarations and wrappers for the CUDA Utility functions.
 *           This file is used to link the CUDA Utility functions to the PVTOL
 *           Tasks and Conduits framework software and can not contain any
 *           CUDA specific functions or types.
 *
 *  $Id: $
 *
 *  Author: James Brock
 */
#ifndef HUTIL_H
#define HUTIL_H

#include <PvtolBasics.h>
#include "cpuUtil.h"
#include "cudaUtil.h" 
#include "oclUtil.h"

#define NO_HSYS_FLAG    0x0
#define CUDA_SYS_FLAG   0x10
#define OCL_SYS_FLAG    0x20

/////////////////////// FUNCTION DECLARATIONS //////////////////////////////

// System and device functions
extern "C" void initSystem(  int flags );
extern "C" void initDevice(  HTaskInfo * info );
extern "C" void closeSystem( int flags );

// Memory operation functions
extern "C" void * initMem(  int * dims, int typeSize, int * stride, const char * name,
                            HTaskInfo * info, int mapHostFlag );
extern "C" void   freeMem(  void * ptr, int * dims, int typeSize, const char * name,
                            HTaskInfo * info, int mapHostFlag );
extern "C" void   clearMem( int * dims, int typeSize, int stride, void * ptr,
                            HTaskInfo * info);
extern "C" void   moveData( void * dst, int dstStride, HTaskInfo * dstInfo,
                            void * src, int srcStride, HTaskInfo * srcInfo,
								    int * dims, int typeSize, const char * name);
// Kernel build and launch functions
extern "C" void build( HTaskInfo * info, char * srcFile);
extern "C" void safeBuild( HTaskInfo * info, char * srcFile);
extern "C" void launchKernel(	const char * krn, int * dims, int nParams, int * paramSizes,
                              void ** params, int * gDim, int * bDim, int locMem,
                              HTaskInfo * info);
// Helper functions
extern "C" hTaskLoc findLocation(HTaskInfo * infos, int ni);

////////////////////// FUNCTION IMPLEMENTATIONS ////////////////////////////

/* findLocation function */
extern "C" inline
hTaskLoc findLocation(HTaskInfo * infos, int ni) {
   int idx = 0;
   hTaskLoc loc = LOC_INVALID;
   for (idx = 0; idx < ni; idx++) {
      if (infos[idx].location > loc) { loc = infos[idx].location; }
   }
   return loc;
}

/* initSystem function */
extern "C" inline
void initSystem(int flags) {
   if (flags & CUDA_SYS_FLAG) { initCUDA(flags); }
   if (flags & OCL_SYS_FLAG) { initOpenCL(); }
   return;
}

/* closeSystem function */
extern "C" inline
void closeSystem(int flags) {
   if (flags & CUDA_SYS_FLAG) { closeCUDA(); }
   if (flags & OCL_SYS_FLAG) { closeOpenCL(); }
   return;
}

/* initDevice function */
extern "C" inline
void initDevice(HTaskInfo * info) {
   hTaskLoc loc = findLocation(info, 1);
   if (loc == LOC_CUDA) {
      cudaInitDevice(info);
   } else if (loc == LOC_OCL) {
      oclInitDevice(info);
   } else if (loc == LOC_CPU) {
      // Do nothing
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location %s:%d\n",__FILE__,__LINE__);
   }
   return;
}      
      
/* initMem function */
extern "C" inline
void * initMem(int * dims, int typeSize, int * stride, const char * name, HTaskInfo * info, int mapHostFlag) {
   void * ptr = NULL;
   hTaskLoc loc = findLocation(info, 1);
   if (loc == LOC_CUDA) {
      ptr = cudaInitMem(dims, typeSize, stride, name, info, mapHostFlag);
   } else if (loc == LOC_OCL) {
      ptr = oclInitMem(dims, typeSize, stride, name, info, mapHostFlag);
   } else if (loc == LOC_CPU) {
      ptr = cpuInitMem(dims, typeSize, stride, name, info, mapHostFlag);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location %s:%d\n",__FILE__,__LINE__);
   }
   return ptr;
}

/* freeMem function */
extern "C" inline
void freeMem( void * ptr, int * dims, int typeSize, const char * name, HTaskInfo * info, int mapHostFlag ) {
   hTaskLoc loc = findLocation(info, 1);
   if (loc == LOC_CUDA) {
      cudaFreeMem(ptr, dims, typeSize, name, info, mapHostFlag);
   } else if (loc == LOC_OCL) {
      oclFreeMem(ptr, dims, typeSize, name, info, mapHostFlag);
   } else if (loc == LOC_CPU) {
      cpuFreeMem(ptr, dims, typeSize, name, info, mapHostFlag);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location %s:%d\n",__FILE__,__LINE__);
   }
   return;
}

/* clearMem function */
extern "C" inline
void clearMem(int * dims, int typeSize, int stride, void * ptr, HTaskInfo * info) {
   hTaskLoc loc = findLocation(info, 1);
   if (loc == LOC_CUDA) {
      cudaClearMem(dims, typeSize, stride, ptr, info);
   } else if (loc == LOC_OCL) {
      oclClearMem(dims, typeSize, stride, ptr, info);
   } else if (loc == LOC_CPU) {
      cpuClearMem(dims, typeSize, stride, ptr, info);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location %s:%d\n",__FILE__,__LINE__);
   }
   return;
}

/* moveData function */
extern "C" inline
void moveData(void * dst, int dstStride, HTaskInfo * dstInfo, void * src, int srcStride,
              HTaskInfo * srcInfo, int * dims, int typeSize, const char * name) {
   HTaskInfo infos[2];
   infos[0] = *srcInfo; infos[1] = *dstInfo;
   hTaskLoc loc = findLocation(infos, 2);

   if (loc == LOC_CUDA) {
      cudaMoveData(dst, dstStride, dstInfo, src, srcStride, srcInfo, dims, typeSize, name);
   } else if (loc == LOC_OCL) {
      oclMoveData(dst, dstStride, dstInfo, src, srcStride, srcInfo, dims, typeSize, name);
   } else if (loc == LOC_CPU) {
      cpuMoveData(dst, dstStride, dstInfo, src, srcStride, srcInfo, dims, typeSize, name);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location loc:%d %s:%d\n",loc,__FILE__,__LINE__);
   }
   return;
}

/* launchKernel function */
extern "C" inline
void launchKernel(const char * krn, int * dims, int nParams, int * paramSizes,
                  void ** params, int * gDim, int * bDim, int locMem, HTaskInfo * info) {
   hTaskLoc loc = findLocation(info, 1);
   if (loc == LOC_CUDA) {
      cudaLaunchKernel(krn, dims, nParams, paramSizes, params, gDim, bDim, locMem, info);
   } else if (loc == LOC_OCL) {
      oclLaunchKernel(krn, dims, nParams, paramSizes, params, gDim, bDim, locMem, info);
   } else if (loc == LOC_CPU) {
      cpuLaunchKernel(krn, dims, nParams, paramSizes, params, gDim, bDim, locMem, info);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location loc:%d %s:%d\n",loc,__FILE__,__LINE__);
   }
   return;
}

extern "C" inline
void build(HTaskInfo * info, char * srcFile) {
   hTaskLoc loc = info->location;
   if (loc == LOC_OCL) {
      oclBuild(info, srcFile);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location loc:%d %s:%d\n",loc,__FILE__,__LINE__);
   }
}

extern "C" inline
void safeBuild(HTaskInfo * info, char * srcFile) {
   hTaskLoc loc = info->location;
   if (loc == LOC_OCL) {
      oclSafeBuild(info, srcFile);
   } else {
      printf("HUTIL ERROR: Invalid heterogeneous device location loc:%d %s:%d\n",loc,__FILE__,__LINE__);
   }
   return;
}

#endif
/* END UTILITY FUNCTIONS */
