################################################################################
#
#  Copyright (c) 2009, Massachusetts Institute of Technology
#  All rights reserved.
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This makefile builds the demonstrations
#
#  Author: Hahn Kim
#
#  $Id$
#

# Determine the architecture of the host which is running 'make'
# Also find a cmake path to use
ARCH=$(shell uname -m)
ifndef HOSTARCH
SYSID=$(shell uname -sm)
ifeq ($(SYSID),Linux i686)
HOSTARCH=$(ARCH)
CMAKE_DIR=$(shell if [ -e ~/tools/cmake-2.6.4-Linux-i386/bin/cmake ] ; \
                  then echo "~/tools/cmake-2.6.4-Linux-i386/bin/"; \
                  elif [ -e /tools/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.8/bin/"; \
                  elif [ -e /tools/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.7/bin/"; \
                  elif [ -e /usr/local/bin/cmake ] ; \
                  then echo "/usr/local/bin/"; fi)
endif
ifeq ($(SYSID),Linux x86_64)
HOSTARCH=$(ARCH)
CMAKE_DIR=$(shell if [ -e ~/tools/cmake-2.6.4-Linux-i386/bin/cmake ] ; \
                  then echo "~/tools/cmake-2.6.4-Linux-i386/bin/"; \
                  elif [ -e /tools/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.8/bin/"; \
                  elif [ -e /tools/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.7/bin/"; \
                  elif [ -e /usr/local/bin/cmake ] ; \
                  then echo "/usr/local/bin/"; fi)
endif
endif

CMAKE_COMMAND=$(CMAKE_DIR)cmake
CTEST_COMMAND=$(CMAKE_DIR)ctest

CMAKE_BUILD_DIR=./cmake_build

BIN_DIR := ./bin
kernelAPP := ./app

default:
	cd ${kernelAPP}; make; cd ..
	mkdir -p ${CMAKE_BUILD_DIR}
	$(CMAKE_COMMAND) -E chdir ${CMAKE_BUILD_DIR} $(CMAKE_COMMAND) ../
	make -C ${CMAKE_BUILD_DIR}
	mkdir -p ${BIN_DIR}; mv  ${CMAKE_BUILD_DIR}/src/*.run ${BIN_DIR}
	make clean

clean:
	rm -rf ${CMAKE_BUILD_DIR}

realclean:
	rm -rf ${CMAKE_BUILD_DIR}; rm -rf ${BIN_DIR};

