
# Builds src/includes and optionally the tests

# include the precore cmake file
#SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/util/cmake FORCE)
include(./util/cmake/CMake_PRECORE.cmake)

# Create a project
project(PVTOL)

include(./util/cmake/CMake_CORE.cmake)

# libraries and code exist here
add_subdirectory(src)

# atlas
IF(BUILD_ATLAS)
    add_subdirectory(util/atlasPreProcessor)
ENDIF(BUILD_ATLAS)

# Using EXCLUDE_FROM_ALL means that tests don't get built by default
IF(BUILD_TESTS)
    #add_subdirectory(tests)
ENDIF(BUILD_TESTS)

#include Dart for Testing
INCLUDE(Dart)
