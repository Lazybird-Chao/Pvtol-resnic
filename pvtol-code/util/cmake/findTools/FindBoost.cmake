#  FindBoost.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Find the boost includes/lib requires paths to be run first
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################


FIND_PATH(BOOST_INCLUDE scoped_array.hpp
          PATHS ${BOOST_INCLUDE_PATHS}
          NO_DEFAULT_PATH)

IF(NOT BOOST_INCLUDE)
  MESSAGE(FATAL_ERROR "Could NOT find BOOST include dir!")
ENDIF(NOT BOOST_INCLUDE)

INCLUDE_DIRECTORIES(${BOOST_INCLUDE})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE}/..)

IF(USE_BOOST_SER_LIB)
   FIND_LIBRARY(BOOST_LIB boost_serialization
                PATHS ${BOOST_LIB_PATHS}
                PATH_SUFFIXES lib
                NO_DEFAULT_PATH)
   IF(NOT BOOST_LIB)
      MESSAGE(FATAL_ERROR "Could NOT find BOOST serialization lib!")
   ENDIF(NOT BOOST_LIB)
   
   LINK_LIBRARIES(${BOOST_LIB})

ENDIF(USE_BOOST_SER_LIB)



