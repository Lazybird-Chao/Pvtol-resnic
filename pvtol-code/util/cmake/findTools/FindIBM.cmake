#  FindIBM.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Find the IBM spe lib, requires paths to be run first
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################


# Find IBM SPE Lib

IF( CC_CELL )
  SET(IBM_SPE_LIB "spe2")
  LINK_DIRECTORIES(${IBM_SPE_PATHS})
ELSE( CC_CELL )
  FIND_LIBRARY(IBM_SPE_LIB spe2
               PATHS ${IBM_SPE_PATHS}
               NO_DEFAULT_PATH)
ENDIF( CC_CELL )

IF(NOT IBM_SPE_LIB)
   MESSAGE(FATAL_ERROR "Could not find IBM SPE lib!")
ENDIF(NOT IBM_SPE_LIB)

LINK_LIBRARIES(${IBM_SPE_LIB})

