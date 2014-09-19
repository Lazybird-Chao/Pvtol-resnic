#  i686Default.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Tool locations for x86 builds
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################
MESSAGE(STATUS "MPICH2: ${MPICH2}")
# Boost
SET(BOOST_INCLUDE_PATHS                                              
  /usr/local/boost/include/boost)
SET(BOOST_LIB_PATHS                                                  
  /usr/local/boost/lib)

# MPICH2
SET( MPICH2_PATHS
  /usr/bin
  /usr/lib/)
  
	 
# Open MPI
SET(MPI_PATHS                                                        
  /opt/openmpi-1.6.5/bin
  /opt/openmpi-1.6.5)

SET(USE_MPI_COMPILER true)

# FFTW
SET(FFTW_PATHS                                                       
  /usr/local/include)
SET(FFTW_LIB_PATHS                                                   
  /usr/local/lib)

# VSIPL++
SET(VSIPLPP_PATHS
      /home/chao/vsipl++-2005Jun29/implementation)
SET(VSIPLPP_LIB_PATHS
  /home/chao/vsipl++-2005Jun29/implementation/vsip)


# MCF 
SET(MCF_PATHS 
    /opt/MultiCorePlus)

SET(XERCES_PATHS /tools/xerces )
