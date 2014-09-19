/**
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \file	PvtolBase.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   PVTOL library object implementation - Base class
 *
 *  $Id: PvtolBase.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */

#ifndef PVTOLBASE_H_
#define PVTOLBASE_H_

#include <PvtolBasics.h>
#include <string>


namespace ipvtol{


	/**
	 * @brief The common interface to the PVTOL object
	 * 
	 */
	class PvtolBase
	{

	public :
		///Get the local processor rank of the current processor
		virtual int rank() = 0;
		
		//Get the ProcID rank of the current processor
		//virtual ProcId getProcId() = 0;
		
		///Get the name of the current processor, given the local rank
		virtual int getProcName(int rank, std::string& name) = 0;
		
		///Get the number of processors in the current level
		virtual int numProcs() = 0;
		
		//Debugging aids
		
		///Print out information about the network
		virtual void printStatus() = 0;			
		
		///dtor
		virtual ~PvtolBase(){}
		
	private:

	};
	
}//namespace ipvtol
#endif
