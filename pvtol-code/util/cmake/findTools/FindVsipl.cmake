#  FindVsipl.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Find the VSIPL++ includes/lib, requires paths to be run first
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################


####################### Find VSIPL++          ########################## 
FIND_PATH(VSIPLPP_INCLUDE vsip/vector.hpp
          PATHS ${VSIPLPP_PATHS}
          PATH_SUFFIXES include
          NO_DEFAULT_PATH)

IF(NOT VSIPLPP_INCLUDE)
   MESSAGE(FATAL_ERROR "Could NOT find VSIPL++ include dir!")
ENDIF(NOT VSIPLPP_INCLUDE)

INCLUDE_DIRECTORIES(${VSIPLPP_INCLUDE})

#IF( CC_CELL )
#  SET(VSIPLPP_LIB "svpp")
#  LINK_DIRECTORIES(${VSIPLPP_LIB_PATHS})
#ELSE( CC_CELL )
#  FIND_LIBRARY(VSIPLPP_LIB svpp
#               PATHS ${VSIPLPP_LIB_PATHS}
#               NO_DEFAULT_PATH)
#ENDIF( CC_CELL )

#IF(NOT VSIPLPP_LIB)
#  MESSAGE(FATAL_ERROR "Could NOT find VSIPL++ lib!")
#ENDIF(NOT VSIPLPP_LIB)

LINK_LIBRARIES(${VSIPLPP_LIB})

