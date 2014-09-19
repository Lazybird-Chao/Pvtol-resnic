/**
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
#include "oclHelpers.h"

// OpenCL Helper variables
cl_int clErr;
size_t psize;
char * pstr;
cl_uint puint;
cl_bool pbool;
cl_ulong pulong;

////////////////////////////////////////////////////////////
//              OPENCL HELPER FUNCTIONS                   //
////////////////////////////////////////////////////////////
/**
 * clDecodeErr function
 *
 * \brief This function checks, analyzes, and diagnoses OpenCL error codes
 *
 * \param err the error code to check
 * \param str the string to write error messages to
 *
 * \return None
 *
 */
extern "C" void clDecodeErr(cl_int err, char * str) {
   str[0] = '\0';
   switch(err) {
      case CL_SUCCESS:                    strcat(str, "CL_SUCCESS"); break;
      case CL_DEVICE_NOT_FOUND:           strcat(str, "CL_DEVICE_NOT_FOUND"); break;
      case CL_DEVICE_NOT_AVAILABLE:       strcat(str, "CL_DEVICE_NOT_AVAILABLE"); break;
      case CL_COMPILER_NOT_AVAILABLE:     strcat(str, "CL_COMPILER_NOT_AVAILABLE"); break;
      case CL_MEM_OBJECT_ALLOCATION_FAILURE: strcat(str, "CL_MEM_OBJECT_ALLOCATION_FAILURE"); break;
      case CL_OUT_OF_RESOURCES:           strcat(str, "CL_OUT_OF_RESOURCES"); break;
      case CL_OUT_OF_HOST_MEMORY:         strcat(str, "CL_OUT_OF_HOST_MEMORY"); break;
      case CL_PROFILING_INFO_NOT_AVAILABLE:  strcat(str, "CL_PROFILING_INFO_NOT_AVAILABLE"); break;
      case CL_MEM_COPY_OVERLAP:           strcat(str, "CL_MEM_COPY_OVERLAP"); break;
      case CL_IMAGE_FORMAT_MISMATCH:      strcat(str, "CL_IMAGE_FORMAT_MISMATCH"); break;
      case CL_IMAGE_FORMAT_NOT_SUPPORTED: strcat(str, "CL_IMAGE_FORMAT_NOT_SUPPORTED"); break;
      case CL_BUILD_PROGRAM_FAILURE:      strcat(str, "CL_BUILD_PROGRAM_FAILURE"); break;
      case CL_MAP_FAILURE:                strcat(str, "CL_MAP_FAILURE"); break;
      case CL_MISALIGNED_SUB_BUFFER_OFFSET:  strcat(str, "CL_MISALIGNED_SUB_BUFFER_OFFSET"); break;
      case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: strcat(str, "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"); break;
      // Insert cases 15-30 here
      case CL_INVALID_VALUE:              strcat(str, "CL_INVALID_VALUE"); break;
      case CL_INVALID_DEVICE_TYPE:        strcat(str, "CL_INVLIAD_DEVICE_TYPE"); break;
      case CL_INVALID_PLATFORM:           strcat(str, "CL_INVALID_PLATFORM"); break;
      case CL_INVALID_DEVICE:             strcat(str, "CL_INVALID_DEVICE"); break;
      case CL_INVALID_CONTEXT:            strcat(str, "CL_INVALID_CONTEXT"); break;
      case CL_INVALID_QUEUE_PROPERTIES:   strcat(str, "CL_INVALID_QUEUE_PROPERTIES"); break;
      case CL_INVALID_COMMAND_QUEUE:      strcat(str, "CL_INVALID_COMMAND_QUEUE"); break;
      case CL_INVALID_HOST_PTR:           strcat(str, "CL_INVALID_HOST_PTR"); break;
      case CL_INVALID_MEM_OBJECT:         strcat(str, "CL_INVALID_MEM_OBJECT"); break;
      case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: strcat(str, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"); break;
      case CL_INVALID_IMAGE_SIZE:         strcat(str, "CL_INVALID_IMAGE_SIZE"); break;
      case CL_INVALID_SAMPLER:            strcat(str, "CL_INVALID_SAMPLER"); break;
      case CL_INVALID_BINARY:             strcat(str, "CL_INVALID_BINARY"); break;
      case CL_INVALID_BUILD_OPTIONS:      strcat(str, "CL_INVALID_BUILD_OPTIONS"); break;
      case CL_INVALID_PROGRAM:            strcat(str, "CL_INVALID_PROGRAM"); break;
      case CL_INVALID_PROGRAM_EXECUTABLE: strcat(str, "CL_INVALID_PROGRAM_EXECUTABLE"); break;
      case CL_INVALID_KERNEL_NAME:        strcat(str, "CL_INVALID_KERNEL_NAME"); break;
      case CL_INVALID_KERNEL_DEFINITION:  strcat(str, "CL_INVALID_KERNEL_DEFINITION"); break;
      case CL_INVALID_KERNEL:             strcat(str, "CL_INVALID_KERNEL"); break;
      case CL_INVALID_ARG_INDEX:          strcat(str, "CL_INVALID_ARG_INDEX"); break;
      case CL_INVALID_ARG_VALUE:          strcat(str, "CL_INVALID_ARG_VALUE"); break;
      case CL_INVALID_ARG_SIZE:           strcat(str, "CL_INVALID_ARG_SIZE"); break;
      case CL_INVALID_KERNEL_ARGS:        strcat(str, "CL_INVALID_KERNEL_ARGS"); break;
      case CL_INVALID_WORK_DIMENSION:     strcat(str, "CL_INVALID_WORK_DIMENSION"); break;
      case CL_INVALID_WORK_GROUP_SIZE:    strcat(str, "CL_INVALID_WORK_GROUP_SIZE"); break;
      case CL_INVALID_WORK_ITEM_SIZE:     strcat(str, "CL_INVALID_WORK_ITEM_SIZE"); break;
      case CL_INVALID_GLOBAL_OFFSET:      strcat(str, "CL_INVALID_GLOBAL_OFFSET"); break;
      case CL_INVALID_EVENT_WAIT_LIST:    strcat(str, "CL_INVALID_EVENT_WAIT_LIST"); break;
      case CL_INVALID_EVENT:              strcat(str, "CL_INVALID_EVENT"); break;
      case CL_INVALID_OPERATION:          strcat(str, "CL_INVALID_OPERATION"); break;
      case CL_INVALID_GL_OBJECT:          strcat(str, "CL_INVALID_GL_OBJECT"); break;
      case CL_INVALID_BUFFER_SIZE:        strcat(str, "CL_INVALID_BUFFER_SIZE"); break;
      case CL_INVALID_MIP_LEVEL:          strcat(str, "CL_INVALID_MIP_LEVEL"); break;
      case CL_INVALID_GLOBAL_WORK_SIZE:   strcat(str, "CL_INVALID_GLOBAL_WORK_SIZE"); break;
      //case CL_INVALID_PROPERTY:           strcat(str, "CL_INVALID_PROPERTY"); break;
      // 64+ go here
      default: strcat(str, "UNKNOWN ERROR CODE");
   }
   return;
}

/**
 * clCheckErr function
 *
 * \brief This function checks, analyzes, and diagnoses OpenCL error codes
 *
 * \param err the error code to check
 * \param line the line number where the error code is being checked
 * \param file the name of the file in which the error code was generated
 *
 * \return None
 *
 */
extern "C" void clCheckErr(cl_int err, int line, char * file) {
   char errStr[STRLEN];
   if (err != CL_SUCCESS) {
      clDecodeErr(err, errStr);
      printf("OpenCL Error %d: %s. In file %s, line %d\n", err, errStr, file, line);
#ifdef KILLONERR
      exit(err);
#endif
   }
   return;
}

/**
 * clDecodeFPConfig function
 *
 * \brief This function decodes the OpenCL floating point configuration type
 *
 * \param cfg Device FP configuration code
 * \param str  Output string to write to
 * \return None
 */
extern "C" void clDecodeFPConfig(cl_device_fp_config cfg, char * str) {
   int on = 0;
   str[0] = '\0';
   if (cfg & CL_FP_DENORM) { strcat(str, "DENORM"); on = 1; }
   if (cfg & CL_FP_INF_NAN) { if(on){strcat(str,", ");} strcat(str, "INF_NAN"); on = 1; }
   if (cfg & CL_FP_ROUND_TO_NEAREST) { if(on){strcat(str,", ");} strcat(str, "ROUND_TO_NEAREST"); on = 1; }
   if (cfg & CL_FP_ROUND_TO_ZERO) { if(on){strcat(str,", ");} strcat(str, "ROUND_TO_ZERO"); on = 1; }
   if (cfg & CL_FP_ROUND_TO_INF) { if(on){strcat(str,", ");} strcat(str, "ROUND TO INF"); on = 1; }
   if (cfg & CL_FP_FMA) { if(on){strcat(str,", ");} strcat(str, "FMA"); on = 1; }
   if (cfg & CL_FP_SOFT_FLOAT) { if(on){strcat(str,", ");} strcat(str, "SOFT FLOAT"); }
   return;
}

/**
 * clDecodeExecCapabilities function
 *
 * \brief This function decodes the OpenCL device execution capabilities type
 *
 * \param dcap  Device execution capabilities code
 * \param str  Output string to write to
 * \return None
 */
extern "C" void clDecodeExecCapabilities(cl_device_exec_capabilities dcap, char * str) {
   int on = 0;
   str[0] = '\0';
   if (dcap & CL_EXEC_KERNEL) { strcat(str, "KERNEL"); on = 1; }
   if (dcap & CL_EXEC_NATIVE_KERNEL) { if(on){strcat(str,", ");} strcat(str, "NATIVE_KERNEL"); }
   return;
}

/**
 * clDecodeDevMemCacheType function
 *
 * \brief This function decodes the OpenCL device memory cache type
 *
 * \param mctype  Device memory cache type code
 * \param str  Output string to write to
 * \return None
 */
extern "C" void clDecodeDevMemCacheType(cl_device_mem_cache_type mctype, char * str) {
   str[0] = '\0';
   switch(mctype) {
      case CL_NONE: strcat(str, "NONE"); break;
      case CL_READ_ONLY_CACHE: strcat(str, "READ_ONLY_CACHE"); break;
      case CL_READ_WRITE_CACHE: strcat(str, "READ_WRITE_CACHE"); break;
      default: strcat(str, "ERROR: Invalid device memory cache type");
   }
   return;
}

/**
 * clDecodeLocalMemType function
 *
 * \brief This function decodes the OpenCL device local memory type
 *
 * \param lmtype  Device local memory type code
 * \param str  Output string to write to
 * \return None
 */
extern "C" void clDecodeLocalMemType(cl_device_local_mem_type lmtype, char * str) {
   str[0] = '\0';
   switch(lmtype) {
      case CL_LOCAL: strcat(str, "LOCAL"); break;
      case CL_GLOBAL: strcat(str, "GLOBAL"); break;
      default: strcat(str, "ERROR: Invalid device local memory type");
   }
   return;
}

/**
 * clDecodeQueueProperties function
 *
 * \brief This function decodes the OpenCL command queue properties type
 *
 * \param qprop  Command queue properties code
 * \param str  Output string to write to
 * \return None
 */
extern "C" void clDecodeQueueProperties(cl_command_queue_properties qprop, char * str) {
   int on = 0;
   str[0] = '\0';
   if (qprop & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) { strcat(str, "OUT_OF_ORDER_EXEC_ENABLE"); on = 1; }
   if (qprop & CL_QUEUE_PROFILING_ENABLE) { if(on){strcat(str,", ");} strcat(str, "PROFILING_ENABLE"); }
   return;
}

/**
 * clDecodeDeviceType function
 *
 * \brief This function decodes the OpenCL device type
 *
 * \param dtype Device type code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeDeviceType(cl_device_type dtype, char * str) {
   int on = 0;
   str[0] = '\0';
   if (dtype & CL_DEVICE_TYPE_DEFAULT) { strcat(str, "DEFAULT"); on = 1; }
   if (dtype & CL_DEVICE_TYPE_CPU) { if(on){strcat(str,", ");} strcat(str, "CPU"); on = 1; }
   if (dtype & CL_DEVICE_TYPE_GPU) { if(on){strcat(str,", ");} strcat(str, "GPU");  on = 1; }
   if (dtype & CL_DEVICE_TYPE_ACCELERATOR) { if(on){strcat(str,", ");} strcat(str, "ACCELERATOR");  on = 1; }
   if (dtype & CL_DEVICE_TYPE_ALL) { if(on){strcat(str,", ");} strcat(str, "ALL"); }
   return;
}

/**
 * clDecodeMemObjType function
 *
 * \brief This function decodes the OpenCL memory object type
 *
 * \param mtype Memory object type code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeMemObjType(cl_mem_object_type mtype, char * str) {
   str[0] = '\0';
   switch(mtype) {
      case CL_MEM_OBJECT_BUFFER: strcat(str, "CL_MEM_OBJECT_BUFFER"); break;
      case CL_MEM_OBJECT_IMAGE2D: strcat(str, "CL_MEM_OBJECT_IMAGE2D"); break;
      case CL_MEM_OBJECT_IMAGE3D: strcat(str, "CL_MEM_OBJECT_IMAGE3D"); break;
      default: strcat(str, "ERROR: Invalid Memory Object Type");
   }
   return;
}

/**
 * clDecodeMemObjFlags function
 *
 * \brief This function decodes the OpenCL memory flag type
 *
 * \param mflags Memory object flags code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeMemObjFlags(cl_mem_flags mflags, char * str) {
   int on = 0;
   str[0] = '\0';
   if (mflags & CL_MEM_READ_WRITE) { strcat(str, "CL_MEM_READ_WRITE "); on = 1; }
   if (mflags & CL_MEM_WRITE_ONLY) { if(on){strcat(str,", ");} strcat(str, "CL_MEM_WRITE_ONLY "); on = 1; }
   if (mflags & CL_MEM_READ_ONLY) { if(on){strcat(str,", ");} strcat(str, "CL_MEM_READ_ONLY "); on = 1; }
   if (mflags & CL_MEM_USE_HOST_PTR) { if(on){strcat(str,", ");} strcat(str, "CL_MEM_USE_HOST_PTR "); on = 1; }
   if (mflags & CL_MEM_ALLOC_HOST_PTR) { if(on){strcat(str,", ");} strcat(str, "CL_MEM_ALLOC_HOST_PTR "); on = 1; }
   if (mflags & CL_MEM_COPY_HOST_PTR) { if(on){strcat(str,", ");} strcat(str, "CL_MEM_COPY_HOST_PTR "); }
   return;
}

/**
 * clDecodeImageFormat function
 *
 * \brief This function decodes the OpenCL image format type
 *
 * \param imgfmt Image format code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeImageFormat(cl_image_format imgfmt, char * str) {
   str[0] = '\0';
   strcat(str, "\n   Channel Order              ");
   switch (imgfmt.image_channel_order) {
      case CL_R : strcat(str, "CL_R"); break;
      case CL_A : strcat(str, "CL_A"); break;
      case CL_RG : strcat(str, "CL_RG"); break;
      case CL_RA : strcat(str, "CL_RA"); break;
      case CL_RGB : strcat(str, "CL_RGB"); break;
      case CL_RGBA : strcat(str, "CL_RGBA"); break;
      case CL_BGRA : strcat(str, "CL_BGRA"); break;
      case CL_ARGB : strcat(str, "CL_ARGB"); break;
      case CL_INTENSITY : strcat(str, "CL_INTENSITY"); break;
      case CL_LUMINANCE : strcat(str, "CL_LUMINANCE"); break;
      default: strcat(str, "ERROR: Invalid Color Order");
   }
   strcat(str, "\n   Channel Data Type          ");
   switch (imgfmt.image_channel_data_type) {
      case CL_SNORM_INT8 : strcat(str, "CL_SNORM_INT8"); break;
      case CL_SNORM_INT16 : strcat(str, "CL_SNORM_INT16"); break;
      case CL_UNORM_INT8 : strcat(str, "CL_UNORM_INT8"); break;
      case CL_UNORM_INT16 : strcat(str, "CL_UNORM_INT16"); break;
      case CL_UNORM_SHORT_565 : strcat(str, "CL_UNORM_SHORT_565"); break;
      case CL_UNORM_SHORT_555 : strcat(str, "CL_UNORM_SHORT_555"); break;
      case CL_UNORM_INT_101010 : strcat(str, "CL_UNORM_INT_101010"); break;
      case CL_SIGNED_INT8 : strcat(str, "CL_SIGNED_INT8"); break;
      case CL_SIGNED_INT16 : strcat(str, "CL_SIGNED_INT16"); break;
      case CL_SIGNED_INT32 : strcat(str, "CL_SIGNED_INT32"); break;
      case CL_UNSIGNED_INT8 : strcat(str, "CL_UNSIGNED_INT8"); break;
      case CL_UNSIGNED_INT16 : strcat(str, "CL_UNSIGNED_INT16"); break;
      case CL_UNSIGNED_INT32 : strcat(str, "CL_UNSIGNED_INT32"); break;
      case CL_HALF_FLOAT : strcat(str, "CL_HALF_FLOAT"); break;
      case CL_FLOAT : strcat(str, "CL_FLOAT"); break;
      default: strcat(str, "ERROR: Invalid Data Type");
   }
   return;
}

/**
 * clDecodeAddressingMode function
 *
 * \brief This function decodes the OpenCL addressing mode type
 *
 * \param amd Addressing mode code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeAddressingMode(cl_addressing_mode amd, char * str) {
   str[0] = '\0';
   switch (amd) {
      case CL_ADDRESS_NONE : strcat(str, "CL_ADDRESS_NONE"); break;
      case CL_ADDRESS_CLAMP_TO_EDGE : strcat(str, "CL_ADDRESS_CLAMP_TO_EDGE"); break;
      case CL_ADDRESS_CLAMP : strcat(str, "CL_ADDRESS_CLAMP"); break;
      case CL_ADDRESS_REPEAT : strcat(str, "CL_ADDRESS_REPEAT"); break;
      default: strcat(str, "ERROR: Invalid Addressing Mode");
   }
   return;
}

/**
 * clDecodeFilterMode function
 *
 * \brief This function decodes the OpenCL filter mode type
 *
 * \param fmd Filter mode code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeFilterMode(cl_filter_mode fmd, char * str) {
   str[0] = '\0';
   switch (fmd) {
      case CL_FILTER_NEAREST : strcat(str, "CL_FILTER_NEAREST"); break;
      case CL_FILTER_LINEAR : strcat(str, "CL_FILTER_LINEAR"); break;
      default: strcat(str, "ERROR: Invalid Filter Mode");
   }
   return;
}

/**
 * clDecodeBuildStatus function
 *
 * \brief This function decodes the OpenCL build status type
 *
 * \param bst Build status code
 * \param str  Output string to write to
 * \return None
 *
 */
extern "C" void clDecodeBuildStatus(cl_build_status bst, char * str) {
   str[0] = '\0';
   switch (bst) {
      case CL_BUILD_SUCCESS : strcat(str, "CL_BUILD_SUCCESS"); break;
      case CL_BUILD_NONE : strcat(str, "CL_BUILD_NONE"); break;
      case CL_BUILD_ERROR : strcat(str, "CL_BUILD_ERROR"); break;
      case CL_BUILD_IN_PROGRESS : strcat(str, "CL_BUILD_IN_PROGRESS");
   }
   return;
}

/**
 * clPlatformDump function
 *
 * \brief This function prints out all information about the given platform ID
 *
 * \param pid Platform id pointer
 * \return None
 *
 */
extern "C" void clPlatformDump(cl_platform_id pid) {
   printf("Printing information for platform ID:0x%X\n", pid);
   
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_PROFILE, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_PROFILE, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tProfile:        %s\n", pstr);
   free(pstr);
   
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VERSION, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tVersion:        %s\n", pstr);
   free(pstr);
   
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_NAME, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tName:           %s\n", pstr);
   free(pstr);
   
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VENDOR, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VENDOR, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tVendor:         %s\n", pstr);
   free(pstr);
   
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_EXTENSIONS, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_EXTENSIONS, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tExtensions:     %s\n", pstr);
   free(pstr);

   printf("\n");
   return;
}

/**
 * clDeviceDump function
 *
 * \brief This function prints out all information about the given device ID 
 *
 * \param did Device id
 * \return None
 *
 */
extern "C" void clDeviceDump(cl_device_id did) {
   size_t * pdims;
   int idx = 0;
   cl_device_fp_config fpcfg;
   cl_device_type dtype;
   cl_device_exec_capabilities dcap;
   cl_device_mem_cache_type mctype;
   cl_device_local_mem_type lmtype;
   cl_platform_id dplat;
   cl_command_queue_properties dqprop;

   printf("\nPrinting information for device ID:0x%X\n", did);
   printf("---------------------------------------------------------------\n");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_VENDOR_ID, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_VENDOR, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_VENDOR, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tVendor:     %s (ID: 0x%X)\n", pstr, puint);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tName:       %s", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_TYPE, sizeof(cl_device_type), &dtype, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeDeviceType(dtype, pstr);
   printf(" (%s) ", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_VERSION, psize, pstr, NULL),__LINE__,__FILE__);
   printf("ver. %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DRIVER_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DRIVER_VERSION, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tDriver:     ver. %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &dplat, NULL),__LINE__,__FILE__);
   printf("\tPlatform:   0x%X\n", dplat);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_OPENCL_C_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_OPENCL_C_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
   printf("\tOpenCL C:   ver. %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tAvailable:  %s\n", (pbool) ? "Yes" : "No");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tCompiler:   %s\n", (pbool) ? "Yes" : "No");   
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(cl_device_exec_capabilities), &dcap, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeExecCapabilities(dcap,pstr);
   printf("\tExecution:  %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_EXTENSIONS, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_EXTENSIONS, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tExtensions: %s\n", pstr);
   free(pstr);

   printf("\nDevice compute capabilities\n");
   printf("---------------------------------------------------------------\n");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PROFILE, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PROFILE, psize, pstr, NULL),__LINE__,__FILE__);
   printf("\tDevice Profile:    %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("\tTimer Resolution:  %dns\n", psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_QUEUE_PROPERTIES, sizeof(cl_command_queue_properties), &dqprop, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeQueueProperties(dqprop, pstr);
   printf("\tCommand Queue:     %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tClock Frequency:   %dMHz (max)\n", puint);   
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tCompute Units:     %d (max)\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("\tWork Group Size:   %d (max)\n", psize);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   pdims = (size_t*)malloc(sizeof(size_t)*puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*puint, pdims, NULL),__LINE__,__FILE__);
   printf("\tWork Item Sizes:   [%d", pdims[0]);
   for (idx = 1; idx < puint; idx++) { printf(", %d",pdims[idx]); }
   printf("] (max)\n");
   free(pdims);

   printf("\nDevice Memory Information\n");
   printf("---------------------------------------------------------------\n");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tAddress Bits:                 %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tLittle Endian:                %s\n", (pbool) ? "Yes" : "No");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tMemory Base Addr Align:       %d bits\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tError Correction:             %s\n", (pbool) ? "Yes" : "No");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tDev/Host Unified Memory:      %s\n", (pbool) ? "Yes" : "No");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &pulong, NULL),__LINE__,__FILE__);
   printf("\tGlobal Memory Size:           %luB\n", pulong);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &pulong, NULL),__LINE__,__FILE__);
   printf("\tGlobal Memory Cache:          %luB\n", pulong);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(cl_device_mem_cache_type), &mctype, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeDevMemCacheType(mctype,pstr);
   printf("\tGlobal Memory Cache Type:     %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tGlobal Memory Cacheline Size: %dB\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &pulong, NULL),__LINE__,__FILE__);
   printf("\tLocal Memory Size:            %dB\n", pulong);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(cl_device_local_mem_type), &lmtype, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeLocalMemType(lmtype, pstr);
   printf("\tLocal Memory Type:            %s\n", pstr);
   free(pstr);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tImage Support:                %s\n", (pbool) ? "Yes" : "No");
   if (pbool) {
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
      printf("\t\tRead Image Args:     %d (max)\n", puint);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
      printf("\t\tWrite Image Args:    %d (max)\n", puint);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_SAMPLERS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
      printf("\t\tSamplers:            %d (max)\b", puint);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
      printf("\t\tImage 2D Height:     %d (max)\n", psize);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
      printf("\t\tImage 2D Width:      %d (max)\n", psize);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
      printf("\t\tImage 3D Depth:      %d (max)\n", psize);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
      printf("\t\tImage 3D Height:     %d (max)\n", psize);
      clCheckErr(clGetDeviceInfo(did, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
      printf("\t\tImage 3D Width:      %d (max)\n", psize);
   }
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Constant Args:                  %d (max)\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &pulong, NULL),__LINE__,__FILE__);
   printf("Constant Buffer Size            %dB (max)\n", pulong);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &pulong, NULL),__LINE__,__FILE__);
   printf("Memory Allocation Size:         %dB (max)\n", pulong);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Parameter Size:                 %dB (max)\n", psize);

   printf("\nDevice data formatting\n");
   printf("---------------------------------------------------------------\n");
#ifdef OPENCL_1.2
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cl_device_fp_config), &fpcfg, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeFPConfig(fpcfg,pstr);
   printf("\tDouble FP Config:      %s\n", pstr);
   free(pstr);
#endif
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_SINGLE_FP_CONFIG, sizeof(cl_device_fp_config), &fpcfg, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeFPConfig(fpcfg,pstr);
   printf("\tSingle FP Config:      %s\n", pstr);
   free(pstr);
#ifdef OPENCL_1.2
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_HALF_FP_CONFIG, sizeof(cl_device_fp_config), &fpcfg, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeFPConfig(fpcfg,pstr);
   printf("\tHalf FP Config:        %s\n", pstr);
   free(pstr);
#endif
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("\tDevice Error Correction Support (ECS): %s\n", (pbool) ? "Yes" : "No");
   printf("\tDevice Native Vector Widths\n");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tCHAR:      %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tSHORT:     %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tINT:       %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tLONG:      %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tFLOAT:     %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tDOUBLE:    %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tHALF:      %d\n", puint);
   printf("\tDevice Preferred Vector Widths\n");
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tCHAR:      %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tSHORT:     %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tINT:       %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tLONG:      %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tFLOAT:     %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tDOUBLE:    %d\n", puint);
   clCheckErr(clGetDeviceInfo(did, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\t\tHALF:      %d\n", puint);
   printf("\n");
   return;
}

/**
 * clContextDump function
 *
 * \brief This function prints out all of the information about the given context
 *
 * \param cid Context id
 * \return None
 *
 */
extern "C" void clContextDump(cl_context cid) {
   int idx = 0;
   cl_device_id * cdevs;
   cl_context_properties cprops[3] = {0, 0, 0};

   printf("Printing information for context ID:0x%X\n", cid);
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tReference Count:   %d\n", puint);
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("\tDevice Count:      %d\n", puint);
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_DEVICES, NULL, NULL, &psize),__LINE__,__FILE__);
   cdevs = (cl_device_id*)malloc(psize);
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_DEVICES, psize, cdevs, NULL),__LINE__,__FILE__);
   printf("\tDevices:           ");
   for (idx = 0; idx < puint; idx++) {
      if (idx > 0) { printf(", "); }
      printf("0x%X", cdevs[idx]);
   }
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_PROPERTIES, NULL, NULL, &psize),__LINE__,__FILE__);
   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_PROPERTIES, psize, cprops, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeQueueProperties(*cprops, pstr);
   printf("\tProperties:        %s\n", pstr);
   free(pstr);
#ifdef OPENCL_1.2
   //   clCheckErr(clGetContextInfo(cid, CL_CONTEXT_D3D10_PREFER_SHARED_RESOURCES_KHR, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
#endif
   pbool = false;
   printf("\tD3D10 Support:     %s\n", (pbool) ? "Yes" : "No");
   printf("\n");
   return;
}

/**
 * clMemObjDump function
 *
 * \brief This function prints out all of the information about the given memory object
 *
 * \param mobj Memory object
 * \return None
 *
 */
extern "C" void clMemObjDump(cl_mem mobj) {
   cl_mem_object_type   mtype;
   cl_mem_flags mflags;
   cl_context  ctx;
   void * vptr;

   printf("Printing information for memory object ID:0x%X\n", mobj);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_TYPE, sizeof(cl_mem_object_type), &mtype, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeMemObjType(mtype, pstr);
   printf("Type              %s\n", pstr);
   free(pstr);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_FLAGS, sizeof(cl_mem_flags), &mflags, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeMemObjFlags(mflags, pstr);
   printf("Flags             %s\n", pstr);
   free(pstr);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_SIZE, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Size              %sB\n", psize);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_HOST_PTR, sizeof(void*), &vptr, NULL),__LINE__,__FILE__);
   printf("Host Ptr          0x%X\n", vptr);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_MAP_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Map Count         %d\n", puint);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_REFERENCE_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Reference Count   %d\n", puint);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_CONTEXT, sizeof(cl_context), &ctx, NULL),__LINE__,__FILE__);
   printf("Context           0x%X\n", &ctx);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_ASSOCIATED_MEMOBJECT, sizeof(cl_mem), &mobj, NULL),__LINE__,__FILE__);
   printf("Assoc. Mem Obj    0x%X\n", &mobj);
   clCheckErr(clGetMemObjectInfo(mobj, CL_MEM_OFFSET, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Offset            %d\n", psize);
   printf("\n");
   return;
}

/**
 * clImgObjDump function
 *
 * \brief This function prints all of the information about the given image object
 *
 * \param img Image object
 * \return None
 *
 */
extern "C" void clImgObjDump(cl_mem img) {
   cl_image_format fmt;

   printf("Printing information for image object ID:0x%X\n", img);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_FORMAT, sizeof(cl_image_format), &fmt, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeImageFormat(fmt, pstr);
   printf("%s\n",pstr);
   free(pstr);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_ELEMENT_SIZE, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Element Size                  %d\n", psize);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_ROW_PITCH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Row Pitch                     %d\n", psize);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_SLICE_PITCH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Slice Pitch                   %d\n", psize);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_WIDTH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Width                         %d\n", psize);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_HEIGHT, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Height                        %d\n", psize);
   clCheckErr(clGetImageInfo(img, CL_IMAGE_DEPTH, sizeof(size_t), &psize, NULL),__LINE__,__FILE__);
   printf("Depth                         %d\n", psize);
   printf("\n");
   return;
}

/**
 * clSamplerDump function
 *
 * \brief This function prints out all of the information about the sampler object
 *
 * \param smp Sampler object
 * \return None
 *
 */
extern "C" void clSamplerDump(cl_sampler smp) {
   cl_context ctx;
   cl_addressing_mode amd;
   cl_filter_mode fmd;

   printf("Printing information for sampler ID:0x%X\n", smp);
   clCheckErr(clGetSamplerInfo(smp, CL_SAMPLER_REFERENCE_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Reference Count            %d\n", puint);
   clCheckErr(clGetSamplerInfo(smp, CL_SAMPLER_CONTEXT, sizeof(cl_context), &ctx, NULL),__LINE__,__FILE__);
   printf("Context                    0x%X\n", ctx);
   clCheckErr(clGetSamplerInfo(smp, CL_SAMPLER_ADDRESSING_MODE, sizeof(cl_addressing_mode), &amd, NULL),__LINE__,__FILE__); 
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeAddressingMode(amd, pstr);
   printf("Addressing Mode            %s\n", pstr);
   free(pstr);
   clCheckErr(clGetSamplerInfo(smp, CL_SAMPLER_FILTER_MODE, sizeof(cl_filter_mode), &fmd, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeFilterMode(fmd, pstr);
   printf("Filter Mode                %s\n", pstr);
   free(pstr);
   clCheckErr(clGetSamplerInfo(smp, CL_SAMPLER_NORMALIZED_COORDS, sizeof(cl_bool), &pbool, NULL),__LINE__,__FILE__);
   printf("Normalized Coordinates     %s\n", (pbool) ? "Yes" : "No");
   printf("\n");
   return;
}

/**
 * clProgramDump function
 *
 * \brief This function prints out all of the information about the given program object
 *
 * \param pid Program id
 * \return None
 *
 */
extern "C" void clProgamDump(cl_program pid) {
   int jdx, kdx = 0;
   cl_context ctx;
   cl_device_type dtp;
   size_t * psizes = NULL;
   unsigned char ** bins = NULL;
   unsigned char * bin = NULL;
   cl_device_id * dptr = NULL;

   printf("Printing information for program ID:0x%X\n", pid);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_REFERENCE_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Reference Count         %d\n", puint);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_CONTEXT, sizeof(cl_context), &ctx, NULL),__LINE__,__FILE__);
   printf("Context                 0x%d\n", ctx);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_SOURCE, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_SOURCE, psize, pstr, NULL),__LINE__,__FILE__);
   printf("Program Source\n");
   printf("%s\n", pstr);
   free(pstr);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Number of Devices                %d\n", puint);
   // Program devices
   psize = sizeof(cl_device_id)*puint;
   dptr = (cl_device_id*)malloc(psize);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_DEVICES, psize, dptr, NULL),__LINE__,__FILE__);
   // Program binary sizes
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_BINARY_SIZES, NULL, NULL, &psize),__LINE__,__FILE__);
   psizes = (size_t*)malloc(psize);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_BINARY_SIZES, psize, psizes, NULL),__LINE__,__FILE__);
   psize = sizeof(unsigned char*) * puint;
   bins = (unsigned char**)malloc(psize);
   clCheckErr(clGetProgramInfo(pid, CL_PROGRAM_BINARIES, psize, bins, NULL),__LINE__,__FILE__);
   printf("Devices\n");
   for (jdx = 0; jdx < puint; jdx++) {
      printf("\t%02d) ", jdx);
      // Device name
      clCheckErr(clGetDeviceInfo(dptr[jdx], CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetDeviceInfo(dptr[jdx], CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
      printf("%s ", pstr);
      free(pstr);
      // Device type
      clCheckErr(clGetDeviceInfo(dptr[jdx], CL_DEVICE_TYPE, sizeof(cl_device_type), &dtp, NULL),__LINE__,__FILE__);
      pstr = (char*)malloc(sizeof(char)*STRLEN);
      clDecodeDeviceType(dtp, pstr);
      printf("(%s)\n", pstr);
      free(pstr);
      // Device Binary
      bin = bins[jdx];
      for (kdx = 0; kdx < psizes[jdx]; kdx++) {
         printf("%02X", bin[kdx]);
         if ((kdx % 4) == 3) { printf(" "); }
         if ((kdx % 16) == 15) { printf("\n"); }
      }
      printf("\n");
   }
   free(bins);
   free(psizes);
   free(dptr);
   return;
}

/**
 * clProgramBuildDump function
 *
 * \brief This function prints out all of the information about the given program
 *        object's build for the specified device
 *
 * \param prg Program id
 * \param dev Device id
 * \return None
 *
 */
extern "C" void clProgramBuildDump(cl_program prg, cl_device_id dev) {
   cl_build_status bst;
   cl_device_type dtp;

   printf("Printing information for Program ID:0x%X Device ID:0x%X\n", prg, dev);
   // Device name
   clCheckErr(clGetDeviceInfo(dev, CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetDeviceInfo(dev, CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
   printf("%s ", pstr);
   free(pstr);
   // Device type
   clCheckErr(clGetDeviceInfo(dev, CL_DEVICE_TYPE, sizeof(cl_device_type), &dtp, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeDeviceType(dtp, pstr);
   printf("(%s)\n", pstr);
   free(pstr);
   // Program build status
   clCheckErr(clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &bst, NULL),__LINE__,__FILE__);
   pstr = (char*)malloc(sizeof(char)*STRLEN);
   clDecodeBuildStatus(bst, pstr);
   printf("Build Status            %s\n", pstr);
   free(pstr);
   // Program build options
   clCheckErr(clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_OPTIONS, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_OPTIONS, psize, pstr, NULL),__LINE__,__FILE__);
   printf("Build Options           %s\n", pstr);
   free(pstr);
   clCheckErr(clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG, psize, pstr, NULL),__LINE__,__FILE__);
   printf("Build Log\n%s", pstr);
   return;
}

/**
 * clKernelDump function
 *
 * \brief This function prints out all of the information for the given kernel object
 *
 * \param krn Kernel object
 * \return None
 *
 */
extern "C" void clKernelDump(cl_kernel krn) {
   printf("Printing information for Kernel ID:0x%X\n", krn);
   // Function name
   clCheckErr(clGetKernelInfo(krn, CL_KERNEL_FUNCTION_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
   pstr = (char*)malloc(psize);
   clCheckErr(clGetKernelInfo(krn, CL_KERNEL_FUNCTION_NAME, psize, pstr, NULL),__LINE__,__FILE__);
   printf("Function Name: %s\n", pstr);
   free(pstr);
   // Kernel Number of Arguments
   clCheckErr(clGetKernelInfo(krn, CL_KERNEL_NUM_ARGS, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Num Args: %d\n", puint);
   // Kernel reference count
   clCheckErr(clGetKernelInfo(krn, CL_KERNEL_REFERENCE_COUNT, sizeof(cl_uint), &puint, NULL),__LINE__,__FILE__);
   printf("Ref Cnt: %d\n", puint);
   return;
}

/**
 * clPrintPlats function
 *
 * \brief This function prints out a very short list of the platforms found on the system
 *
 * \return None
 *
 */
extern "C" void clPrintPlats() {
   cl_uint idx, np = 0;
	cl_platform_id * ps;
	cl_platform_id curplat;

   // Query and list platforms
   clCheckErr(clGetPlatformIDs(0, NULL, &np),__LINE__,__FILE__);
   ps = (cl_platform_id*)malloc(sizeof(cl_platform_id)*(np));
   clCheckErr(clGetPlatformIDs(np,ps,NULL),__LINE__,__FILE__);
   for (idx = 0; idx < np; idx++) {
   	curplat = ps[idx];
      printf("Platform %d: 0x%X ==> ", idx, curplat);
      clCheckErr(clGetPlatformInfo(curplat, CL_PLATFORM_VENDOR, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetPlatformInfo(curplat, CL_PLATFORM_VENDOR, psize, pstr, NULL),__LINE__,__FILE__);
      printf("%s ", pstr);
      free(pstr);
      clCheckErr(clGetPlatformInfo(curplat, CL_PLATFORM_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetPlatformInfo(curplat, CL_PLATFORM_NAME, psize, pstr, NULL),__LINE__,__FILE__);
      printf("%s\n", pstr);
      free(pstr);
   }
   free(ps);
   return;
}

/**
 * clPrintDevs function
 *
 * \brief This function prints out a very short list of the devices found on the specifed platform
 *
 * \param pid Platform ID
 * \return None
 *
 */
extern "C" void clPrintDevs(cl_platform_id pid) {
	cl_uint idx, nd = 0;
 	cl_device_id * ds;
 	cl_device_id currdev;
 	cl_device_type tmptype;
 
   // Query and list devices
   clCheckErr(clGetDeviceIDs(pid, CL_DEVICE_TYPE_ALL, NULL, NULL, &nd),__LINE__,__FILE__);
   ds = (cl_device_id*)malloc(sizeof(cl_device_id)*(nd));
   clCheckErr(clGetDeviceIDs(pid, CL_DEVICE_TYPE_ALL, nd, ds, NULL),__LINE__,__FILE__);
      
   for (idx = 0; idx < nd; idx++) {
      currdev = ds[idx];
      printf("\tDevice %d (ID:0x%X):", idx, currdev );
      clCheckErr(clGetDeviceInfo(currdev, CL_DEVICE_NAME, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetDeviceInfo(currdev, CL_DEVICE_NAME, psize, pstr, NULL),__LINE__,__FILE__);
      printf(" %s", pstr);
      free(pstr);
      clCheckErr(clGetDeviceInfo(currdev, CL_DEVICE_TYPE, sizeof(cl_device_type), &tmptype, NULL),__LINE__,__FILE__);
      pstr = (char*)malloc(sizeof(char)*STRLEN);
      clDecodeDeviceType(tmptype, pstr);
      printf(" (%s)", pstr);
      free(pstr);
      clCheckErr(clGetDeviceInfo(currdev, CL_DEVICE_VERSION, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetDeviceInfo(currdev, CL_DEVICE_VERSION, psize, pstr, NULL),__LINE__,__FILE__);
      printf(" ver. %s\n", pstr);
      free(pstr);
   }
	free(ds);
   return;
}

/**
 * clFindPlatform function
 *
 * \brief This function concatenates a platform description string from the given platform index
 *
 * \param plat Platform type
 * \param cmpStr Output string
 * \return None
 *
 */
extern "C" void clDecodeTaskPlat(int plat, char * cmpStr) {
   cmpStr[0] = '\0';
   switch(plat) {
      case 0: strcat(cmpStr, "NVIDIA"); break;
      case 1:    strcat(cmpStr, "AMD"); break;
      case 2:  strcat(cmpStr, "Intel"); break;
      default:
         printf("OCLUTIL ERROR: Invalid task platform\n");
         #ifdef KILLONERR
         exit(-559); 
         #endif
   }
   return;
}

/**
 * clFindPlatform function
 *
 * \brief This function returns the system platform index from the given platform type input
 *
 * \param plat Platform type
 * \return int Platform index
 *
 */
extern "C" int clFindPlatform(int plat) {
   cl_uint nplat = 0;
   clCheckErr(clGetPlatformIDs(0, NULL, &nplat),__LINE__,__FILE__);
   cl_platform_id * plats = (cl_platform_id*)malloc(sizeof(cl_platform_id)*nplat);
   clCheckErr(clGetPlatformIDs(nplat,plats,NULL),__LINE__,__FILE__);
   char cmpStr[STRLEN];
   clDecodeTaskPlat(plat, cmpStr);

   // For each platform, find and list the devices
   for (int idx = 0; idx < nplat; idx++) {
      cl_platform_id pid = plats[idx];
      clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VENDOR, NULL, NULL, &psize),__LINE__,__FILE__);
      pstr = (char*)malloc(psize);
      clCheckErr(clGetPlatformInfo(pid, CL_PLATFORM_VENDOR, psize, pstr, NULL),__LINE__,__FILE__);
      if (strncmp(cmpStr,pstr,6)) { return idx; }
      free(pstr);
   }
   return -1;
}

/**
 * clFindKernel function
 *
 * \brief This function returns the OpenCL kernel object specified by the kernel name in the
 *        specified list of kernels 
 *
 * \param krns List of kernels
 * \param nrkn Number of kernels in list, krns
 * \param nrkName String of the kernel name to find
 * \return cl_kernel Found OpenCL kernel object
 *
 */
extern "C" cl_kernel * clFindKernel(cl_kernel * krns, int nkrn, const char * krnName) {
   size_t nameSz = 0;
   char * name = NULL;
   for (int idx = 0; idx < nkrn; idx++) {
      clCheckErr(clGetKernelInfo(krns[idx], CL_KERNEL_FUNCTION_NAME, NULL, NULL, &nameSz),__LINE__,__FILE__);
      name = (char*)malloc(nameSz);
      clCheckErr(clGetKernelInfo(krns[idx], CL_KERNEL_FUNCTION_NAME, nameSz, name, NULL),__LINE__,__FILE__);
      if (!strcmp(name,krnName)) {
         free(name);
         return &(krns[idx]);
      }
      free(name);
   }
   printf("OCLUTIL ERROR: No valid kernel found in file:%s at line:%d\n",__FILE__,__LINE__); exit(-560);
   return NULL;
}

