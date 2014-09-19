/**
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

#include "cudaUtil.h"
#define CUDA
#include "cudaKernel.h"
#undef CUDA

// Global variables
cudaDeviceProp ** devProp;  // CUDA Device propertie structures
int nDev;                   // Number of CUDA devices available
int cudaSysInit = 0;        // System initialization flag
int sysFlags;               // System flags

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
extern "C" void cudaCheckErr(cudaError_t err, int line, char * file) {
  if (err != cudaSuccess) {
    const char * err_str = cudaGetErrorString(err);
    printf("CUDA ERROR %d: %s on line %d in file %s\n", err, err_str, line, file);
#ifdef KILLONERR
    exit(err);
#endif
  }
  return;
}

/**
 * initCUDA function
 * 
 * \brief This function will initialize the CUDA system, finding any devices
 *        available, and initializing any 3rd party libraries requested
 * \return None
 *
 */
extern "C" void initCUDA(int flags) {
  sysFlags = flags;
  if (cudaSysInit == 0) {
    cudaCheckErr(cudaGetDeviceCount(&nDev),__LINE__,__FILE__);
    devProp = (cudaDeviceProp**)malloc(sizeof(cudaDeviceProp*)*nDev);
    for (int idx = 0; idx < nDev; idx++) {
      devProp[idx] = new cudaDeviceProp;
    }
    if (sysFlags & CUBLAS_FLAG) { /* Init CUBLAS interface */ }
    if (sysFlags & CUFFT_FLAG) { /* Init CUFFT interface */ }
    if (sysFlags & CURAND_FLAG) { /* Init CURAND interface */ }
  }
  return;
}

/**
 * closeCUDA function
 * 
 * \brief  This function will clean up, releasing and deleting all of the CUDA objects
 *         that were allocated or initialized during program execution
 * \return None
 *
 */
extern "C" void closeCUDA() {
  if (sysFlags & CUFFT_FLAG) { /* Close CUFFT interface */ }
  if (sysFlags & CUBLAS_FLAG) { /* Close CUBLAS interface */ }
  if (sysFlags & CURAND_FLAG) { /* Close CURAND interface */ }
  // Delete and close main CUDA structures last
  for (int idx = 0; idx < nDev; idx++) { delete devProp[idx]; }
  free(devProp); nDev = 0;
  return;
}

/**
 * cudaInitDevice function
 * 
 * \brief  This function selects the coprocessor device and mapping and performs
 *         any initialization tasks that need to occur.
 * \param  info Heterogeneous task information structure of the device to be initialized
 * \return None
 *
 */
extern "C" void cudaInitDevice(HTaskInfo * info) {
  int idx = 0;
  cudaError_t err = cudaSuccess;
  int dev = info->device;
  int proc = info->process;
  hTaskLoc loc = info->location;

  // Device Management and initialization
  if (dev != -1) {
    err = cudaSetDevice(dev);
    if (err != cudaSuccess) {
      printf("CUDAUTIL WARNING: Could not assign to previously initialized device %d\n", dev);
      idx = 0;
      while ((cudaSetDevice(idx) != cudaSuccess) && (idx < nDev)) { idx++; }
      if (idx == nDev) {
	printf("CUDAUTIL ERROR: Could not assign a CUDA device!\n"); exit(-333);
      } else {
	cudaCheckErr(cudaGetDevice(&dev),__LINE__,__FILE__);
	info->device = dev;
	printf("CUDAUTIL WARNING: Re-assigning to CUDA device %d\n", dev);
      }
    } else {
      printf("Using CUDA Device %d\n", dev);
    }
  } else {
    idx = 0;
    while ((cudaSetDevice(idx) != cudaSuccess) && (idx < nDev)) { idx++; }
    if (idx == nDev) {
      printf("CUDAUTIL ERROR: Could not assign a CUDA device!\n"); exit(-334);
    } else {
      cudaCheckErr(cudaGetDevice(&dev),__LINE__,__FILE__);
      info->device = dev;
      printf("Assigning to CUDA device %d\n", dev);
    }
  }

  // Process management and initialization
  if ((proc != -1) && (proc != 0)) {
    cudaCheckErr(cudaStreamCreate((cudaStream_t*)&proc),__LINE__,__FILE__);
    info->process = proc;
    printf("Assigning to CUDA Stream %d\n", proc);  
  } else {
    info->process = proc;
  }
  return;
}

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
extern "C" void * cudaInitMem(int * dims, int typeSize, int * stride, const char * name, HTaskInfo * info, int mapHostFlag) {
  void * locPtr = NULL;
  size_t free, total = 0;
  int datSize = 1;
  int dev = info->device;
  hTaskLoc loc = info->location;

  // Set to proper device
  cudaCheckErr(cudaSetDevice(dev),__LINE__,__FILE__);

  // Get full data size
  for (int idx = 0; idx < HNDIMS; idx++) { datSize *= dims[idx]; }
  datSize *= typeSize;

  if (!strcmp(name,"")) { // Data is not a symbol
    // CHECK 1: Memory requested available in global memory
    cudaCheckErr(cudaMemGetInfo(&free, &total),__LINE__,__FILE__);
    if (free > datSize) { // There is adequate available memory
      // Allocate memory for an object
      cudaExtent extent = make_cudaExtent(dims[HLENGTH]*typeSize, dims[HWIDTH], dims[HDEPTH]);
      cudaPitchedPtr pitchedPtr;
      cudaCheckErr(cudaMalloc3D(&pitchedPtr, extent),__LINE__,__FILE__);
      cudaCheckErr(cudaMemset3D(pitchedPtr, 0, extent),__LINE__,__FILE__);
      locPtr = pitchedPtr.ptr;
      *stride = pitchedPtr.pitch;
      cudaCheckErr(cudaMemGetInfo(&free, &total),__LINE__,__FILE__);
      printf("Allocated %dB of memory on device. %dB remaining of %dB total\n", datSize, free, total);
      if (*stride != (dims[HLENGTH]*typeSize)) {
	printf("CUDAUTIL WARNING: Stride is not the same size as dims[HLENGTH]\n");
      }
    } else {
      printf("CUDAUTIL ERROR: Attempting to allocate %dB of memory when only %dB are \
         available out of a total of %dB\n", datSize, free, total); exit(-336);
    }
  } else { // Data is a CUDA symbol
    cudaCheckErr(cudaGetSymbolAddress(&locPtr, name),__LINE__,__FILE__);
    cudaCheckErr(cudaGetSymbolSize((size_t*)(&dims[HLENGTH]), name),__LINE__,__FILE__);
  }
  cudaThreadSynchronize();
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return locPtr;
}

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
extern "C" void cudaFreeMem( void * ptr, int * dims, int typeSize, const char * name, HTaskInfo * info, int mapHostFlag ) {
  hTaskLoc loc = info->location;
  int dev = info->device;

  if (mapHostFlag) {
    cudaCheckErr(cudaFreeHost(ptr),__LINE__,__FILE__);
  } else {
    cudaCheckErr(cudaSetDevice(dev),__LINE__,__FILE__);
    if (!strcmp(name,"")) { cudaCheckErr(cudaFree(ptr),__LINE__,__FILE__); }
  }
  cudaThreadSynchronize();
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return;
}

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
extern "C" void cudaClearMem(int * dims, int typeSize, int stride, void * ptr, HTaskInfo * info) {
  int datSize = 0;
  int dev = info->device;
  hTaskLoc loc = info->location;

  // Set to proper device
  cudaCheckErr(cudaSetDevice(dev),__LINE__,__FILE__);

  // Get full data size
  for (int idx = 0; idx < HNDIMS; idx++) { datSize += dims[idx]; }
  datSize *= typeSize;

  // Clear memory on the host for device use
  cudaExtent extent = make_cudaExtent(dims[HLENGTH]*typeSize, dims[HWIDTH], dims[HDEPTH]);
  cudaPitchedPtr pitchedPtr;
  pitchedPtr.pitch = stride;
  pitchedPtr.ptr = ptr;
  pitchedPtr.xsize = dims[HLENGTH];
  pitchedPtr.ysize = dims[HWIDTH];
  cudaCheckErr(cudaMemset3D(pitchedPtr, 0, extent),__LINE__,__FILE__);
  cudaThreadSynchronize();
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return;
}

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
				int * dims, int typeSize, const char * name) {
  hTaskLoc srcLoc = srcInfo->location;
  hTaskLoc dstLoc = dstInfo->location;
  int srcDev = srcInfo->device;
  int dstDev = dstInfo->device;
  int dev = -1;
  int proc = -1;
  cudaMemcpy3DParms cpyParms = {0};
  cudaStream_t stream;
  cudaMemcpyKind kind;

  // Set to proper device
  if (srcLoc == LOC_CUDA) { dev = srcDev; }
  if (dstLoc == LOC_CUDA) { dev = dstDev; }
  if (dev != -1) { cudaCheckErr(cudaSetDevice(dev),__LINE__,__FILE__); }

  if (!strcmp(name,"")) { // conduit is not for a symbol
    // Determine stream value
    if ((srcLoc == LOC_CUDA) && (dstLoc == LOC_CPU)) {
      proc = srcInfo->process;
      kind = cudaMemcpyDeviceToHost;
    } else if ((dstLoc == LOC_CUDA) && (srcLoc == LOC_CPU)) {
      proc = dstInfo->process;
      kind = cudaMemcpyHostToDevice;
    } else if ((dstLoc == LOC_CUDA) && (srcLoc == LOC_CUDA)) {
      proc = srcInfo->process;
      kind = cudaMemcpyDeviceToDevice;
    } else if ((dstLoc == LOC_CPU) && (srcLoc == LOC_CPU)) {
      proc = srcInfo->process;
      kind = cudaMemcpyHostToHost;
    }
    // Both src and dst are CPUs
    int stride = (srcStride > dstStride) ? srcStride : dstStride;
    cpyParms.extent = make_cudaExtent(dims[HLENGTH]*typeSize, dims[HWIDTH], dims[HDEPTH]);
    cpyParms.srcPtr.pitch = stride;
    cpyParms.srcPtr.ptr = src;
    cpyParms.srcPtr.xsize = dims[HLENGTH]*typeSize;
    cpyParms.srcPtr.ysize = dims[HWIDTH];
    cpyParms.dstPtr.pitch = stride;
    cpyParms.dstPtr.ptr = dst;
    cpyParms.dstPtr.xsize = dims[HLENGTH]*typeSize;
    cpyParms.dstPtr.ysize = dims[HWIDTH];
    cpyParms.kind = kind;
    cudaCheckErr(cudaMemcpy3DAsync(&cpyParms, (cudaStream_t)(proc)),__LINE__,__FILE__);
    cudaStreamSynchronize((cudaStream_t)(proc));
  } else { // this is a symbol
    size_t datSize = dims[HLENGTH]*dims[HWIDTH]*dims[HDEPTH]*typeSize;
    if (dstLoc == LOC_CUDA) {
      cudaMemcpyToSymbol(name, src, datSize, 0, cudaMemcpyHostToDevice);
    } else if (srcLoc == LOC_CUDA) {
      cudaMemcpyFromSymbol(dst, name, datSize, 0, cudaMemcpyDeviceToHost);
    } else {
      printf("CUDAUTIL ERROR: Invalid device location for symbol\n"); exit(-340);
    }
  }
  cudaThreadSynchronize();
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return;
}

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
/*extern "C" void cudaLaunchKernel(	const char * krn, int * dims, int nParams, int * paramSizes, void ** params,
					int * gDim, int * bDim, int locMem, HTaskInfo * info) {
  hTaskLoc loc = info->location;
  int dev = info->device;
  int proc = info->process;
  size_t offset = 0;
  dim3 gridDims = dim3(gDim[HLENGTH],gDim[HWIDTH],gDim[HDEPTH]);
  dim3 blockDims = dim3(bDim[HLENGTH],bDim[HWIDTH],bDim[HDEPTH]);


  // Set the device and configure the kernel call to a stream
  cudaCheckErr(cudaSetDevice(dev),__LINE__,__FILE__);
  cudaCheckErr(cudaConfigureCall(gridDims, blockDims, locMem, (cudaStream_t)proc),__LINE__,__FILE__);
	
  // For each kernel parameter passed in, push the kernel argument onto the stack
  //   turn into internal function
  float *s;
  cudaMalloc(&s,sizeof(float)*2);
  params[0]=&s;
  for (int i = 0; i < nParams; i++) {
    cudaCheckErr(cudaSetupArgument(params[i], paramSizes[i], offset),__LINE__,__FILE__);
    offset = offset + paramSizes[i];
  }
  
  if (!strcmp(krn,"scale")) {
    printf("Launching %s on dev:%d\n", krn, dev);
    cudaCheckErr(cudaLaunch(scale),__LINE__,__FILE__);
  }

  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);

  if ((proc != 0) && (proc != -1)) { cudaStreamSynchronize((cudaStream_t)proc); }
  //cudaThreadSynchronize();
  cudaCheckErr(cudaDeviceSynchronize(),__LINE__,__FILE__);
  printf("proc= %d  run to here\n", proc);
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return;
}
*/

