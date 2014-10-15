/*
 * matrixmulti.h
 *
 *  Created on: Dec 9, 2013
 *      Author: Chao
 */

#ifndef MATRIXMULTI
#define MATRIXMULTI

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#include <Pvtol.h>
//pvtol=ipvtol if include Pvtol.h
using namespace pvtol;

class MatrixMulti{
public:
	MatrixMulti();

	void init(int size, int sub_size, float *A, float *B, float *C);

	int run();

	float *A, *B, *C;
	int size;

	/*char *s;
	void init(char *s);*/
};


#endif

