/** @file	PvtolCell.h
 *  @author	Sanjeev Mohindra
 *  @date	7/20/2007
 *  @brief	Singleton (Borg) Class for starting and shutting down a network.

@todo Use strategy pattern to work with the vsip class 
*/
#ifndef PVTOLCELL_H_
#define PVTOLCELL_H_

#include <mcf_m.h>
#include <string>
#include <vector>
#include <PvtolBase.h>

//This should go to some config file
//#define DEFAULT_WORKERS 8
//#define MAX_WORKERS 16
//#define MAX_PATH_LENGTH 256;

namespace ipvtol { 
namespace mcf {	

	
	typedef MCF_m_net_handle PvtolHandle;
	typedef MCF_alloc_desc   PvtolMemAllocHandle;
	
	/**
	 *  \brief Pvtol is a reference counted singleton that starts us the cell network
	 * 
	 * It provides functionality similar to MPI_Initialize and MPI_Finalize
	 * and the vsipl::vpp class
	 * \todo #DEFINEs for rc flag, interface with MCF
	 */
	class Pvtol: public ipvtol::PvtolBase {

	public:
	    
		///Constructors
		Pvtol();
		/**
		 * \todo Use getopt to parse the command line
		 */		
		Pvtol(int argc, char* argv[]);
		
		///Destructor 
		~Pvtol() ; 
		
		///Methods
		
		/**
		 * \brief Returns the current network handle
		 */
	    static PvtolHandle getHandle();
	    
	    /**
	     * \brief Returns the number of workers
	     */
	    int numProcs(); 
	    

	    /**
	     * \brief returns the rank what??
	     */
		int rank();

			
		/**
		 * \brief Returns a std::vector of all tile processor ranks
		 * 
		 */
		std::vector<int> processorSet() ;

		//Debugging aids
	    /**
	     * \brief Get the name of the processor
	     */		
		int getProcName(int rank, std::string& name);		
		
	    /**
	     * \brief Prints status of the network
	     */
	    void printStatus();	
	 


	private:
			
			Pvtol(const Pvtol&); ///< No copy constructor
			Pvtol& operator=(const Pvtol&); ///<No assignment operator
			static int nCount; ///< reference counter
			static void initialize(int argc, char * argv[]);///< Named constructor object
			static void finalize(); ///<Named destructor object
			
			static int n_workers; ///< number of workers
			static int rc;		  ///< return code for operations
		    static PvtolHandle h_net; ///< Current Network handle
	};

} //Namespace mcf	
} //Namespace ipvtol

#endif /*PVTOL_H_*/
