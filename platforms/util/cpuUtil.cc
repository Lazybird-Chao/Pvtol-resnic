/**
 *  Copyright (c) 2009, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy$
 *  \date    $LastChangedDate$
 *  \version $LastChangedRevision$
 *  \brief   
 *
 *  $Id$
 *
 *  Author: James Brock
 */
#include "cpuUtil.h"
#define CPU
#include "cpuKernel.h"
#undef CPU

/**
 * cpuInitMem function
 * 
 * \brief  This function intelligently allocates memory on the host or device
 *         as specified and returns pointer to the memory as well as the
 *         stride for the data, which is used for multi-dimensional data
 *         blocks.
 * \param  dims The dimensions of the memory to be allocated
 * \param  stride The stride (width in bytes) of the data. This only matters
 *         for multi-dimensional data
 * \param  loc The location of the memory to be allocated (host or device)
 * \param  ptr A pointer to where the memory pointer is to be stored
 * \return None
 *
 */
extern "C" void * cpuInitMem(int * dims, int typeSize, int * stride, const char * name, HTaskInfo * info, int mapHostFlag) {
	void * locPtr = NULL;
   int datSize = 1;

   for (int idx = 0; idx < HNDIMS; idx++) { datSize *= dims[idx]; }
   datSize *= typeSize;
   locPtr = malloc(datSize);
   memset(locPtr, 0, datSize);
	return locPtr;
}

/**
 * cpuFreeMem function
 * 
 * \brief  This function frees allocated memory on the host or device
 *         as specified.
 * \param  ptr Pointer to the data to be freed
 * \param  dims The dimensions of the data to be freed
 * \param  loc The location of the data to be freed
 * \return None
 *
 */
extern "C" void cpuFreeMem( void * ptr, int * dims, int typeSize, const char * name, HTaskInfo * info, int mapHostFlag ) {
   free(ptr);
	return;
}

/**
 * cpuClearMem function
 * 
 * \brief  
 * \param  dims The dimensions of the memory to be allocated
 * \param  stride The stride (width in bytes) of the data. This only matters
 *         for multi-dimensional data
 * \param  loc The location of the memory to be allocated (host or device)
 * \param  ptr A pointer to where the memory pointer is to be stored
 * \return None
 *
 */
extern "C" void cpuClearMem(int * dims, int typeSize, int stride, void * ptr, HTaskInfo * info) {
   int datSize = 1;
   for (int idx = 0; idx < HNDIMS; idx++) { datSize *= dims[idx]; }
   datSize *= typeSize;
   memset(ptr, 0, datSize);
	return;
}

/**
 * cpuMoveData function
 * 
 * \brief  This function intelligently copies data from the source location
 *         with the specified stride to the destination location with the 
 *         specified stride. The dimensions parameter indicates the
 *         dimensions of the data and must be the same for both source and
 *         destination.
 * \param  dest Pointer to the destination memory location
 * \param  destStride Stride (width in bytes) of the destination
 *         memory location
 * \param  destLoc The location of the destination memory
 * \param  src Pointer to the source memory location
 * \param  srcStride Stride (width in bytes) of the source memory
 *         location
 * \param  srcLoc The location of the destination memory
 * \param  dims The dimensions of the data to be copied
 * \return None
 *
 */
extern "C" void cpuMoveData(	void * dst, int dstStride, HTaskInfo * dstInfo,
									   void * src, int srcStride, HTaskInfo * srcInfo,
									   int * dims, int typeSize, const char * name ) {
   int datSize = 1;
   for (int idx = 0; idx < HNDIMS; idx++) { datSize *= dims[idx]; }
   datSize *= typeSize;
   memcpy(dst, src, datSize);
	return;
}

/**
 * cpuLaunchKernel function
 * 
 * \brief  This function uses the specified parameters to execute the
 *         coprocessor function specified by kernel. Any parameters to
 *         the kernel should be included in params.
 * 
 * \param  kernel A string naming the kernel to execute
 * \param  dims The dimensions of the data to execute the kernel on
 * \param  nParams The number of parameters to be passed to the kernel
 * \param  params Array of parameters to be passed to the kernel
 * \param  gDim The grid dimensions of the kernel
 * \param  bDim The block dimensions of the kernel
 * \param  locMem The amount of local or shared memory to allocate for kernel
 *         execution
 * \param  stream The stream index to associate this kernel's execution and
 *         data with
 * \return None
 *
 */
 /*
extern "C" void cpuLaunchKernel(	const char * krn, int * dims, int nParams, int * paramSizes, void ** params,
										int * gDim, int * bDim, int locMem, HTaskInfo * info) {
   printf("Launching %s on cpu\n", krn);
	if (!strcmp(krn,"cpu_mc_stage1")) {
	   cpu_mc_stage1(*((randState**)params[0]), *((phDetPkg**)params[1]), *((uint**)params[2]), *((float*)params[3]), *((int*)params[4]), *((InputStruct**)params[5]));
	}
	if (!strcmp(krn,"cpu_mc_stage2")) {
	   cpu_mc_stage2(*((phDetPkg**)params[0]), *((positionGrid**)params[1]), *((uint**)params[2]), *((float*)params[3]));
	}
	return;
}
*/

