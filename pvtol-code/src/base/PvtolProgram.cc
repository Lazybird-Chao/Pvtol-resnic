/**
 * File: PvtolProgram.cc
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *  \file	PvtolProgram.cc
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief	PVTOL library object implementation
 *  @todo   Make some functions inline and put in inl file
 *  @todo   Exception and Error handling
 *  $Id: PvtolProgram.cc 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#include <PvtolProgram.h>
#include <RootTask.h>
#include <iostream>

namespace ipvtol{

int PvtolProgram::m_nCount = 0;
TaskId PvtolProgram::m_rootTaskId = 0;
std::vector<PvtolBase*> PvtolProgram::m_providers;

//This simply initializes the network based on the default Machine model
//Todo: Kill this constructor??
PvtolProgram::PvtolProgram()
{
	int argc = 1;
	char* argv_storage[2];
	char** argv = argv_storage;
	argv[0] = "Pvtol";
	argv[1]= NULL;
	initialize(argc, argv);
}

PvtolProgram::PvtolProgram(int& argc, char** &argv)
{
	initialize(argc, argv);
}


//destructor
PvtolProgram::~PvtolProgram()
{
	finalize();
}	

//Return the model with the given providerId
PvtolBase* PvtolProgram::getProvider(int providerId)
{
	return m_providers[providerId];
}


TaskManager* PvtolProgram::getTaskManager()
{
	static TaskManager* taskManager  = TaskManager::Instance();
	return taskManager;
}	

MachineModel* PvtolProgram::getMachineModel()
{
	static MachineModel* machineModel = new MachineModel;
	return machineModel;
}			

//Number of processors, processor ranks, and processor set
int PvtolProgram::numProcs(int providerId)
{
	return m_providers[providerId]->numProcs();
}

int PvtolProgram::rank(int providerId )
{
	return m_providers[providerId]->rank();
}

ProcId PvtolProgram::getProcId(int providerId )
{
	return m_providers[providerId]->rank();
}


//Task related convenience items
TaskId PvtolProgram::getCurrentTaskId()
{
	TaskManager* taskManager = getTaskManager();
	return taskManager->getCurrentTaskId();
}


TaskId PvtolProgram::getParentTaskId()
{
	TaskManager* taskManager = getTaskManager();
	return taskManager->getParentTaskId();
}

TaskBase& PvtolProgram::getCurrentTask()
{
	TaskManager* taskManager = getTaskManager();
	return taskManager->getCurrentTask();
}

TaskBase& PvtolProgram::getParentTask()
{
	TaskManager* taskManager = getTaskManager();
	return taskManager->getParentTask();
}
/*
std::vector<int> Pvtol::processorSet(int id)
{
	return  providers[id]->processorSet();
}
*/
	
//Debugging aids

int PvtolProgram::getProcName(int rank, std::string& name, int providerId)
{
	name = "Unknown";
	return -1;
}

///Get version number
std::string PvtolProgram::getVersion()
{
	return PVTOL_VERSION; //defined in version.h
}
//print out information about the network
void PvtolProgram::printStatus()
{
	//
	std::vector<PvtolBase*>::const_iterator iter;

	for (iter = m_providers.begin(); iter!=m_providers.end();++iter)
	{
		(*iter)->printStatus();
	}					
}		


void PvtolProgram::initialize (int& argc, char * argv[])
{
	//Only the root task can change the state
	if (m_nCount && TaskManager::getCurrentTaskId() != m_rootTaskId)
		return;
	
	if (m_nCount ++ !=0)
		return;

	//std::cout<<"\nInitializing system\n";	
	
	//Todo: Put in try catch block
	
	//Initialize the models from the machine model	
	//ToDo: Implement machine model format read, write & decode
	//Do the initialization
	
	//Do like this when machine model is built 
	//				std::string filename = "mm.xml"; //get from argv
	//				machineModel = new MachineModel(filename);

	MachineModel* machineModel = getMachineModel();
	m_providers = machineModel->Initialize(argc,argv);
	
	//Initialize the TaskManager here
	TaskManager* mgr  = getTaskManager();
	
	int nProcs = m_providers[0]->numProcs();
	
	RootTask* root = new RootTask(nProcs);
	m_rootTaskId = mgr->registerTask(root);
}



void PvtolProgram::finalize()
{
	//Only the root task can change the state
	if (TaskManager::getCurrentTaskId() != m_rootTaskId)
		return;
	
	if (--m_nCount !=0 )
		return;


	//Todo: Put in try catch block and eat the exception
	
	//std::cout << "\nFinalizing system\n";
	std::vector<PvtolBase*>::const_iterator iter;
	for (iter = m_providers.begin(); iter!=m_providers.end();++iter)
	{
		delete (*iter);
	}
	
}

}//namespace ipvtol
