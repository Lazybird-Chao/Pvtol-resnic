/** @file	PvtolMpi.cc
 *  @author	Sanjeev Mohindra
 *  @date	1/2007
 *  @brief	MPI Network class implementation

*/
#include <PvtolMpi.h>

namespace ipvtol{
namespace mpi{

int Pvtol::nCount = 0;
int Pvtol::n_workers = 0;

Pvtol::Pvtol()
{
	int argc = 1;
	char* argv_storage[2];
	char** argv = argv_storage;
	argv[0] = "pvtolmpi";
	argv[1]= NULL;
	initialize(argc, argv);
}

Pvtol::Pvtol(int &argc, char ** &argv)
{
	initialize(argc, argv);
}


void Pvtol::initialize (int& argc, char ** &argv)
{
	if (nCount ++ !=0)
		return;
	int required;
	//TODO:
	//Throw exception in case of failure
#ifndef MULTIPLE_MPI
	required = MPI_THREAD_SINGLE;//SINGLE // MULTIPLE;
#else
	required = MPI_THREAD_MULTIPLE;
#endif

	int provided =0;
	//MPI_Init(&argc, &argv);
	MPI_Init_thread(&argc, &argv,required, &provided);

	std::cout << "MPI provider = " << provided << std::endl;
}

void Pvtol::finalize()
{
	if (--nCount !=0)
		return;

	//TODO:
	MPI_Finalize();
}

Pvtol::~Pvtol()
{
	finalize();
}

int Pvtol::getProcName(int rank, std::string& name)
{
	//TODO:
	name = "Unknown";
	return -1;
}


/**
 * \brief Returns the number of workers
 */
int Pvtol::numProcs() {
	//TODO:
	int nprocs =0;
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	return nprocs;
}

int Pvtol::numProcs(MPI_Comm comm) {
	//TODO:
	int nprocs =0;
	MPI_Comm_size(comm,&nprocs);
	return nprocs;
}

/**
 * \brief returns the rank
 */
int Pvtol::rank()
{
	//TODO:
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return rank;
}

int Pvtol::rank(MPI_Comm comm)
{
	//TODO:
	int rank;
	MPI_Comm_rank(comm, &rank);
	return rank;
}

void Pvtol::printStatus()
{
	//TODO
}

} //namespace mpi
}//namespace ipvtol
