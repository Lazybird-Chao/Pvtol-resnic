#  FindMPI.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Finds the MPI includes, libraries and compilers
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################


FIND_PATH(   MPI_INCLUDE mpi.h 
             PATHS ${MPI_PATHS}
             PATH_SUFFIXES include
             NO_DEFAULT_PATH)
IF(NOT MPI_INCLUDE)
    MESSAGE( FATAL_ERROR "Could not find MPI include dir" )
ENDIF(NOT MPI_INCLUDE)

INCLUDE_DIRECTORIES(${MPI_INCLUDE})


FIND_LIBRARY(MPI_LIB mpi_cxx
             PATHS ${MPI_PATHS}
             PATH_SUFFIXES lib
             NO_DEFAULT_PATH)
IF(NOT MPI_LIB)
    MESSAGE( FATAL_ERROR "Could not find MPI library" )
ENDIF(NOT MPI_LIB)

LINK_LIBRARIES(${MPI_LIB})


FIND_PROGRAM(MPI_RUN mpiexec 
             PATHS ${MPI_PATHS}
             PATH_SUFFIXES bin
             NO_DEFAULT_PATH)
IF(NOT MPI_RUN)
  MESSAGE( FATAL_ERROR "Could not find MPI run exe" )
ENDIF(NOT MPI_RUN)


FIND_PROGRAM(MPI_CXX mpicxx
             PATHS ${MPI_PATHS}
             PATH_SUFFIXES bin
             NO_DEFAULT_PATH)
IF(NOT MPI_CXX)
    MESSAGE( FATAL_ERROR "Could not find MPI cxx compiler" )
ENDIF(NOT MPI_CXX)


FIND_PROGRAM(MPI_CC mpicc
             PATHS ${MPI_PATHS}
             PATH_SUFFIXES bin
             NO_DEFAULT_PATH)
IF(NOT MPI_CC)
    MESSAGE( FATAL_ERROR "Could not find MPI cc compiler" )
ENDIF(NOT MPI_CC)


ADD_DEFINITIONS( -D_STANDARD_MPI )

# If we are not using the mpi compiler we will need some extra libs
IF(NOT USE_MPI_COMPILER)
    FIND_LIBRARY(MPI_LIB2 mpi
                 PATHS ${MPI_PATHS}
                 PATH_SUFFIXES lib
                 NO_DEFAULT_PATH)
    IF(NOT MPI_LIB2)
      MESSAGE( FATAL_ERROR "Could not find MPI library (2)" )
    ENDIF(NOT MPI_LIB2)

    LINK_LIBRARIES(${MPI_LIB2})

    FIND_LIBRARY(MPI_LIB3 open-rte
                 PATHS ${MPI_PATHS}
                 PATH_SUFFIXES lib
                 NO_DEFAULT_PATH)
    IF(NOT MPI_LIB3)
        MESSAGE( FATAL_ERROR "Could not find MPI library (3)" )
    ENDIF(NOT MPI_LIB3)

    LINK_LIBRARIES(${MPI_LIB3})

    FIND_LIBRARY(MPI_LIB4 open-pal
                 PATHS ${MPI_PATHS}
                 PATH_SUFFIXES lib
                 NO_DEFAULT_PATH)
    IF(NOT MPI_LIB4)
        MESSAGE( FATAL_ERROR "Could not find MPI library (4)" )
    ENDIF(NOT MPI_LIB4)

    LINK_LIBRARIES(${MPI_LIB4})

    # Use find file
    FIND_FILE(NUMA_LIB libnuma.so.1
                 PATHS ${NUMA_PATHS}
                 NO_DEFAULT_PATH)
    IF(NOT NUMA_LIB)
        MESSAGE( FATAL_ERROR "Could not find NUMA library in ${NUMA_PATHS} ${NUMA_LIB}" )
    ENDIF(NOT NUMA_LIB)

    LINK_LIBRARIES(${NUMA_LIB})
    LINK_LIBRARIES(util)
    LINK_LIBRARIES(pthread)

ENDIF(NOT USE_MPI_COMPILER)




####################### Totalview            #########################
OPTION(TOTALVIEW "Use Totalview?" OFF)
IF(TOTALVIEW MATCHES ON)
   SET(MPI_FLAGS "-tv")
ENDIF(TOTALVIEW MATCHES ON)



#only looks until found (then stops checking)
FIND_FILE(MACHINE_FILE_FOUND ${MACHINE_FILE} "The machine file for mpi exec")

IF (MACHINE_FILE_FOUND MATCHES "MACHINE_FILE_FOUND-NOTFOUND")
  # no machine file found, use default
ELSE (MACHINE_FILE_FOUND MATCHES "MACHINE_FILE_FOUND-NOTFOUND")
  SET(MPI_FLAGS -machinefile;${MACHINE_FILE_FOUND})
ENDIF(MACHINE_FILE_FOUND MATCHES "MACHINE_FILE_FOUND-NOTFOUND")

