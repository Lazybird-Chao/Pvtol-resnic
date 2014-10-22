/*
 * main.cc
 *
 *  Created on: Dec 9, 2013
 *      Author: Chao
 */
#include <Pvtol.h>
//pvtol=ipvtol if include Pvtol.h
using namespace pvtol;

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/time.h>
using namespace std;
#include <sys/types.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "matrixmulti.h"

int main(int argc, char*argv[])
{
	Timer timer;
	float runtime=0.0;

	PvtolProgram prog(argc, argv);
	//cout<<prog.numProcs()<<"  "<<prog.rank()<<endl;
	int matrix_size;
	int thread_size;
	if(argc!=3)
	{
		cout<<"need matrix_size and thread_size!"<<endl;
		return -1;
	}
	matrix_size=atoi(argv[1]);
	thread_size=atoi(argv[2]);
	cout<<matrix_size<<"    "<<thread_size<<endl;
	//PvtolProgram prog2(argc, argv);
	//cout<<prog2.numProcs()<<"  "<<prog2.rank()<<endl;

	vector <RankId> rank;
	int i;
	for(i=0;i<thread_size;i++)
		rank.push_back(0);
	/*rank.push_back(0);
	rank.push_back(1);*/
	RankList ranks(rank);
	TaskMap mymap(ranks);

	Task<MatrixMulti> matrixmultiply("matixmultiply", mymap);

		/*cout<<"taskid:"<<matrixmultiply.getTaskID()<<"  ";
		cout<<"parentid:"<<matrixmultiply.getParentTaskID()<<"  ";
		cout<<"numprocesses:"<<matrixmultiply.getNumProcesses()<<"  ";
		cout<<"processrank:"<<matrixmultiply.getProcessRank()<<"  ";
		cout<<"numranks:"<<matrixmultiply.getNumRanks()<<"  ";
		cout<<"numlocalthreads:"<<matrixmultiply.getNumLocalThreads()<<"  ";
		cout<<"localthreadrank:"<<matrixmultiply.getLocalThreadRank()<<"  ";
		cout<<"globalthreadrank:"<<matrixmultiply.getGlobalThreadRank();
		cout<<endl<<endl;*/

	float *A=(float *)malloc(matrix_size*matrix_size*sizeof(float));
	float *B=(float *)malloc(matrix_size*matrix_size*sizeof(float));
	float *C=(float *)malloc(matrix_size*matrix_size*sizeof(float));

	int sub_size=matrix_size/thread_size;
	matrixmultiply.init(matrix_size,sub_size, A, B, C);

	timer.start();
	matrixmultiply.run();
	//Barrier b;
	//b.synch();
	cout<<"here in main thread"<<endl;
	matrixmultiply.waitTillDone();
	timer.stop();

	/* A task object can only be initialized and ran for one time!
	matrixmultiply.init(matrix_size,sub_size, A, B, C);
	matrixmultiply.run();
	matrixmultiply.waitTillDone();
	*/
	runtime=(float)timer.value()/1000.0;
	cout<<"task runing time is "<<runtime<<"ms"<<endl;

	int j,k;
	float *C_compare=(float *)malloc(matrix_size*matrix_size*sizeof(float));
	for(i=0;i<matrix_size;i++)
			for(j=0;j<matrix_size;j++)
			{
				C_compare[i*matrix_size+j]=0.0;
				for(k=0;k<matrix_size;k++)
					C_compare[i*matrix_size+j]+=A[i*matrix_size+k]*B[k*matrix_size+j];
				if(C_compare[i*matrix_size+j]!=C[i*matrix_size+j])
				{
					cout<<"error in computing "<<i<<","<<j<<endl;
					break;
				}
			}
	if(j==matrix_size) cout<<"all right\n";

	/*cout<<getpid()<<";  "<<syscall(SYS_gettid)<<endl;
	char *s="abcde";
	matrixmultiply.init(s);
	matrixmultiply.run();
	matrixmultiply.waitTillDone();*/

	return 0;
}



