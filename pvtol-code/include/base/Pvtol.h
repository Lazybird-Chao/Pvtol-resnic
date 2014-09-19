/**
 *    File: Pvtol.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   This file is included by the client
 *			 This is the only file the client should include in his program
 *  $Id: Pvtol.h 938 2009-02-18 17:39:52Z ka21088 $
 *  $HeadURL: http://cvs102/svn/pvtol/branch/pvtol-gpu/code/include/base/Pvtol.h $
 *
 */
#ifndef PVTOL_H_
#define PVTOL_H_

//TOP
#include <PvtolBasics.h>

//This is for client includes
//A
#include <AutoMap.h>
#include <AutoTemporalMap.h>
//B
#include <Barrier.h>
//BlockDist.h (see Dist)
//BlockCyclicDist.h (see Dist)
#include <BlockingCQueue.h>
//C
#include <CommScope.h>
#include <Conduit.h>
#include <MulticastConduit.h>
//CyclicDist.h (see Dist)
//D
#include <DataDistDescription.h>
//DataMap.h (see Map)
//Dist
//All four must be within same file to work for MapTest
#include <BlockDist.h>
#include <BlockCyclicDist.h>
#include <CyclicDist.h>
#include <ReplicatedDist.h>
//F
//G
#include <Grid.h>
//H
//#include <HierArray.h>
#include <hUtil.h>
#include <HeterogeneousQueue.h>
#include <HeterogeneousConduit.h>
//I
//InterThreadXferData.h(see Xfer)
//M
#include <MachineModel.h>
#include <Matrix.h>
//Map
#include <Map.h>
#include <DataMap.h>
#include <RuntimeMap.h>
#include <TemporalMapBase.h>
//N
//P
//PVTOL
#include <PvtolBase.h>
//#include <PvtolBasics.h> (see top)
#include <PvtolProgram.h>

//R
#include <RankList.h>
#include <Route.h>
//ReplicatedDist.h (see Dist)
//RuntimeMap.h (see Map)
//S
#include <SharedConduit.h>
//T
#include <Task.h>
#include <TaskBarrierData.h>
#include <TaskBase.h>
#include <TaskMap.h>
#include <TaskManager.h>
#include <Timer.h>
#include <Transfer.h>
//TemporalMapBase.h (see Map)
//V
#include <Vector.h>
#include <version.h>
//Xfer
#include <CdtLocalXferData.h>
#include <InterThreadXferData.h>
#include <XferDealer.h>

#include <vector>
#include <iostream>
namespace pvtol=ipvtol;

//timing method
#include "helper_timer.h"

#endif /*PVTOL_H_*/
