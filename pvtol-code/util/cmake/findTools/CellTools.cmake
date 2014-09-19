#  CellTools.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This file includes all files that are required for 
#           Cell builds of PVTOL
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################
MESSAGE(STATUS "CellToolsMPICH2: ${MPICH2}")
if(MPICH2)
include(FindMPICH2)
else(MPICH2)
include(FindMPI)
endif(MPICH2)

include(FindBoost)
include(FindVsipl)
include(FindMCF)
include(FindIBM)

INCLUDE(AtlasTools)

####################### Definitions, Includes, Linking ################

# This is the library suffix
SET(LIB_NAME CELL_${CMAKE_BUILD_TYPE})

SET(PVTOL_BASE_LIB PvtolBase_${LIB_NAME})
SET(PVTOL_MPI_LIB PvtolMpi_${LIB_NAME})
SET(PVTOL_MCF_LIB PvtolMcf_${LIB_NAME})

# Set PVTOL MCF dir and lib
#SET(PVTOL_MCF_INCLUDE_DIR "${PVTOL_DATABASE}/include/mcf" 
#    CACHE STRING "Includes for Pvtol MCF specializations" FORCE)

LINK_DIRECTORIES( 
  ${LIBRARY_OUTPUT_PATH};
  )

LINK_LIBRARIES(
  ${PVTOL_BASE_LIB};
  ${PVTOL_MPI_LIB};
  ${PVTOL_MCF_LIB};
  )
  
ADD_DEFINITIONS( -D_PVTOL_CELL )

# Add 32 bit flag for cell
SET (CXX_FLAGS "-ansi -ftemplate-depth-60 -m32")
SET (C_FLAGS "-ansi -m32")

IF(USE_MPI_COMPILER)
    SET(CXX_COMPILER ${MPI_CXX} CACHE STRING "CXX Compiler" FORCE)
    SET(C_COMPILER ${MPI_CC} CACHE STRING "C Compiler" FORCE)
ELSE(USE_MPI_COMPILER)
    # Add definitions to use mpicxx like compiling with another compiler
    ADD_DEFINITIONS( -rpath-link ${NUMA_LIB})
    FIND_PROGRAM(CXX_COMPILER ppu-g++
                 PATHS ${CELL_COMPILER_PATHS}
                 NO_DEFAULT_PATHS)
    FIND_PROGRAM(C_COMPILER ppu-gcc
                 PATHS ${CELL_COMPILER_PATHS}
                 NO_DEFAULT_PATHS)
    #SET(CXX_COMPILER ${CELL_CXX_COMPILER} CACHE STRING "CXX Compiler" FORCE)
    #SET(C_COMPILER ${CELL_C_COMPILER} CACHE STRING "C Compiler" FORCE)
ENDIF(USE_MPI_COMPILER)

