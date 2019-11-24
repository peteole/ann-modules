/*
 * OCLNetwork.cpp
 *
 *  Created on: Nov 24, 2019
 *      Author: olep
 */

#include "OCLNetwork.h"

namespace std {
OCLNetwork::OCLNetwork(int *neuronsInLayer, int numOfLayers, string a,
		string as) :
		NeuralNetwork(neuronsInLayer[0], neuronsInLayer[numOfLayers - 1]), OpenClExecuter() {
	string srcA =
			"__kernel void a(__global float* in, __global float *y){int index=get_global_id(0); float x=in[index]; y[index]=";
	srcA.append(a);
	srcA.append(";}");
	this->a = loadKernelSrc(srcA.c_str(), "a", (size_t) srcA.length());

	string srcAs =
			"__kernel void as(__global float* in, __global float *y){int index=get_global_id(0); float x=in[index]; y[index]=";
	srcAs.append(as);
	srcAs.append(";}");
	this->as = loadKernelSrc(srcAs.c_str(), "as", (size_t) srcA.length());
}
void OCLNetwork::applyA(cl_mem a, cl_mem result, size_t len) {
	cl_int ret;
	ret = clSetKernelArg(this->a, 0, sizeof(cl_mem), (void*) &a);
	ret = clSetKernelArg(this->a, 1, sizeof(cl_mem), (void*) &result);

	ret = clEnqueueNDRangeKernel(command_queue, this->a, 1, NULL, &len, NULL, 0,
	NULL, NULL);
}
void OCLNetwork::applyA(float *a, float *result, size_t len) {
	cl_int ret;
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
			len * sizeof(float), NULL, &ret);
	cl_mem r_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
			len * sizeof(float), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
			len*sizeof(float), a, 0, NULL, NULL);
	applyA(a_mem_obj, r_mem_obj, len);
	ret = clEnqueueReadBuffer(command_queue, r_mem_obj, CL_TRUE, 0,
			len * sizeof(float), result, 0, NULL, NULL);

}
} /* namespace std */
