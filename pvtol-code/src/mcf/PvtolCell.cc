/** @file	PvtolCell.cc
 *  @author	Sanjeev Mohindra
 *  @date	7/20/2007
 *  @brief	Cell Network class implementation
 
*/
#ifdef _PVTOL_CELL


#include <PvtolBasics.h>
#include <PvtolCell.h>
#include <logger.h>

namespace ipvtol{
namespace mcf{
	
int Pvtol::nCount = 0;
int Pvtol::n_workers = DEFAULT_WORKERS;
int Pvtol::rc = 0;
PvtolHandle Pvtol::h_net = 0;


Pvtol::Pvtol()
{
	int argc = 1;
	char* argv_storage[2];
	char** argv = argv_storage;
	argv[0] = "pvtolcell";
	argv[1]= NULL;
	initialize(argc, argv);
}

Pvtol::Pvtol(int argc, char * argv[])
{
	initialize(argc, argv);
}


void Pvtol::initialize (int argc, char * argv[])
{
	if (nCount ++ !=0)
		return;
	
    PROGRESS("Creating global MCF object");				
    if(argc < 2) {
    	n_workers = DEFAULT_WORKERS ;
 
    }
    else{
        n_workers = atoi(argv[1]);
    }

    if (n_workers > MAX_WORKERS)
    	n_workers = MAX_WORKERS;
    
    PROGRESS("mcf_m_net_create()");
    
    rc = mcf_m_net_create(n_workers,
                          MCF_RESERVED_FLAG,
                          &h_net);
                          
    
//    MCF_M_NET_CHECK_RC(h_net, rc, MCF_ERROR_EXIT);
                       
	PROGRESS( "mcf_m_net_set_mem_heap_n_bytes()" );
	
	//TODO: Get these using getopt

	rc = mcf_m_net_set_mem_heap_n_bytes ( h_net, ( 256 * 1024 * 1024 ) );   

//    MCF_M_NET_CHECK_RC(h_net, 
//                       rc, 
//                       MCF_ERROR_EXIT);   
    
    if (rc!=MCF_SUCCESS)
    {
    	PROGRESS("Unable to create network");
    	return;
    }
    
    PROGRESS("mcf_m_net_initialize()");
    
    rc = mcf_m_net_initialize(h_net,
                              MCF_RESERVED_FLAG);
    
//    MCF_M_NET_CHECK_RC(h_net, rc, MCF_ERROR_EXIT);	
    
    if (rc!=MCF_SUCCESS)
    {
    	PROGRESS("Unable to create network");
//        int rc2 = mcf_m_net_destroy(&h_net); //destroy it
//        rc2 = mcf_m_net_create(0,MCF_RESERVED_FLAG, &h_net);//network with 0 workers
//        rc2 = mcf_m_net_set_mem_heap_n_bytes ( h_net, ( 256 * 1024 * 1024 ) ); 
//        rc2 = mcf_m_net_initialize(h_net, MCF_RESERVED_FLAG);
//        if (rc2!=MCF_SUCCESS)
        return;
    }
	
}
	
void Pvtol::finalize()
{
	if (--nCount !=0)
		return;
		
	PROGRESS("mcf_m_net_destroy()");
    rc = mcf_m_net_destroy(&h_net);
    
    MCF_M_NET_CHECK_RC(h_net, rc, MCF_ERROR_EXIT);
    
    rc = 0;
    PROGRESS("\nManager is DONE.\n\n");
}	

Pvtol::~Pvtol()
{
	finalize();
}	
		
std::vector<int> Pvtol::processorSet()
{

	static bool pset_initialized = false;
	static std::vector<int> processors;
	if (!pset_initialized)
	{
		pset_initialized = true;
		//the following is a hack which works for a global communicator
		for(unsigned int i = 0; i < numProcs(); ++i)
			processors.push_back(i);
	}
	return processors;

}

int Pvtol::getProcName(int rank, std::string& name)
{
	name = "Unknown";
	return -1;
}


/**
 * \brief Returns the number of workers
 */
int Pvtol::numProcs() {
	return n_workers;
}	


/**
 * \brief returns the rank what??
 */
int Pvtol::rank()
{
	return -1;
}

/**
 * \brief Returns the current network handle
 */
PvtolHandle Pvtol::getHandle() {
	return h_net;
}

void Pvtol::printStatus()
{
	//TODO
}

} //namespace mcf
}//namespace ipvtol
#endif
