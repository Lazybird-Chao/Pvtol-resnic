#  CellDefault.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Cell Default Path list file
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

# Boost                                                              
SET(BOOST_INCLUDE_PATHS                                              
  /usr/local/boost/include/boost)
SET(BOOST_LIB_PATHS                                                  
  /usr/local/boost/lib) 

# VSIPL++                                                                                                           
SET(VSIPLPP_PATHS
      /home/chao/vsipl++-2005Jun29/implementation)
SET(VSIPLPP_LIB_PATHS
  /home/chao/vsipl++-2005Jun29/implementation/vsip)
                                                        
# MPICH2
SET( MPICH2_PATHS
  /usr/lib)
	 
# Open MPI                                                           
SET(MPI_PATHS                                                        
  /opt/openmpi-1.6.5/bin
  /opt/openmpi-1.6.5)

SET(USE_MPI_COMPILER true)
# NUMA is required when USE_MPI_COMPILER=false
SET(NUMA_PATHS /home/gridsan/pvtol/tools/ppc64 /home/pvtol/tools/ppc64 )

# MCF
SET(MCF_PATHS 
    /opt/MultiCorePlus)

# IBM
SET(IBM_SPE_PATHS 
    /usr/lib)

# These are not used if USE_MPI_COMPILER is set to true
# Compiler Paths
SET(CELL_COMPILER_PATHS /opt/cell/bin /opt/cell/toolchain/bin)

SET(XERCES_PATHS /tools/xerces-2.8.0 
    /home/gridsan/pvtol/tools/ppc64/xerces-2.8.0/)

