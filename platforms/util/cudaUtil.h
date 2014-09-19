/**
 *  Copyright (c) 2009, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    cudaUtil.h
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
#ifndef CUDAUTIL_H
#define CUDAUTIL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PvtolBasics.h>
#include <cuda_runtime.h>

using namespace ipvtol;

#define STRLEN 256
//#define KILLONERR
#define CUDA_NOLIB   0x0
#define CUFFT_FLAG   0x1
#define CUBLAS_FLAG  0x2
#define CURAND_FLAG  0x4


/**
 * cudaCheckErr function
 *
 * This function checks to see if any errors have occured in using CUDA, and
 * prints out the relevent error information. This function only checks the last error to occur.
 *
 * \param err The error code to check
 * \param line The line at which the error code was generated
 * \param file The file in which the error code was generated
 * \return None
 *
 */
extern "C" void cudaCheckErr(cudaError_t err, int line, char * file);


/**
 * initCUDA function
 * 
 * \brief This function will initialize the CUDA system, finding any devices
 *        available, and initializing any 3rd party libraries requested
 *
 * \return None
 *
 */
extern "C" void initCUDA(int flags);

/**
 * closeCUDA function
 * 
 * \brief  This function will clean up, releasing and deleting all of the CUDA objects
 *         that were allocated or initialized during program execution
 * \return None
 *
 */
extern "C" void closeCUDA();

/**
 * cudaInitDevice function
 * 
 * \brief  This function selects the coprocessor device and mapping and performs
 *         any initialization tasks that need to occur.
 * \param  info Heterogeneous task information structure of the device to be initialized
 * \return None
 *
 */
extern "C" void cudaInitDevice(HTaskInfo * info);

/**
 * cudaInitMem function
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
extern "C" void * cudaInitMem(int * dims, int typeSize, int * stride, const char * name, HTaskInfo * info, int mapHostFlag );

/**
 * cudaFreeMem function
 * 
 * \brief  This function frees allocated memory on the host or device
 *         as specified.
 * \param  ptr Pointer to the data to be freed
 * \param  dims The dimensions of the data to be freed
 * \param  loc The location of the data to be freed
 * \return None
 *
 */
extern "C" void cudaFreeMem( void * ptr, int * dims, int typeSize, const char * name, HTaskInfo * info, int mapHostFlag );

/**
 * cudaClearMem function
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
extern "C" void cudaClearMem(int * dims, int typeSize, int stride, void * ptr, HTaskInfo * info);

/**
 * cudaMoveData function
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
extern "C" void cudaMoveData(	void * dst, int dstStride, HTaskInfo * dstInfo,
									   void * src, int srcStride, HTaskInfo * srcInfo,
									   int * dims, int typeSize, const char * name);

/**
 * cudaLaunchKernel function
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
extern "C" void cudaLaunchKernel(   const char * krn, int * dims, int nParams, int * paramSizes,
                                    void ** params, int * gDim, int * bDim, int locMem,
                                    HTaskInfo * info);

#endif
// END CUDA UTILITY FUNCTIONS
