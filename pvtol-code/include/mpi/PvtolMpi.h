/** @file	PvtolMpi.h
*  @author	Sanjeev Mohindra
*  @date	1/7/2008
*  @brief	PVTOL library object implementation for mpi

*/
#ifndef PVTOLMPI_H_
#define PVTOLMPI_H_

#include <mpi.h>
#include <PvtolBase.h>

#include <string>
#include <vector>

namespace ipvtol { 
namespace mpi {	
	
	/**
	 *  \brief Pvtol is a reference counted singleton that starts us the mpi network
	 * 
	 * It provides functionality similar to MPI_Initialize and MPI_Finalize
	 */
	class Pvtol: public ipvtol::PvtolBase {

	public:
	    
		///Constructors
		Pvtol();
		/**
		 * \todo Use getopt to parse the command line
		 */		
		Pvtol(int& argc, char** &argv);
		
		///Destructor 
		~Pvtol() ; 
		
		///Methods
			    
	    /**
	     * \brief Returns the number of workers
	     */
	    int numProcs(); 
	    int numProcs(MPI_Comm mpiComm); 
	    

	    /**
	     * \brief returns the rank
	     */
		int rank();
		
		int rank(MPI_Comm comm);
		
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
			static void initialize(int& argc, char** &argv);///< Named constructor object
			static void finalize(); ///<Named destructor object
			
			static int n_workers; ///< number of workers

	};

} //Namespace mpi	
} //Namespace ipvtol


#endif

