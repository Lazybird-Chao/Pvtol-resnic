#  FindXerces.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Find the fftw includes/lib requires paths to be run first
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

FIND_PATH(XERCES_INCLUDE xercesc/dom/DOM.hpp
    PATHS ${XERCES_PATHS}
    PATH_SUFFIXES include
    NO_DEFAULT_PATHS)
FIND_FILE(XERCES_LIB libxerces-c.so
    PATHS ${XERCES_PATHS}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATHS)
FIND_PATH(XERCES_LIB_PATH libxerces-c.so
    PATHS ${XERCES_PATHS}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATHS)

IF(NOT XERCES_INCLUDE OR NOT XERCES_LIB)
    MESSAGE(STATUS "Could not find Xerces, not building Atlas")
    SET(BUILD_ATLAS false)
ELSE(NOT XERCES_INCLUDE OR NOT XERCES_LIB)
    INCLUDE_DIRECTORIES(
        ${XERCES_INCLUDE}
        )
    LINK_DIRECTORIES( ${XERCES_LIB_PATH} )
    LINK_LIBRARIES(xerces-c #/tools/xerces/lib/libxerces-c.so
        #${XERCES_LIB}
        )
    SET(BUILD_ATLAS true)
ENDIF(NOT XERCES_INCLUDE OR NOT XERCES_LIB)


#/tools/xerces/lib/libxerces-c.so; /tools/xerces/lib/libxerces-c.so


