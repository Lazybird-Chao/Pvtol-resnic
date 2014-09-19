/** @file	MachineModel.cc
 *  @author	Sanjeev Mohindra
 *  @date	1/3/2008
 *  @brief	Machine Model class. To be replaced later
  
*/
#include <MachineModel.h>
#include <vector>

namespace ipvtol{
		
	
	MachineModel::MachineModel() 
	{ 
	  //... perform necessary instance initializations 
	}

	MachineModel::MachineModel(std::string XmlFileName) 
	{ 
	  //... perform necessary instance initializations 
	}
	
	std::vector<PvtolBase*> MachineModel::Initialize(int argc, char * argv[])
	{
		//currently hardcoded to a cell cluster
		std::vector<PvtolBase*> machines;
#ifndef _PVTOL_USE_VSIPLMPI
		machines.push_back(new ipvtol::mpi::Pvtol(argc,argv));
#else
		machines.push_back(new ipvtol::vsiplpp::Pvtol(argc,argv));
#endif		

		//@todo:Make sure that the network is started only once on each processor
#ifdef _PVTOL_CELL
		machines.push_back(new ipvtol::mcf::Pvtol(argc,argv));
#endif		
	
		return machines;
	}
}

