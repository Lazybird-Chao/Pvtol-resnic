/**
 *    File: MachineModel.h
 *
 *  Copyright (c) 2007, Massachusetts Institute of Technology
 *  All rights reserved.
 *
 *  \author  $LastChangedBy: ka21088 $
 *  \date    $LastChangedDate: 2009-02-18 12:39:52 -0500 (Wed, 18 Feb 2009) $
 *  \version $LastChangedRevision: 938 $
 *  \brief   This is the machinemodel class. 
 *  \todo	 This is a temporary placeholder
 *
 *  $Id: MachineModel.h 938 2009-02-18 17:39:52Z ka21088 $
 *
 */
#ifndef MACHINEMODEL_H_
#define MACHINEMODEL_H_


/****************************************/
/* THIS IS ALL TEMPORARY                */
/****************************************/	
#include <PvtolBasics.h>
#include <PvtolBase.h>


#ifdef _PVTOL_CELL
#include <PvtolCell.h>
#endif
#ifndef _PVTOL_USE_VSIPLMPI
#include <PvtolMpi.h>
#else
#include <vsiplpp/include/PvtolVsiplpp.h>
#endif

#include <vector>
#include <string>


namespace ipvtol {

class MachineModel
{
	public:
		///Default constructor
		MachineModel();
		
		///Read and instantiate the machine model from xml file
		MachineModel(std::string XmlFileName);
		
		//Initialize is called by the PVTOL object
		std::vector<PvtolBase*> Initialize(int argc, char * argv[]);
	private:
		//No assignment and copy constructor
		MachineModel(const MachineModel&);
		MachineModel& operator=(const MachineModel&);		
};

}//end namespace
#endif /*MACHINEMODEL_H_*/

