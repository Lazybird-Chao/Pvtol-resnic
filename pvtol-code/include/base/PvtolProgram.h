/**
 *    File: PvtolProgram.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *  \file	 PvtolProgram.h
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   PVTOL library Program Object  
 *
 *  $Id: PvtolProgram.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef PVTOLPROGRAM_H_
#define PVTOLPROGRAM_H_



#ifdef _STANDARD_MPI
#include <mpi.h>
#endif //  _STANDARD_MPI

#include <PvtolBase.h>
#include <TaskManager.h>
#include <MachineModel.h>

#include <version.h>
#include <vector>
#include <string>

namespace ipvtol{

/**
 * \brief The PvtolProgram class manages the initialization of the network
 */	
class PvtolProgram
	{

		public:
		///This simply initializes the network based on the default Machine model
		PvtolProgram();

		///This uses the command line args to initialize the network
		//Pass argc,argv by reference
		PvtolProgram(int &argc, char** &argv);

		///Shut down the network
		~PvtolProgram();

		///Get the task manager
		static TaskManager* getTaskManager();

		///Get the Machine model
		static MachineModel* getMachineModel();
			
		///Return the model for the supplied identifier.
		//TODO: This identifier will get clearer after the machine model is built
		PvtolBase* getProvider(int providerId = 0 );
		
		///Local rank of the processor 
		int rank(int providerId = 0 );
				
		///Proc id (same as above?)
		int getProcId(int providerId = 0 );		
		
		///Number of processors for the given machine id 
		int numProcs(int providerId = 0 );
		


		//Task related convenience items
		
		/// Get the current Task Id
		TaskId getCurrentTaskId();
		
		/// Get the current Task	
		TaskBase& getCurrentTask();

		/// Get the parent Task Id
		TaskId getParentTaskId();
		
		/// Get the Parent Task	
		TaskBase& getParentTask();		
		
		//Debugging aids
		
		///Get processor name
		int getProcName(int rank, std::string& name, int providerId = 0);

		///Print out information about the network
		void printStatus();
		
		///Get version number
		std::string getVersion();
		
		private:
		
		/// Initialize the network
		static void initialize (int& argc, char * argv[]);
		
		///Shut down the network
		static void finalize();	
		
		///The number of users of the PvtolProgram object
		static int m_nCount; 
		
		//A vector of PvtolBase pointer objects
		//TODO: This should be built up using the machine model, but we 
		//are currently limiting it to two layers
		static std::vector<PvtolBase*> m_providers;
		
		static TaskId m_rootTaskId;


	};

}//namespace ipvtol
#endif
