#  CellCrossDefault.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   Default Path list for cross compiling on the cell
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

# This has not yet been fully tested

# Boost                                                              
SET(BOOST_INCLUDE_PATHS                                              
  /usr/local/boost/include/boost)
SET(BOOST_LIB_PATHS                                                  
  /usr/local/boost/lib)
                                                                     
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

# NUMA is required when USE_MPI_COMPILER=false
SET(NUMA_PATHS /home/gridsan/pvtol/tools/ppc64 /home/pvtol/tools/ppc64 )

# MCF
SET(MCF_PATHS 
    /opt/MultiCorePlus)

# IBM
SET(IBM_SPE_PATHS 
    /usr/lib
    /opt/cell/sysroot/usr/lib/)
  
# These are not used if USE_MPI_COMPILER is set to true
# Compiler Paths
SET(CELL_COMPILER_PATHS /opt/cell/bin /opt/cell/toolchain/bin)

SET(XERCES_PATHS /tools/xerces /home/gridsan/pvtol/tools/ppc64/xerces-2.8.0/)

#SET(MCF_LIB "mcf_m")
#SET(IBM_SPE_LIB "spe2")
#SET(VSIPLPP_LIB "svpp")





