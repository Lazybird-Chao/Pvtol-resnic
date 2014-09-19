#  CMakeLists.txt
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   CMakeLists.txt file for building Atlas
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/util/atlasPreProcessor/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/base)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/mpi)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/mcf)
#INCLUDE(include) # includes src/util/atlasPreProcessor/include

SET(ATLAS_EXE ${ATLAS_PP})

ADD_EXECUTABLE(${ATLAS_EXE}
  source/atlasPreProc.cc
  source/atlasPreProcErrorHandler.cc
  source/atlasPreProcUtils.cc
  source/atlasStrings.cc
)

#ADD_CUSTOM_TARGET(move ALL
#interactive so user doesn't overwrite 
#  echo Atlas;preprocessor;${PROJECT_NAME};now;built,;moving;to;bin \;
#  mv;-i;${ATLAS_EXE};${PVTOL_DATABASE}/util/atlasPreProcessor/bin;
#)

ADD_CUSTOM_TARGET(move ALL
  echo Atlas;preprocessor;now;built,;moving;to;bin \;
  mv;${ATLAS_PP};${PROJECT_SOURCE_DIR}/bin;
)

# CHANGE THE INSTALL TO A COPY since the libraries cannot be found after install...
#INSTALL(TARGETS ${ATLAS_PP} RUNTIME DESTINATION ${CMAKE_INSTALL_PATH}) 