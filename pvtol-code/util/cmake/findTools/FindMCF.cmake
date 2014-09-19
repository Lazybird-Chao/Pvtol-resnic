#  FindMCF.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Find the MCF includes/lib, requires paths to be run first
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################


####################### Find MultiCorePlus     ###########################
FIND_PATH(MCF_INCLUDE mcf_m.h 
          PATHS ${MCF_PATHS}
          PATH_SUFFIXES include
          NO_DEFAULT_PATH)

IF(NOT MCF_INCLUDE)
   MESSAGE(FATAL_ERROR "Could not find MCF include dir!")    
ENDIF(NOT MCF_INCLUDE)

INCLUDE_DIRECTORIES(${MCF_INCLUDE})

IF( CC_CELL )
  LINK_DIRECTORIES(${MCF_PATHS}/lib)
  SET(MCF_LIB "mcf_m")
ELSE( CC_CELL )
  FIND_LIBRARY(MCF_LIB mcf_m
               PATHS ${MCF_PATHS}
               PATH_SUFFIXES lib
               NO_DEFAULT_PATH)
ENDIF( CC_CELL )

IF(NOT MCF_LIB)
   MESSAGE(FATAL_ERROR "Could not find MCF lib!")
ENDIF(NOT MCF_LIB)

LINK_LIBRARIES(${MCF_LIB})
