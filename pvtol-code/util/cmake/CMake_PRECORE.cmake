#  CMake_PRECORE.cmake
#
#   December 2007 Jim Daly ported from Z. Sun's PVL version
#
#  \author  $LastChangedBy: $
#  \date    $LastChangedDate: 2008-10-27 15:27:24 -0400 (Mon, 27 Oct 2008) $
#  \version $LastChangedRevision: 856 $
#  \brief   
#              This file must be run before 'PROJECT' is called
#
#  $Id: CMake_PRECORE.cmake 856 2008-10-27 19:27:24Z  $
#
#  Copyright (c) 2007, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

#Used for cross compiling (possibly for CPU type)
#(Leave empty as default)
SET(TARGET "" CACHE STRING "Type of System Architecture (IntelX86 Cell_PPE)")

#Set this variable to see the commands issued during builds
SET(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Echos all commands used during builds" FORCE)

# Setup the Testing timeout (before the project command)
SET(DART_TESTING_TIMEOUT 300)

