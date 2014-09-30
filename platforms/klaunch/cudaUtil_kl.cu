/**



*/



#include "../util/cudaUtil.h"
#define CUDA
#include "cudaKernel.h"
#undef CUDA


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
extern "C" void cudaLaunchKernel(	const char * krn, int * dims, int nParams, int * paramSizes, void ** params,
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
  /*
  if (!strcmp(krn,"scale")) {
    printf("Launching %s on dev:%d\n", krn, dev);
    cudaCheckErr(cudaLaunch(scale),__LINE__,__FILE__);
  }*/

  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);

  if ((proc != 0) && (proc != -1)) { cudaStreamSynchronize((cudaStream_t)proc); }
  //cudaThreadSynchronize();
  cudaCheckErr(cudaDeviceSynchronize(),__LINE__,__FILE__);
  printf("proc= %d  run to here\n", proc);
  cudaCheckErr(cudaGetLastError(),__LINE__,__FILE__);
  return;
}


