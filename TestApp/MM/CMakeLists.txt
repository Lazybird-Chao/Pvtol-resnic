################################################################################
#
#  Copyright (c) 2009, Massachusetts Institute of Technology
#  All rights reserved.
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This CMake file will include all subdirectories containing a
#           CMakeLists.txt file for building.
#
#  Author: Hahn Kim
#
#  $Id$
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.2)
# Version 2.2.3 Adds SET_TESTS_PROPERTIES with FAIL_REGULAR_EXPRESSION
# Which is used to search the error output of tests.


######################################################################
# SETUP SUBDIRECTORIES

# Get a list of all dirs with CMakeLists.txt inside
IF(NOT TEST)
  FILE(GLOB TESTS "src/CMakeLists.txt")
  # Iterate through all directories
  FOREACH( t ${TESTS})
     GET_FILENAME_COMPONENT( dirs ${t} PATH ) # Get just the path
     GET_FILENAME_COMPONENT( dir ${dirs} NAME ) # Get the subdir part of the path
     SUBDIRS( ${dir} ) # Add the subdir to the project
  ENDFOREACH ( t )
ELSE(NOT TEST)
  SUBDIRS( ${TEST} )
ENDIF(NOT TEST)

