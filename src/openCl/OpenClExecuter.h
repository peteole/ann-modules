/*
 * OpenClExecuter.h
 *
 *  Created on: Nov 17, 2019
 *      Author: olep
 */

#ifndef OPENCL_OPENCLEXECUTER_H_
#define OPENCL_OPENCLEXECUTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

namespace std {
class OpenClExecuter {
public:
	OpenClExecuter();
	int* add(int *A, int *B, int length);
	void matrixVectorMult(float *resultVector, float *matrixA,
			float *vectorB, int columns, int rows);
	void matrixVectorMult(cl_mem resultVector, cl_mem matrixA,
			cl_mem vectorB, int columns, int rows);
	cl_kernel loadKernelSrc(const char *src, const char *name,size_t len);
//private:
	cl_kernel loadKernel(char *srcpath, char *name);
	cl_context context;
	cl_command_queue command_queue;
	cl_kernel vectorAdd;
	cl_kernel matrixVectorMultKernel;
	cl_device_id device_id;
};

} /* namespace std */

#endif /* OPENCL_OPENCLEXECUTER_H_ */
