/*
 * matrixmulti.cc
 *
 *  Created on: Dec 9, 2013
 *      Author: Chao
 */

#include "matrixmulti.h"
#include <sys/types.h>
#include <pthread.h>
#include <sys/syscall.h>

MatrixMulti::MatrixMulti()
{
	//cout<<"using init to start\n";

}

void MatrixMulti::init(int n, int sub_n,float *in_A, float *in_B, float *out_C)
{
	PvtolProgram prog;
	TaskBase& mytask=prog.getCurrentTask();
	A=in_A;
	B=in_B;
	C=out_C;
	size=n;
	int myrank=mytask.getLocalThreadRank();
	srand(time(0));
	int i,j;
	for(i=myrank*sub_n;i<(myrank+1)*sub_n;i++)
		for(j=0;j<size;j++)
		{
			A[i*size+j]=(rand()%100)/100.0;
			B[i*size+j]=(rand()%100)/100.0;
		}
	sleep(myrank);
	Barrier	bar;
	//bar.synch();
	cout<<myrank<<endl;

}

int MatrixMulti::run()
{
	PvtolProgram prog;
	TaskBase& mytask=prog.getCurrentTask();
	int myrank=mytask.getLocalThreadRank();
	int thread_size=mytask.getNumLocalThreads();
	int sub_size=size/thread_size;
	int i, j, k;
	for(i=myrank*sub_size;i<(myrank+1)*sub_size;i++)
		for(j=0;j<size;j++)
		{
			C[i*size+j]=0.0;
			for(k=0;k<size;k++)
				C[i*size+j]+=A[i*size+k]*B[k*size+j];
		}
	/*if(s)
		cout<<s<<";  "<<getpid()<<";  "<<syscall(SYS_gettid)<<endl;*/
	sleep(myrank);
	Barrier	bar;
	bar.synch();
	cout<<myrank<<endl;

	return 0;
}

/*void MatrixMulti::init(char *in_s)
{
	s=in_s;
	PvtolProgram prog;
	TaskBase& mytask=prog.getCurrentTask();
	cout<<"taskid:"<<mytask.getTaskID()<<"  ";
			cout<<"parentid:"<<mytask.getParentTaskID()<<"  ";
			cout<<"numprocesses:"<<mytask.getNumProcesses()<<"  ";
			cout<<"processrank:"<<mytask.getProcessRank()<<"  ";
			cout<<"numranks:"<<mytask.getNumRanks()<<"  ";
			cout<<"numlocalthreads:"<<mytask.getNumLocalThreads()<<"  ";
			cout<<"localthreadrank:"<<mytask.getLocalThreadRank()<<"  ";
			cout<<"globalthreadrank:"<<mytask.getGlobalThreadRank();
			cout<<endl<<endl;
}*/


