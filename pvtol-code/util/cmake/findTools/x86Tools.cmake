#  x86Tools.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This file includes all files that are required for PVTOL
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################
MESSAGE(STATUS "x86ToolsMPICH2: ${MPICH2}")
IF(MPICH2)
include(FindMPICH2)
ELSE(MPICH2)
include(FindMPI)
ENDIF(MPICH2)

include(FindBoost)
include(FindVsipl)
include(FindFftw)

INCLUDE(AtlasTools)

####################### Definitions, Includes, Linking ################

# This is the library suffix
SET(LIB_NAME ${CMAKE_SYSTEM_PROCESSOR}_${CMAKE_BUILD_TYPE})

SET(PVTOL_BASE_LIB PvtolBase_${LIB_NAME})
SET(PVTOL_MPI_LIB PvtolMpi_${LIB_NAME})

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

LINK_LIBRARIES(
  ${PVTOL_BASE_LIB};
  ${PVTOL_MPI_LIB};
  )

ADD_DEFINITIONS(
  -msse;
  -msse2;
  -mmmx;
  -DSSE;
  )


#Library builds need -ansi
SET (CXX_FLAGS "-ansi -ftemplate-depth-60" CACHE STRING "Flags used by the compiler during all build types." FORCE)
SET (C_FLAGS "-ansi" FORCE)

SET (CXX_COMPILER ${MPI_CXX} CACHE STRING "CXX Compiler" FORCE)
SET (C_COMPILER ${MPI_CC} CACHE STRING "C Compiler" FORCE)

