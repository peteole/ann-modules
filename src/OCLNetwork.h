/*
 * OCLNetwork.h
 *
 *  Created on: Nov 24, 2019
 *      Author: olep
 */

#ifndef OCLNETWORK_H_
#define OCLNETWORK_H_


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "openCl/OpenClExecuter.h"
#include "networkTypes/NeuralNetwork.h"

namespace std {

class OCLNetwork: public NeuralNetwork, public OpenClExecuter {
public:
	OCLNetwork(int *neuronsInLayer, int numOfLayers, string a, string as);

	void applyA(cl_mem a,cl_mem result, size_t len);
	void applyA(float* a,float* result, size_t len);
	NeuralNetwork* clone(){return 0;};
	void copyParameters(NeuralNetwork *const toCopy){};
	void addDerivatives(NeuralNetwork *const toAdd){};
	void updateOutput(){};
	void addDerivatives(){};
	void updateParameters(double alpha){};
	void updateParameters(void (*updateFunction)(double &derivative,double &oldValue, char* parameters)){};
	void createBufferStorage(int numOfBytes){};
private:
	cl_kernel a;
	cl_kernel as;
};

} /* namespace std */

#endif /* OCLNETWORK_H_ */
