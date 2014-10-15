################################################################################
#
#  Copyright (c) 2009, Massachusetts Institute of Technology
#  All rights reserved.
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This CMake file builds the FMT PVTOL application using the PVTOL API.
#
#  Author: James Brock
#
#  $Id$
#
#################################################################################


INCLUDE(../config.cmake)

MESSAGE(STATUS ${CMAKE_SOURCE_DIR})


# Set the directory containing PVTOL
# !!!!! CHANGE THIS TO YOUR SPECIFIC ENVIRONMENT !!!!!
SET(CUDA_DIR /usr/local/cuda-5.5)
#SET(CUDA_INSTALL_PREFIX ${CUDA_DIR})

#!!!!!MAY CHANGE
SET(PVTOLPATH /home/chao/Desktop/Pvtol-resnic)
SET(PVTOL_DIR ${PVTOLPATH}/pvtol-code)
SET(PLAT_DIR ${PVTOLPATH}/platforms)
SET(UTIL_DIR ${PLAT_DIR}/util)

SET(CXX_FLAGS_DEBUG ${CXX_FLAGS_DEBUG} -g)
SET(CXX_FLAGS ${CXX_FLAGS} "-g -w -O0")

# Set this variable to see the commands issued during builds
SET(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Echos all commands used during builds" FORCE)


# Set include and library paths for various third-party libraries.
# Use the same ones used by PVTOL
INCLUDE(${PVTOL_DIR}/util/cmake/findPaths/i686Default.cmake)
INCLUDE(${PVTOL_DIR}/util/cmake/findTools/FindBoost.cmake)
INCLUDE(${PVTOL_DIR}/util/cmake/findTools/FindMPI.cmake)
INCLUDE(${PVTOL_DIR}/util/cmake/findTools/FindVsipl.cmake)


# Use the MPI compilers
SET (CXX_COMPILER ${MPI_CXX} CACHE STRING "CXX Compiler" FORCE)
SET (C_COMPILER ${MPI_CC} CACHE STRING "C Compiler" FORCE)
SET (AR "ar" CACHE STRING "Archiver" FORCE)
SET (AR_ARGS "-cr" CACHE STRING "Archiver" FORCE)
SET (RANLIB "ranlib" CACHE STRING "Ranlib" FORCE)

SET(CMAKE_CXX_COMPILER ${CXX_COMPILER} CACHE INTERNAL "see CXX COMPILER" FORCE)
SET(CMAKE_CXX_FLAGS ${CXX_FLAGS} CACHE INTERNAL "see CXX COMPILER_FLAGS" FORCE)
SET(CMAKE_CXX_FLAGS_DEBUG ${CXX_FLAGS_DEBUG} CACHE INTERNAL "see CXX COMPILER_FLAGS_DEBUG" FORCE)
SET(CMAKE_CXX_FLAGS_RELEASE ${CXX_FLAGS_RELEASE} CACHE INTERNAL "see CXX COMPILER_FLAGS_RELEASE" FORCE)

SET(CMAKE_C_COMPILER ${C_COMPILER} CACHE INTERNAL "see C COMPILER" FORCE)
SET(CMAKE_C_FLAGS ${C_FLAGS} CACHE INTERNAL "see C COMPILER_FLAGS" FORCE)
SET(CMAKE_C_FLAGS_DEBUG ${CXX_FLAGS_DEBUG} CACHE INTERNAL "see CXX COMPILER_FLAGS_DEBUG" FORCE)
SET(CMAKE_C_FLAGS_RELEASE ${CXX_FLAGS_RELEASE} CACHE INTERNAL "see CXX COMPILER_FLAGS_RELEASE" FORCE)

# Set the include and library paths for PVTOL
INCLUDE_DIRECTORIES(${CUDA_DIR}/include)
INCLUDE_DIRECTORIES(${PVTOL_DIR}/include/base)
INCLUDE_DIRECTORIES(${PVTOL_DIR}/include/mpi)
INCLUDE_DIRECTORIES(${UTIL_DIR})
LINK_DIRECTORIES(${PVTOLPATH}/lib)


# Specify the code for the demo
ADD_EXECUTABLE(${EXECUTABLE} ${SRCS})

# Specify the libraries to link to
#PvtolBase_i686_Debug PvtolMpi_i686_Debug
SET(APP_LIB CoreApps)
TARGET_LINK_LIBRARIES(${EXECUTABLE} PvtolBase_x86_64_Debug
                                    PvtolMpi_x86_64_Debug
                                    ${APP_LIB}  
                                    ${MPI_LIB}
                                    mpi
                                    pthread 
				    				OpenCL )
				    
				    

