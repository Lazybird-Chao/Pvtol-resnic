##############################################################################
#  Makefile   for PVTOL
#
#   \author  $LastChangedBy: $
#   \date    $LastChangedDate: 2009-02-09 15:29:16 -0500 (Mon, 09 Feb 2009) $
#   \version $LastChangedRevision: 927 $
#   \brief   Makefile used to kick off the building of the PVTOL libraries
#             this can also be used to build and run the PVTOL unit tests,
#             performance bencmarks, and any utilities
#
#     Note: CMAKE is used herein extensively to build for the many platforms
#             that PVTOL supports.
# 
#   $Id: makefile 927 2009-02-09 20:29:16Z  $
#   $HeadURL: http://cvs102/svn/pvtol/branch/pvtol-gpu/code/makefile $
# 
#   Copyright (c) 2007, Massachusetts Institute of Technology
# 
#   All rights reserved.
#
##############################################################################

#############################################################################
# Determine the architecture of the host which is running 'make'
# Also find a cmake path to use
ARCH=$(shell uname -m)
ifndef HOSTARCH
SYSID=$(shell uname -sm)
ifeq ($(SYSID),Linux i686)
HOSTARCH=$(ARCH)
CMAKE_DIR=$(shell if [ -e /tools/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.8/bin/"; \
                  elif [ -e /tools/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.7/bin/"; \
                  elif [ -e /home/gridsan/pvtol/tools/x86_64/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/home/gridsan/pvtol/tools/x86_64/cmake-2.4.8/bin/"; \
                  elif [ -e /home/pvtol/tools/x86_64/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/home/pvtol/tools/x86_64/cmake-2.4.8/bin/cmake"; \
                  elif [ -e /usr/local/bin/cmake ] ; \
                  then echo "/usr/local/bin/"; fi)
endif
ifeq ($(SYSID),Linux x86_64)
HOSTARCH=$(ARCH)
CMAKE_DIR=$(shell if [ -e /tools/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.8/bin/"; \
                  elif [ -e /tools/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.7/bin/"; \
                  elif [ -e /home/gridsan/pvtol/tools/x86_64/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/home/gridsan/pvtol/tools/x86_64/cmake-2.4.8/bin/"; \
                  elif [ -e /home/pvtol/tools/x86_64/cmake-2.4.8/bin/cmake ] ; \
                  then echo "/home/pvtol/tools/x86_64/cmake-2.4.8/bin/"; \
                  elif [ -e /usr/local/bin/cmake ] ; \
                  then echo "/usr/local/bin/"; fi)
endif
ifeq ($(SYSID),Linux ppc64)
HOSTARCH=CELL
CMAKE_DIR=$(shell if [ -e /home/gridsan/pvtol/tools/ppc64/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/home/gridsan/pvtol/tools/ppc64/cmake-2.4.7/bin/"; \
                  elif [ -e /tools/cmake-2.4.7/bin/cmake ] ; \
                  then echo "/tools/cmake-2.4.7/bin/"; \
                  elif [ -e /usr/local/bin/cmake ] ; \
                  then echo "/usr/local/bin/"; fi)
endif
endif

CMAKE_COMMAND=$(CMAKE_DIR)cmake
CTEST_COMMAND=$(CMAKE_DIR)ctest

# decides name of temporary build folder (e.g. tmpLocalBuild_PENTIUM)
ifndef TARGET
OBJ_FOLDER=$(HOSTARCH)
else
OBJ_FOLDER=$(TARGET)_CROSS
CROSSFLAGS=-DTARGET:string=$(TARGET)
endif

# Release file is relative to where ctest is called from (normally in the temp dir)
RELEASEFILE=-A ../util/cmake/Release.txt
DEBUGFLAG=-DCMAKE_BUILD_TYPE:string=Debug
RELEASEFLAG=-DCMAKE_BUILD_TYPE:string=Release

LIB_DIR=../lib
LIB_BIN=./tmpLibBuild_$(OBJ_FOLDER)
APP_BIN=./tmpAppBuild_$(OBJ_FOLDER)
TEST_BIN=$(LIB_BIN)/tests
INST_BIN=../install

ifndef BUILD_TESTS
	BUILD_TESTS=true
endif
BUILDTESTSFLAG=-DBUILD_TESTS:string=$(BUILD_TESTS)

ifndef MPICH2
	MPICH2=false
endif
#-D declares a variable in cmake
BUILDWMPICH2=-DMPICH2:string=$(MPICH2)

ifndef COVERAGE
	COVERAGE=false
endif
TESTCOVERAGE=-DCOVERAGE:string=$(COVERAGE)

# System flags are used to denote that there is a separate build location file
# for the system specified, it will look for one of the following files depending
# on the type of system that you are building on:
# CellCross<system>.cmake Cell<system>.cmake x86_64<system>.cmake and i686<system>.cmake
ifdef SYSTEM
SYSFLAG=-DSYSTEM:string=$(SYSTEM)
endif

BUILDFLAGS= $(CROSSFLAGS) $(BUILDTESTSFLAG) $(BUILDWMPICH2) $(TESTCOVERAGE) $(SYSFLAG)

#############################################################################
# Code building targets
# 

default: debug

cmake: makelibdirs
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CMAKE_COMMAND) .. $(DEBUGFLAG) $(BUILDFLAGS)

cmakeR: makelibdirs
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CMAKE_COMMAND) .. $(RELEASEFLAG) $(BUILDFLAGS)

debug: cmake
	make -C $(LIB_BIN)

release: cmakeR
	make -C $(LIB_BIN)

all: debug

clean:
	-rm -fr $(LIB_BIN)

dist: cleanAll debug clean release doxygenrelease
	 rm -rf $(INST_BIN)/$(OBJ_FOLDER)
	 rsync -ar lib include bin $(INST_BIN)/$(OBJ_FOLDER) --exclude=".svn"
	 rsync -ar ../doc/doxygen/docRel/PVTOL.pdf ../doc/doxygen/docRel/html $(INST_BIN)/$(OBJ_FOLDER)/doc --exclude=".svn"

smalldist: cleanAll release
	 rm -rf $(INST_BIN)/$(OBJ_FOLDER)
	 rsync -ar lib include bin $(INST_BIN)/$(OBJ_FOLDER) --exclude=".svn"

makelibdirs:
	mkdir -p $(LIB_BIN)

cleanAll: clean cleanApps
	-rm -f ./lib/*.a
	-rm -f ./bin/*

doxygen:
	doxygen ../doc/doxygen/Doxyfile
	$(MAKE) -C ../doc/doxygen/doc/latex/ all
	cp  ../doc/doxygen/doc/latex/refman.pdf ../doc/doxygen/doc/PVTOL.pdf

doxygenrelease:
	doxygen ../doc/doxygen/DoxyfileRel
	$(MAKE) -C ../doc/doxygen/docRel/latex/ all
	mv ../doc/doxygen/docRel/latex/refman.pdf ../doc/doxygen/docRel/PVTOL.pdf

#############################################################################
# Atlas PP
#
atlasPP: cmake
	make -C $(LIB_BIN)/util/atlasPreProcessor

atlasPPclean:
	-make -C $(LIB_BIN)/util/atlasPreProcessor clean

#############################################################################
# Test building targets
#
unitTests:
	$(MAKE) -C $(TEST_BIN)/$(TEST)

unitTestsRelease:
	@echo "This will only build release tests if you first called make release"
	$(MAKE) -C $(TEST_BIN)/$(TEST)

test:
	-$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND) --force-new-ctest-process -R $(TEST)

testV:
	-$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND) -V --force-new-ctest-process -R $(TEST)

runUnitTests: test
runVUnitTests: testV

cleanUnitTests:
	-$(MAKE) -C $(TEST_BIN)/$(TEST) clean

utAll: cmake cleanUnitTests unitTests test
	@echo "Finished Rebuliding and running!"

utVAll: cmake cleanUnitTests unitTests testV
	@echo "Finished Rebuliding and running!"

utRAll: cmakeR cleanUnitTests unitTestsRelease test
	@echo "Use make release, then make test to run Release tests"


#############################################################################
# Dart submission building targets
#

experimentalBuild: cmake
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Experimental

nightlyBuild: cmake
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Nightly

# release flag
experimentalRBuild: cmakeR
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Experimental ${RELEASEFILE}

# release flag
nightlyRBuild: cmakeR
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Nightly ${RELEASEFILE}

experimental: cleanAll experimentalBuild
	@echo "Experimental build and test has completed"

nightly: cleanAll nightlyBuild
	@echo "Nightly build and test has completed"

# Release versions 
experimentalRelease: cleanAll experimentalRBuild
	@echo "Experimental release build and test has completed"

nightlyRelease: cleanAll nightlyRBuild
	@echo "Nightly release build and test has completed"

#  Continuous
continuous: cmake
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Continuous 

continuousRelease: cmakeR
	$(CMAKE_COMMAND) -E chdir $(LIB_BIN) $(CTEST_COMMAND)  --force-new-ctest-process -D Continuous ${RELEASEFILE}

#############################################################################
# Benchmarks
#

mpiBenchmark:
	cd ./benchmarks/IMB && ./IMBTest_FULL.script

mpiBenchmarkHelp:
	cat ./benchmarks/IMB/README.txt



.PHONY : debug release clean cleanAll test unitTests runUnitTests testR unitTestsRelease cleanUnitTests utAll utRAll runVUnitTests utVAll apps runApps runVApps cleanApps experimentalBuild experimental experimentalRBuild experimentalRelease nightlyBuild nightly nightlyRBuild nightlyRelease doxygen doxygenrelease dist smalldist mpiBenchmark mpiBenchmarkHelp help

#############################################################################
# Help!
#
help:
	@echo "PVTOL Makefile Help:"
	@echo ""
	@echo "debug             default when you type make"
	@echo "release           builds with the release flag"
	@echo "clean             removes all objects"
	@echo "cleanAll          removes all objects, libs, test objects, logs and bins"
	@echo ""
	@echo "test/runUnitTests       runs the unit tests"
	@echo "testR/unitTestsRelease  make the tests with the release flag"
	@echo "cleanUnitTests          cleans up the specified test(s)"
	@echo "utAll             runs cleanUnitTests, unitTests, runUnitTests"
	@echo "runVUnitTests     same as runUnitTests - shows test output"
	@echo "utVAll            same as utAll - shows test output" 
	@echo "utRAll            same as utAll - runs with Release flag"
	@echo ""
	@echo "atlasPP           builds the atlas preprocessor"
	@echo ""
	@echo "The following commands submit results to the dash website"
	@echo "  experimental commands should be used for testing"
	@echo "  nightly      commands are used for automated tests"
	@echo "  continuous   command is used for automated tests during the day"
	@echo "experimentalBuild    builds libs, tests, bins and runs tests"
	@echo "experimental         cleans then runs experimentalBuild"
	@echo "experimentalRBuild   same as experimentalBuild but with release flag"
	@echo "experimentalRelease  same as experimental but with release flag"
	@echo "nightlyBuild"
	@echo "nightly"
	@echo "nightlyRBuild"
	@echo "nightlyRelease"
	@echo "continuous"
	@echo "continuousRelease"
	@echo ""
	@echo "doxygen        builds html documents and pdf file, documents are"
	@echo "                 stored in ../doc/doxygen/doc" 
	@echo "doxygenrelease builds doxygen with no private members"
	@echo "                 stored in ../doc/doxygen/docRel"
	@echo "dist           Create a distribution in the install directory"
	@echo "smalldist      Creates a small distribution with only headers and"
	@echo "                 release lib"
	@echo ""
	@echo "mpiBenchmark   build and run the mpi benchmarks"
	@echo "mpiBenchmarkHelp print the readme for the mpi benchmarks"
	@echo ""
	@echo "To build and run just one test use TEST=<dir> - dir is case sensitive"
	@echo "      ex: make unitTests TEST=Barrier"
	@echo "          make runUnitTests"
	@echo "      ex2: make utAll TEST=Barrier"
	@echo ""
	@echo "To Cross compile for the Cell use TARGET=CELL"
	@echo "To build for the SBC for RAPID use TARGET=SBC"
	@echo "To build only the library, not tests use BUILD_TESTS=false"
	@echo "To build with coverage flags use COVERAGE=true"
	@echo "   Detailed coverage files showing the missed lines are located in"
	@echo "   $(LIB_BIN)/Testing/CoverageInfo"
	@echo "To run PVTOL with MPICH2 use MPICH2=true"
	@echo "To build for other systems set SYSTEM ex: SYSTEM=DTX"
	@echo ""
	@echo " Also see 'make helpApps'"

#############################################################################
# Application building targets
# 
apps: setup
	mkdir -p $(APP_BIN)
	$(CMAKE_COMMAND) -E chdir $(APP_BIN) $(CMAKE_COMMAND) ../applications $(DEBUGFLAG) $(CROSSFLAGS)
	$(MAKE) -C $(APP_BIN)

appsRelease: setup
	mkdir -p $(APP_BIN)
	$(CMAKE_COMMAND) -E chdir $(APP_BIN) $(CMAKE_COMMAND) ../applications $(RELEASEFLAG) $(CROSSFLAGS)
	$(MAKE) -C $(APP_BIN)

runApps:
	-$(MAKE) -C $(APP_BIN) test

runVApps:
	-$(CMAKE_COMMAND) -E chdir $(APP_BIN) $(CTEST_COMMAND) -V --force-new-ctest-process

cleanApps:
	-rm -fr $(APP_BIN)

helpApps:
	@echo "make apps help"
	@echo "apps              build the /applications directory"
	@echo "appsRelease       build the /applications directory release mode"
	@echo "runApps           run the applications in the applications dir"
	@echo "runVApps          run the apps in verbose mode"
	@echo "cleanApps         cleans up the $(SRC_TO_BUILD_APP) directory"
