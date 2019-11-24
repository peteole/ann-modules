/*
 * OpenClExecuter.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: olep
 */

#include "OpenClExecuter.h"

namespace std {
cl_kernel OpenClExecuter::loadKernel(char *srcpath, char *name) {
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen(srcpath, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	cl_int ret;
	source_str = (char*) malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	printf("kernel loading done\n");
	printf("before building\n");
	// Create a program from the kernel source
	return loadKernelSrc(source_str, name,source_size);
	/*cl_program program = clCreateProgramWithSource(context, 1,
			(const char**) &source_str, (const size_t*) &source_size, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	return clCreateKernel(program, name, &ret);*/
}
cl_kernel OpenClExecuter::loadKernelSrc(const char *src,const char * name,size_t len){
	cl_int ret;
	cl_program program = clCreateProgramWithSource(context, 1,
			(const char**) &src, (const size_t*) &len, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	return clCreateKernel(program, name, &ret);
}
int* OpenClExecuter::add(int *A, int *B, int length) {
	cl_int ret;
	// Create memory buffers on the device for each vector
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
			length * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
			length * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
			length * sizeof(int), NULL, &ret);

	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
			length * sizeof(int), A, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
			length * sizeof(int), B, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	printf("before building\n");

	printf("ret at %d is %d\n", __LINE__, ret);
	// Set the arguments of the kernel
	ret = clSetKernelArg(vectorAdd, 0, sizeof(cl_mem), (void*) &a_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(vectorAdd, 1, sizeof(cl_mem), (void*) &b_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(vectorAdd, 2, sizeof(cl_mem), (void*) &c_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);
	//added this to fix garbage output problem
	//ret = clSetKernelArg(kernel, 3, sizeof(int), &length);

	printf("before execution\n");
	// Execute the OpenCL kernel on the list
	size_t global_item_size = length; // Process the entire lists
	size_t local_item_size = 64; // Divide work items into groups of 64
	ret = clEnqueueNDRangeKernel(command_queue, vectorAdd, 1, NULL,
			&global_item_size, NULL, 0, NULL, NULL);
	printf("after execution\n");
	// Read the memory buffer C on the device to the local variable C
	int *C = (int*) malloc(sizeof(int) * length);
	ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
			length * sizeof(int), C, 0, NULL, NULL);
	return C;
}
void OpenClExecuter::matrixVectorMult(cl_mem r_mem_obj, cl_mem a_mem_obj,
		cl_mem b_mem_obj, int columns, int rows){
	cl_int ret;
	ret = clSetKernelArg(matrixVectorMultKernel, 0, sizeof(cl_mem),
			(void*) &r_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(matrixVectorMultKernel, 1, sizeof(cl_mem),
			(void*) &a_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(matrixVectorMultKernel, 2, sizeof(cl_mem),
			(void*) &b_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);
	cl_int col=columns;
	ret = clSetKernelArg(matrixVectorMultKernel, 3, sizeof(cl_int),
			(void*)&col);
	size_t global_item_size = rows;
	ret = clEnqueueNDRangeKernel(command_queue, matrixVectorMultKernel, 1, NULL,
			&global_item_size, NULL, 0, NULL, NULL);
}
void OpenClExecuter::matrixVectorMult(float *resultVector, float *matrixA,
		float *vectorB, int columns, int rows) {
	cl_int ret;
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
			columns * rows * sizeof(float), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
			columns * sizeof(float), NULL, &ret);
	cl_mem r_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
			rows * sizeof(float), NULL, &ret);


	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
			columns * rows * sizeof(float), matrixA, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
			columns  * sizeof(float), vectorB, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	matrixVectorMult(r_mem_obj, a_mem_obj, b_mem_obj, columns, rows);
	ret = clEnqueueReadBuffer(command_queue, r_mem_obj, CL_TRUE, 0,
			rows * sizeof(float), resultVector, 0, NULL, NULL);
}
OpenClExecuter::OpenClExecuter() {
	// Get platform and device information
	device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*) malloc(
			ret_num_platforms * sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_ALL, 1, &device_id,
			&ret_num_devices);
	printf("ret at %d is %d\n", __LINE__, ret);
	// Create an OpenCL context
	context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Create a command queue
	this->command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);
	this->vectorAdd = loadKernel("src/openCl/vector_add_kernel.cl",
			"vector_add");
	this->matrixVectorMultKernel = loadKernel(
			"src/openCl/matrix_vector_mult_kernel.cl", "matrix_vector_mult");
}
} /* namespace std */
