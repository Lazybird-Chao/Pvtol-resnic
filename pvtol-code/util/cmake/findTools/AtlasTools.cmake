#  AtlasTools.cmake
#
#  \author  $LastChangedBy$
#  \date    $LastChangedDate$
#  \version $LastChangedRevision$
#  \brief   This includes the additional files required by Atlas
#
#  $Id$
#
#  Copyright (c) 2008, Massachusetts Institute of Technology
#
#  All rights reserved.
#############################################################################

SET(BUILD_ATLAS true) # will be set to false if it cannot find any of the req'd tools

SET(USE_BOOST_SER_LIB true)
INCLUDE(FindBoost)
INCLUDE(FindXerces)

####################### Definitions, Includes, Linking ################

ADD_DEFINITIONS(
  -DCSP_NO_NAMESPACE;
  )