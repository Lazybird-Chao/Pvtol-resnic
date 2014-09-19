/**
 *  Copyright (c) 2009, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file    oclHelpers.h
 *  \author  $LastChangedBy: $
 *  \date    $LastChangedDate: $
 *  \version $LastChangedRevision: $
 *  \brief   Function declarations and wrappers for the OpenCL helper functions.
 *
 *  $Id: $
 *
 *  Author: James Brock
 */
#undef __SSE2__
#undef __SSE__
#ifndef OCLHELP_H
#define OCLHELP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <typeinfo>
#include <CL/opencl.h>

#define STRLEN 256
// #define KILLONERR
#define MAX_PROCESSES   2
#define MAX_KERNS       32
#define OPENCL_1.0
#define OPENCL_1.1

// OpenCL Device
typedef struct {
   cl_device_id      did;
   cl_command_queue  queues[MAX_PROCESSES];
   cl_uint           nkrn;
   cl_kernel         kerns[MAX_KERNS];
} oclDeviceInfo;

// OpenCL Platform
typedef struct {
   cl_platform_id    pid;
   cl_context        ctx;
   cl_program        prg;
   oclDeviceInfo *   devs;
   cl_uint           ndevs;
} oclPlatInfo;

// OpenCL State
typedef struct {
   cl_uint           nplat;
   oclPlatInfo *     plats;
} clState;

////////////////////////////////////////////////////////////
//              OPENCL HELPER FUNCTIONS                   //
////////////////////////////////////////////////////////////
/**
 * OpenCL Helper Functions
 */
extern "C" void clDecodeErr(cl_int err, char * str);
extern "C" void clCheckErr(cl_int err, int line, char * file);
extern "C" void clDecodeFPConfig(cl_device_fp_config cfg, char * str);
extern "C" void clDecodeExecCapabilities(cl_device_exec_capabilities dcap, char * str);
extern "C" void clDecodeDevMemCacheType(cl_device_mem_cache_type mctype, char * str);
extern "C" void clDecodeLocalMemType(cl_device_local_mem_type lmtype, char * str);
extern "C" void clDecodeQueueProperties(cl_command_queue_properties qprop, char * str);
extern "C" void clDecodeDeviceType(cl_device_type dtype, char * str);
extern "C" void clDecodeMemObjType(cl_mem_object_type mtype, char * str);
extern "C" void clDecodeMemObjFlags(cl_mem_flags mflags, char * str);
extern "C" void clDecodeImageFormat(cl_image_format imgfmt, char * str);
extern "C" void clDecodeAddressingMode(cl_addressing_mode amd, char * str);
extern "C" void clDecodeFilterMode(cl_filter_mode fmd, char * str);
extern "C" void clDecodeBuildStatus(cl_build_status bst, char * str);
extern "C" void clPlatformDump(cl_platform_id pid);
extern "C" void clDeviceDump(cl_device_id did);
extern "C" void clContextDump(cl_context cid);
extern "C" void clMemObjDump(cl_mem mobj);
extern "C" void clImgObjDump(cl_mem img);
extern "C" void clSamplerDump(cl_sampler smp);
extern "C" void clProgamDump(cl_program pid);
extern "C" void clProgramBuildDump(cl_program prg, cl_device_id dev);
extern "C" void clKernelDump(cl_kernel krn);
extern "C" void clPrintPlats();
extern "C" void clPrintDevs(cl_platform_id pid);
extern "C" void clDecodeTaskPlat(int plat, char * cmpStr);
extern "C" int clFindPlatform(int plat);
extern "C" cl_kernel * clFindKernel(cl_kernel * krns, int nkrn, const char * krnName);

#endif
