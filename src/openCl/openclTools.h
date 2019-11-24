/*
 * openclTools.h
 *
 *  Created on: Nov 15, 2019
 *      Author: olep
 */

#ifndef OPENCL_OPENCLTOOLS_H_
#define OPENCL_OPENCLTOOLS_H_
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)
namespace std{
extern cl_kernel add;
void initializeOpencl();
}
#endif /* OPENCL_OPENCLTOOLS_H_ */
