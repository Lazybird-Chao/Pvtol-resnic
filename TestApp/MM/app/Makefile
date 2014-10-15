
################################################################################
#
#
#
################################################################################

##need to be modified to the right path !!!!!
PVTOL_PATH := /home/chao/Desktop/Pvtol-resnic
PVTOLBASE := $(PVTOL_PATH)/pvtol-code
PLATFORMS := $(PVTOL_PATH)/platforms
PVTOL_LIBDIR := $(PVTOL_PATH)/lib

##need to be modified to the right path !!!!!!
CUDA_INSTALL_PATH := /usr/local/cuda-5.5


LIBNAME := libCoreApps.a


##the icludes, libs and cflags may be modefied by needs  !!!!!!
NVCC := $(CUDA_INSTALL_PATH)/bin/nvcc -Xcompiler -fPIC 
LINK := $(CUDA_INSTALL_PATH)/bin/nvcc -Xlinker -fPIC -shared 
CFLAGS := -g -G -O0 -w -use_fast_math -arch=sm_11 
INCLUDES  := -I$(CUDA_INSTALL_PATH)/include -I${PVTOLBASE}/include/base  
LIBDIRS := -L$(CUDA_INSTALL_PATH)/lib64
#LIBS := -lcudart -lcublas -lcufft -lOpenCL


##need to be modified by needs, the user's own kernel file  !!!!!!!
kernel_SRCS := 
kernel_OBJS := 

util_OBJS := $(PVTOL_LIBDIR)/cpuUtil.o $(PVTOL_LIBDIR)/cpuUtil_kl.o $(PVTOL_LIBDIR)/cudaUtil.o \
			$(PVTOL_LIBDIR)/cudaUtil_kl.o $(PVTOL_LIBDIR)/oclHelpers.o $(PVTOL_LIBDIR)/oclUtil.o

default: core

core: ${kernel_OBJS} $(util_OBJS)
	$(LINK) -o ${LIBNAME} ${kernel_OBJS} $(util_OBJS) $(LIBDIRS) ${LIBS}
	

${kernel_OBJS}: ${kernel_SRCS}
	$(NVCC) $(INCLUDES) $(CFLAGS) -c ${kernel_SRCS}
	
clean:
	rm *.a;
	rm *.o;

