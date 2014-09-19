#  CMake_CORE.cmake
#
#   December 2007 Jim Daly ported from Z. Sun's PVL version
#
#  \author  $LastChangedBy: ka21088 $
#  \date    $LastChangedDate: 2008-11-13 14:55:32 -0500 (Thu, 13 Nov 2008) $
#  \version $LastChangedRevision: 882 $
#  \brief   
#
#  $Id: CMake_CORE.cmake 882 2008-11-13 19:55:32Z ka21088 $
#
#  Copyright (c) 2007, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

# Setup the module/util cmake path
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/util/cmake)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/util/cmake/findTools)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/util/cmake/findPaths)


# optional include a path to install binaries to
SET(CMAKE_INSTALL_PATH ${PROJECT_SOURCE_DIR}/bin)

#Used to specify where the final library goes
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib CACHE PATH "Single output directory for building all libraries." FORCE)

#Folder specifying where the Atlas preprocessors are
SET(ATLAS_FOLDER ${PROJECT_SOURCE_DIR}/util/atlasPreProcessor)
SET(ATLAS_PP atlaspp_${CMAKE_SYSTEM_PROCESSOR})



#find the processor (distinguish between LG4 and Linux Pentium/AMD machines
IF(CMAKE_UNAME)
   EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
ENDIF(CMAKE_UNAME)

####################### System Architecture ##########################

#we are not building for cell processor
SET(TARGET_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "Set target processor")
SET(ENDIAN 0) #Little Endian

# Determine which Paths and Tools to include based on the target processor
#   and if we are cross compiling
IF(CMAKE_SYSTEM MATCHES "Linux") #command is result of uname
   
   IF( TARGET_PROCESSOR MATCHES "x86_64")
        IF(NOT SYSTEM)
            INCLUDE(x86_64Default)
        ELSE(NOT SYSTEM)
            INCLUDE(x86_64${SYSTEM})
        ENDIF(NOT SYSTEM)

        INCLUDE(x86Tools)
   ELSEIF( TARGET_PROCESSOR MATCHES "i686")
        IF(NOT SYSTEM)
            INCLUDE(i686Default)
        ELSE(NOT SYSTEM)
            INCLUDE(i686${SYSTEM})
        ENDIF(NOT SYSTEM)
        
        INCLUDE(x86Tools)
   ELSE( TARGET_PROCESSOR MATCHES "x86_64" )
        MESSAGE(FATAL_ERROR "Build on this Architecture not supported")
   ENDIF( TARGET_PROCESSOR MATCHES "x86_64" )

ELSE(CMAKE_SYSTEM MATCHES "Linux")
    MESSAGE(FATAL_ERROR "Non Linux build not supported")
ENDIF(CMAKE_SYSTEM MATCHES "Linux")

SET(EXE_SUFFIX "")

###############################################################################
##  Library setup

SET(LIB_SUFFIX .a)

#This variable was found in share/CMake/Modules/CMakeSystemSpecificInformation.cmake 
#  -- it sets the correct library suffix
SET(CMAKE_STATIC_LIBRARY_SUFFIX ${LIB_SUFFIX})

IF(CMAKE_SYSTEM MATCHES "Linux")
   SET(AR_ARGS "-cr")
ENDIF(CMAKE_SYSTEM MATCHES "Linux")

IF(NOT CMAKE_C_CREATE_STATIC_LIBRARY)
  SET(CMAKE_C_CREATE_STATIC_LIBRARY
      "<CMAKE_AR> <TARGET> <LINK_FLAGS> <OBJECTS> "
      "<CMAKE_RANLIB> <TARGET>")
ENDIF(NOT CMAKE_C_CREATE_STATIC_LIBRARY)

IF(NOT CMAKE_CXX_CREATE_STATIC_LIBRARY)
  SET(CMAKE_CXX_CREATE_STATIC_LIBRARY
      "<CMAKE_AR> ${AR_ARGS} <TARGET> <LINK_FLAGS> <OBJECTS> "
      "<CMAKE_RANLIB> <TARGET>")
ENDIF(NOT CMAKE_CXX_CREATE_STATIC_LIBRARY)

###############################################################################
##  General compiler setup

IF(COVERAGE)
SET (CXX_FLAGS_DEBUG "-g -O0 -Wall -DPVTOL_DEVELOP -fprofile-arcs -ftest-coverage" CACHE STRING "Flags used by the compiler during debug builds." FORCE)
ELSE(COVERAGE)
SET (CXX_FLAGS_DEBUG "-g -O0 -Wall -DPVTOL_DEVELOP" CACHE STRING "Flags used by the compiler during debug builds." FORCE)
ENDIF(COVERAGE)

SET (CXX_FLAGS_RELEASE "-O3 -Wall -fstrict-aliasing" CACHE STRING "Flags used by the compiler during release builds." FORCE)

SET (AR "ar" CACHE STRING "Archiver" FORCE)
SET (AR_ARGS "-cr" CACHE STRING "Archiver" FORCE)
SET (RANLIB "ranlib" CACHE STRING "Ranlib" FORCE)


#We have to use a second set of variable since 'PROJECT' automatically sets them
SET(CMAKE_CXX_COMPILER ${CXX_COMPILER} CACHE INTERNAL "see CXX COMPILER" FORCE)
SET(CMAKE_CXX_FLAGS ${CXX_FLAGS} CACHE INTERNAL "see CXX COMPILER_FLAGS" FORCE)
SET(CMAKE_CXX_FLAGS_DEBUG ${CXX_FLAGS_DEBUG} CACHE INTERNAL "see CXX COMPILER_FLAGS_DEBUG" FORCE)
SET(CMAKE_CXX_FLAGS_RELEASE ${CXX_FLAGS_RELEASE} CACHE INTERNAL "see CXX COMPILER_FLAGS_RELEASE" FORCE)

#set C debug and release flags as CXX flags
SET(CMAKE_C_COMPILER ${C_COMPILER} CACHE INTERNAL "see C COMPILER" FORCE)
SET(CMAKE_C_FLAGS ${C_FLAGS} CACHE INTERNAL "see C COMPILER_FLAGS" FORCE)
SET(CMAKE_C_FLAGS_DEBUG ${CXX_FLAGS_DEBUG} CACHE INTERNAL "see CXX COMPILER_FLAGS_DEBUG" FORCE)
SET(CMAKE_C_FLAGS_RELEASE ${CXX_FLAGS_RELEASE} CACHE INTERNAL "see CXX COMPILER_FLAGS_RELEASE" FORCE)

SET(CMAKE_EXE_LINKER_FLAGS ${EXE_LINKER_FLAGS} CACHE INTERNAL "see EXE_LINKER_FLAGS" FORCE)
SET(CMAKE_AR ${AR} CACHE INTERNAL "see AR" FORCE)
SET(CMAKE_RANLIB ${RANLIB} CACHE INTERNAL "see RANLIB" FORCE)

IF(CMAKE_BUILD_TYPE MATCHES "Release")
  SET(CMAKE_EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS} ${CXX_FLAGS_RELEASE}" CACHE INTERNAL "see EXE_LINKER_FLAGS" FORCE)
ELSE(CMAKE_BUILD_TYPE MATCHES "Release")
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Set default to DEBUG" FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS} ${CXX_FLAGS_DEBUG}" CACHE INTERNAL "see EXE_LINKER_FLAGS" FORCE)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")

#these variables are hidden in gui until user chooses advanced options
MARK_AS_ADVANCED(FORCE 
  CXX_COMPILER;
  CXX_FLAGS;
  CXX_FLAGS_DEBUG;
  CXX_FLAGS_RELEASE;
  C_COMPILER;
  C_FLAGS;
  C_FLAGS_DEBUG;
  C_FLAGS_RELEASE;
  AR;
  RANLIB;
  )


####################### Macros for easier builds  #####################


# The following commands regenerate the makefile (a call to cmake) with the specified input and then call make

ADD_CUSTOM_TARGET(debug
  ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:string=Debug ${PROJECT_SOURCE_DIR} \\;
  make -s
  )

ADD_CUSTOM_TARGET(release
  ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:string=Release ${PROJECT_SOURCE_DIR} \\;
  make -s
  )

#No cross compiler
ADD_CUSTOM_TARGET(nocross
  ${CMAKE_COMMAND} -DTARGET:string="" ${PROJECT_SOURCE_DIR} \\;
  make
  )

