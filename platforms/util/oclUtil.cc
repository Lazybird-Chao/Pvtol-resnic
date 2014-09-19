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

#include "oclUtil.h"
//#include "oclKernel.h"


clState glblState = {0};
int oclSysInit = 0;
char * buildOpts = "-DOCLBUILD -I./platforms/app -I./platforms/util";

/**
 * initOpenCL function
 * 
 * \brief This function will initialize the OpenCL system, finding any platforms and devices
 *        available, and printing out a short list of them.
 *
 * \return None
 *
 */
extern "C" void initOpenCL() {
  int idx, jdx = 0,kdx=0;
   size_t psize;
   char * pstr;
   cl_int err = CL_SUCCESS;
   cl_device_type tmptype;
   
   if (oclSysInit == 0) {
      // Query and list platforms
      clCheckErr(clGetPlatformIDs(0, NULL, &(glblState.nplat)),__LINE__,__FILE__);
      cl_platform_id * plats = (cl_platform_id*)malloc(sizeof(cl_platform_id)*glblState.nplat);
      glblState.plats = (oclPlatInfo*)malloc(sizeof(oclPlatInfo)*glblState.nplat);
      memset(glblState.plats, 0, sizeof(oclPlatInfo)*glblState.nplat);
      clCheckErr(clGetPlatformIDs(glblState.nplat,plats,NULL),__LINE__,__FILE__);

      // For each platform, find and list the devices
      for (idx = 0; idx < glblState.nplat; idx++) {
         oclPlatInfo * plt = &(glblState.plats[idx]);
         plt->pid = plats[idx];
         plt->prg = 0;
         printf("Found platform %d: 0x%X ==> ", idx, plt->pid);
         clCheckErr(clGetPlatformInfo(plt->pid, CL_PLATFORM_VENDOR, NULL, NULL, &psize),__LINE__,__FILE__);
         pstr = (char*)malloc(psize);
         clCheckErr(clGetPlatformInfo(plt->pid, CL_PLATFORM_VENDOR, psize, pstr, NULL),__LINE__,__FILE__);
         printf("%s ", pstr); free(pstr);
         clCheckErr(clGetPlatformInfo(plt->pid, CL_PLATFORM_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
         pstr = (char*)malloc(psize);
         clCheckErr(clGetPlatformInfo(plt->pid, CL_PLATFORM_NAME, psize, pstr, NULL),__LINE__,__FILE__);
         printf("%s\n", pstr); free(pstr);

         clCheckErr(clGetDeviceIDs(plt->pid, CL_DEVICE_TYPE_ALL, NULL, NULL, &(plt->ndevs)),__LINE__,__FILE__);
         cl_device_id * devs = (cl_device_id*)malloc(sizeof(cl_device_id)*plt->ndevs);
         plt->devs = (oclDeviceInfo*)malloc(sizeof(oclDeviceInfo)*plt->ndevs);
         memset(plt->devs, 0, sizeof(oclDeviceInfo)*plt->ndevs);
         clCheckErr(clGetDeviceIDs(plt->pid, CL_DEVICE_TYPE_ALL, plt->ndevs, devs, NULL),__LINE__,__FILE__);
         cl_context_properties cprops[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(plt->pid), 0};
         plt->ctx = clCreateContext(cprops, plt->ndevs, devs, NULL, NULL, &err);
         clRetainContext(plt->ctx);
         clCheckErr(err,__LINE__,__FILE__);

         // For each device in the platform, print some basic information
         for (jdx = 0; jdx < plt->ndevs; jdx++) {
            oclDeviceInfo * dev = &(plt->devs[jdx]);
            for (kdx = 0; kdx < MAX_PROCESSES; kdx++) { dev->queues[kdx] = 0; }
            dev->did = devs[jdx];
            printf("\tDevice %d (ID:0x%X):", jdx, dev->did );
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
            pstr = (char*)malloc(psize);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
            printf(" %s", pstr);
            free(pstr);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_TYPE, sizeof(cl_device_type), &tmptype, NULL),__LINE__,__FILE__);
            pstr = (char*)malloc(sizeof(char)*STRLEN);
            clDecodeDeviceType(tmptype, pstr);
            printf(" (%s)", pstr);
            free(pstr);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
            pstr = (char*)malloc(psize);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_VERSION, psize, pstr, NULL),__LINE__,__FILE__);
            printf(" ver. %s\n", pstr);
            free(pstr);
         }
         free(devs);
      }
      free(plats);
   } else {
      printf("OCLUTIL WARNING: OpenCL system already initialized\n");
   }
   return;
}

/**
 * oclInitDevice function
 * 
 * \brief  This function selects the coprocessor device and mapping and performs
 *         any initialization tasks that need to occur.
 * \param  info Heterogeneous task information structure of the device to be initialized
 * \return None
 *
 */
extern "C" void oclInitDevice(HTaskInfo * info) {
   cl_int err = CL_SUCCESS;
   int didx = info->device; 
   int qidx = info->process; 
   hTaskPlat platName = info->platform; 
   int pidx = clFindPlatform(platName);
   oclPlatInfo * plat = &(glblState.plats[pidx]);
   oclDeviceInfo * dev = &(plat->devs[didx]);

   if (dev->queues[qidx] == 0) {
      dev->queues[qidx] = clCreateCommandQueue(plat->ctx, dev->did, 0, &err);
      clRetainCommandQueue(dev->queues[qidx]);
      clCheckErr(err,__LINE__,__FILE__);
   } else {
      printf("OCLUTIL WARNING: Device has already been initialized pidx=%d, didx=%d, %s:%d\n",
              pidx, didx, __FILE__,__LINE__);
   }
   return;
}

/**
 * closeOpenCL function
 * 
 * \brief  This function will clean up, releasing and deleting all of the OpenCL objects
 *         that were allocated or initialized during program execution
 *
 * \return None
 *
 */ 
extern "C" void closeOpenCL() {
   int idx, jdx, kdx = 0;
   int nplat = glblState.nplat;

   for (idx = 0; idx < nplat; idx++) {
      oclPlatInfo * plat = &(glblState.plats[idx]);
      for (jdx = 0; jdx < plat->ndevs; jdx++) {
         oclDeviceInfo * dev = &(plat->devs[jdx]);
         for (kdx = 0; kdx < dev->nkrn; kdx++) {
            clCheckErr(clReleaseKernel(dev->kerns[kdx]),__LINE__,__FILE__);
         }
         for (kdx = 0; kdx < MAX_PROCESSES; kdx++) {
            clCheckErr(clReleaseCommandQueue(dev->queues[kdx]),__LINE__,__FILE__);
         }
#ifdef OPENCL_1.2
         //clCheckErr(clReleaseDevice(dev->did),__LINE__,__FILE__);
#endif
      }
      clCheckErr(clReleaseProgram(plat->prg),__LINE__,__FILE__);
      clCheckErr(clReleaseContext(plat->ctx),__LINE__,__FILE__);
      free(plat->devs);
   }
   free(glblState.plats);
   glblState.nplat = 0;
   return;
}

/**
 * oclMallocMem function
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
extern "C" void * oclInitMem(int * dims, int typeSize, int * stride, const char * name, HTaskInfo * info, int mapHostFlag) {
   int size = 0;
   hTaskLoc loc = info->location;
   int dev = info->device; 
   int proc = info->process; 
   cl_int err = CL_SUCCESS;
   void * ptr = NULL;
   int pidx = clFindPlatform(info->platform);
   cl_context ctx = glblState.plats[pidx].ctx;
   cl_command_queue queue = glblState.plats[pidx].devs[dev].queues[proc];
   int fill = 0;
   size = typeSize*dims[HLENGTH]*dims[HWIDTH]*dims[HDEPTH];

	// If the memory is to be allocated on the host
	if (loc == LOC_CPU) {
	   if (mapHostFlag) {
		   ptr = clCreateBuffer(ctx, (CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR), size, NULL, &err);
#ifdef OPENCL_1.2
		   //clCheckErr(clEnqueueFillBuffer(queue, ptr, &fill, sizeof(int), 0, size, 0, NULL, NULL),__LINE__,__FILE__);
		   clFlush(queue);
#else
		   memset(ptr, 0, size);
#endif
		} else {
		   ptr = malloc(size);
		   memset(ptr, 0, size);
		}
	} else if (loc == LOC_OCL) {
      if (!strcmp(name,"")) {
         ptr = clCreateBuffer(ctx, CL_MEM_READ_WRITE, size, NULL, &err);
      } else {
         ptr = clCreateBuffer(ctx, CL_MEM_READ_ONLY, size, NULL, &err);
      }
      clCheckErr(err,__LINE__,__FILE__);
   } else {
      printf("OCLUTIL ERROR: Invalid device location: line:%d file:%s\n",__LINE__,__FILE__); exit(-447);
   }
	return ptr;
}

/**
 * oclFreeMem function
 * 
 * \brief  This function frees allocated memory on the host or device
 *         as specified.
 * \param  ptr Pointer to the data to be freed
 * \param  dims The dimensions of the data to be freed
 * \param  loc The location of the data to be freed
 * \return None
 *
 */
extern "C" void oclFreeMem(void * ptr, int * dims, int typeSize, const char * name, HTaskInfo * info, int mapHostFlag) {
	hTaskLoc loc = info->location; //getLocation();
	if (loc == LOC_CPU) {
	   if (mapHostFlag) { clCheckErr(clReleaseMemObject((cl_mem)ptr),__LINE__,__FILE__); } else { free(ptr); }
	} else if (loc == LOC_OCL) {
	   clCheckErr(clReleaseMemObject((cl_mem)ptr),__LINE__,__FILE__);
	} else {
      printf("OCLUTIL ERROR: Invalid device location: line:%d file:%s\n",__LINE__,__FILE__); exit(-448);
	}
	return;
}

/**
 * oclClearMem function
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
extern "C" void oclClearMem(int * dims, int typeSize, int stride, void * ptr, HTaskInfo * info) {
   int size = 0;
   hTaskLoc loc = info->location; //getLocation();
   int dev = info->device; 
   int proc = info->process; 
   int pidx = clFindPlatform(info->platform); //getPlatform());
   cl_command_queue queue = glblState.plats[pidx].devs[dev].queues[proc];
   int fill = 0;
   size = typeSize*dims[HLENGTH]*dims[HWIDTH]*dims[HDEPTH];
#ifdef OPENCL_1.2
   //clCheckErr(clEnqueueFillBuffer(queue, ptr, &fill, sizeof(int), 0, size, 0, NULL, NULL),__LINE__,__FILE__);
#endif
   clCheckErr(clEnqueueBarrier(queue),__LINE__,__FILE__);
   clFlush(queue);
   return;
}

/**
 * oclMoveData function
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
extern "C" void oclMoveData(	void * dst, int dstStride, HTaskInfo * dstInfo,
									   void * src, int srcStride, HTaskInfo * srcInfo,
									   int * dims, int typeSize, const char * name) {
   hTaskLoc srcLoc = srcInfo->location;
   hTaskLoc dstLoc = dstInfo->location;
   int size = typeSize*dims[HLENGTH]*dims[HWIDTH]*dims[HDEPTH];
   int dev, proc, pidx;
   cl_command_queue queue;

   if ((srcLoc == LOC_CPU) && (dstLoc == LOC_OCL)) {
      dev = dstInfo->device;
      proc = dstInfo->process;
      pidx = clFindPlatform(dstInfo->platform);
      queue = glblState.plats[pidx].devs[dev].queues[proc];
      clCheckErr(clEnqueueWriteBuffer(queue, (cl_mem)dst, CL_TRUE, 0, size, src, 0, NULL, NULL),__LINE__,__FILE__);
   } else if ((srcLoc == LOC_OCL) && (dstLoc == LOC_CPU)) {
      dev = srcInfo->device;
      proc = srcInfo->process;
      pidx = clFindPlatform(srcInfo->platform);
      queue = glblState.plats[pidx].devs[dev].queues[proc];
      clCheckErr(clEnqueueReadBuffer(queue, (cl_mem)src, CL_TRUE, 0, size, dst, 0, NULL, NULL),__LINE__,__FILE__);
   } else if ((srcLoc == LOC_OCL) && (dstLoc == LOC_OCL)) {
      dev = dstInfo->device;
      proc = dstInfo->process;
      pidx = clFindPlatform(dstInfo->platform);
      queue = glblState.plats[pidx].devs[dev].queues[proc];
      clCheckErr(clEnqueueCopyBuffer(queue, (cl_mem)src, (cl_mem)dst, 0, 0, size, 0, NULL, NULL),__LINE__,__FILE__);
   } else {
      printf("OCLUTIL ERROR: Invalid device locations: line:%d file:%s\n",__LINE__,__FILE__); exit(-449);
   }
	
   clCheckErr(clEnqueueBarrier(queue),__LINE__,__FILE__);
   clCheckErr(clFinish(queue),__LINE__,__FILE__);   
	return;
}

/**
 * clSafeBuild function
 * 
 * \brief  This function will build the kernels in the specified source file for the given
 *         heterogeneous device, printing build errors and debug information out at
 *         each compilation.
 *
 * \param  info The heterogeneous device information of the device to be used
 * \param  srcFile File name of the kernel source file
 * \return None
 *
 */
extern "C" void oclSafeBuild(HTaskInfo * info, char * srcFile) {
   cl_int err = CL_SUCCESS;
   size_t psize;
   char * pstr;
   char * src = NULL;
   FILE * fid = NULL;
   size_t srcSz = 0;
   bool built = false;
   bool first = true;
   int pidx = clFindPlatform(info->platform); //getPlatform());
   oclPlatInfo * plat = &(glblState.plats[pidx]);
   cl_kernel * krns = (cl_kernel*)malloc(sizeof(cl_kernel)*MAX_KERNS);
   cl_device_id * devs = (cl_device_id*)malloc(sizeof(cl_device_id)*plat->ndevs);
   for (int idx = 0; idx < plat->ndevs; idx++) { devs[idx] = plat->devs[idx].did; }

   while(!built) {
      fid = fopen(srcFile,"rb");
      if (fid == NULL) {
         printf("OCLUTIL ERROR: Unable to open file %s for building program!\n", srcFile); exit(-551);
      }
      fseek(fid, 0, SEEK_END);
      srcSz = ftell(fid);
      rewind(fid);
      src = (char*)malloc(sizeof(char)*srcSz);
      srcSz = fread(src, sizeof(char), srcSz, fid);
      fclose(fid);
      if (!first) { clReleaseProgram(plat->prg); first = false; }
      plat->prg = clCreateProgramWithSource(plat->ctx, 1, (const char**)&src, &srcSz, &err);
      clCheckErr(err,__LINE__,__FILE__);
      err = clBuildProgram(plat->prg, plat->ndevs, devs, buildOpts, NULL, NULL);

      if (err != CL_SUCCESS) {
         cl_build_status bst;
         char * pstr = (char*)malloc(sizeof(char)*STRLEN);
         clDecodeErr(err, pstr);
         printf("Encountered program build error %d: %s\n", err, pstr);
         free(pstr);
         for (int idx = 0; idx < plat->ndevs; idx++) {
            oclDeviceInfo * dev = &(plat->devs[idx]);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
            pstr = (char*)malloc(psize);
            clCheckErr(clGetDeviceInfo(dev->did, CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
            printf("\tProgram build for device: %s", pstr); 
            free(pstr);
            clCheckErr(clGetProgramBuildInfo(plat->prg, dev->did, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &bst, NULL),__LINE__,__FILE__);
            if (bst != CL_BUILD_SUCCESS) {
               printf(" has FAILED! See information below ...\n");
               clProgramBuildDump(plat->prg, dev->did);
               bool input = false;
               char inval = 'x';
               while (!input) {
                  printf("\nWould you like to try to re-build the program [Y/N]? ");
                  std::cin >> inval;
                  input = ((inval=='Y')||(inval=='N')) ? true : false;
               }
               if (inval=='N') { built = true; break; }
            } else {  printf(" has SUCCEEDED!\n"); }
         }
      } else {
         built = true; printf("Built program successfully!\n");
         for (int idx = 0; idx < plat->ndevs; idx++) {
            oclDeviceInfo * dev = &(plat->devs[idx]);
            clCheckErr(clCreateKernelsInProgram(plat->prg, MAX_KERNS, krns, &(dev->nkrn)),__LINE__,__FILE__);
            for (int jdx = 0; jdx < dev->nkrn; jdx++) { dev->kerns[jdx] = krns[jdx]; }
         }
      }
      free(src);
   }
   free(krns);
   free(devs);
   return;
}

/**
 * clBuild function
 * 
 * \brief  This function will build the kernels in the specified source file for the given
 *         heterogeneous device
 *
 * \param  info The heterogeneous device information of the device to be used
 * \param  srcFile File name of the kernel source file
 * \return None
 *
 */
extern "C" void oclBuild(HTaskInfo * info, char * srcFile) {
   cl_int err = CL_SUCCESS;
   char * src = NULL;
   FILE * fid = NULL;
   size_t srcSz = 0;
   int pidx = clFindPlatform(info->platform); //getPlatform());
   oclPlatInfo * plat = &(glblState.plats[pidx]);
   cl_kernel * krns = (cl_kernel*)malloc(sizeof(cl_kernel)*MAX_KERNS);

   fid = fopen(srcFile,"rb");
   if (fid == NULL) {
      printf("OCLUTIL ERROR: Unable to open file %s for building program!\n", srcFile); exit(-551);
   }
   fseek(fid, 0, SEEK_END);
   srcSz = ftell(fid);
   rewind(fid);
   src = (char*)malloc(sizeof(char)*srcSz);
   srcSz = fread(src, sizeof(char), srcSz, fid);
   fclose(fid);
   plat->prg = clCreateProgramWithSource(plat->ctx, 1, (const char**)&src, &srcSz, &err);
   clCheckErr(err,__LINE__,__FILE__);
   
   for (int idx = 0; idx < plat->ndevs; idx++) {
      oclDeviceInfo * dev = &(plat->devs[idx]);
      clCheckErr(clBuildProgram(plat->prg, 1, &(dev->did), "", NULL, NULL),__LINE__,__FILE__);
      clCheckErr(clCreateKernelsInProgram(plat->prg, MAX_KERNS, krns, &(dev->nkrn)),__LINE__,__FILE__);
      for (int jdx = 0; jdx < dev->nkrn; jdx++) {
         dev->kerns[jdx] = krns[jdx];
      }
   }
   free(src);
   free(krns);
   return;
}

/**
 * oclLaunchKernel function
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
extern "C" void oclLaunchKernel(const char * krn, int * dims, int nParams,
                                int * paramSizes, void ** params,
										  int * gDim, int * bDim, int locMem, HTaskInfo * info) {
   int work_dim = 1;
   hTaskLoc loc = info->location;
   int dev = info->device;
   int proc = info->process;
   int pidx = clFindPlatform(info->platform);
   oclPlatInfo * plat = &(glblState.plats[pidx]);
   cl_command_queue queue = plat->devs[dev].queues[proc];
   cl_kernel * kern = clFindKernel(plat->devs[dev].kerns, plat->devs[dev].nkrn, krn);

   size_t gSize[3] = {gDim[0]*bDim[0], gDim[1]*bDim[1], gDim[2]*bDim[2]};
   for (int idx = 1; idx < 3; idx++) {
      if (gSize[idx] > 1) {
         work_dim++;
      }
   }
   for (int idx = 0; idx < nParams; idx++) {
      clCheckErr(clSetKernelArg(*kern, idx, paramSizes[idx], params[idx]),__LINE__,__FILE__);
   }
   clCheckErr(clEnqueueNDRangeKernel(queue, *kern, work_dim, NULL, gSize, (size_t*)bDim, 0, NULL, NULL),__LINE__,__FILE__);
   clCheckErr(clEnqueueBarrier(queue),__LINE__,__FILE__);
   clCheckErr(clFinish(queue),__LINE__,__FILE__);
	return;
}

