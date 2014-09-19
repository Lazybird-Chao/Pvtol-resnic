
################################################################################
#
#
#
################################################################################


LIBDIR := ../lib

#PVTOLBASE := where the pvtol source code is 
PVTOLBASE := /home/chao/Desktop/PVTOL/pvtol-code

CUDA_INSTALL_PATH := /usr/local/cuda-5.5

NVCC := $(CUDA_INSTALL_PATH)/bin/nvcc -Xcompiler -fPIC 
LINK := $(CUDA_INSTALL_PATH)/bin/nvcc -Xlinker -fPIC -shared 
CFLAGS := -g -G -O0 -w -use_fast_math -arch=sm_11 
INCLUDES  := -I$(CUDA_INSTALL_PATH)/include -I${PVTOLBASE}/include/base -I./util -I./klaunch


###
SRCS := ./util/cudaUtil.cu ./util/oclUtil.cc ./util/oclHelpers.cc ./util/cpuUtil.cc   ./klaunch/cpuUtil_kl.cc ./klaunch/cudaUtil_kl.cu
OBJS := cudaUtil.o oclUtil.o oclHelpers.o cpuUtil.o cpuUtil_kl.o cudaUtil_kl.o


#default:$(OBJS)

${OBJS}: ${SRCS}
	$(NVCC) $(INCLUDES) $(CFLAGS) -c ${SRCS};\
	mv -t $(LIBDIR) *.o;

clean:
	rm *.o

