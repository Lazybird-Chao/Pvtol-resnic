/**




*/


#include "../util/cpuUtil.h"
#define CPU
#include "cpuKernel.h"
#undef CPU


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
 
extern "C" void cpuLaunchKernel(	const char * krn, int * dims, int nParams, int * paramSizes, void ** params,
										int * gDim, int * bDim, int locMem, HTaskInfo * info) {
   printf("Launching %s on cpu\n", krn);
	/*if (!strcmp(krn,"cpu_mc_stage1")) {
	   cpu_mc_stage1(*((randState**)params[0]), *((phDetPkg**)params[1]), *((uint**)params[2]), *((float*)params[3]), *((int*)params[4]), *((InputStruct**)params[5]));
	}
	if (!strcmp(krn,"cpu_mc_stage2")) {
	   cpu_mc_stage2(*((phDetPkg**)params[0]), *((positionGrid**)params[1]), *((uint**)params[2]), *((float*)params[3]));
	}*/

	return;
}



