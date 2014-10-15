#######################################################
#   Application specifice configuration
#
#
#######################################################
##!!!!!!!!!
PROJECT(MatrixMulti)

# Set the name of the demo executable  !!!!!!
SET(EXECUTABLE MatrixMulti.run)

# Set source files for the demo  !!!!!!
SET(SRCS	main.cc
			matrixmulti.cc
	 					)



# !!!!! SET PROG_DIR to your own application directory !!!!!
SET(PROG_DIR  /home/chao/Desktop/Pvtol-resnic/TestApp/MM)


SET(APP_DIR ${PROG_DIR}/app)
INCLUDE_DIRECTORIES(${APP_DIR})
LINK_DIRECTORIES(${APP_DIR})

SET(SRC_DIR ${PROG_DIR}/src)
INCLUDE_DIRECTORIES(${SRC_DIR})






