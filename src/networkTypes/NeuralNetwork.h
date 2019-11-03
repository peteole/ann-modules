/*
 * NeuralNetwork.h
 *
 *  Created on: Jul 1, 2019
 *      Author: olep
 */
#ifndef NETWORKTYPES_NEURALNETWORK_H_
#define NETWORKTYPES_NEURALNETWORK_H_

#include "PlugIn.h"
#include "PlugOut.h"

using namespace std;

class NeuralNetwork: public PlugOut, public PlugIn {
public:
	NeuralNetwork(int inputs, int outputs) :
			PlugOut(outputs), PlugIn(inputs) {

	}
	virtual NeuralNetwork* clone()=0;
	virtual void copyParameters(NeuralNetwork *const toCopy)=0;
	virtual void addDerivatives(NeuralNetwork *const toAdd)=0;
	virtual void updateOutput()=0;
	virtual void addDerivatives()=0;
	virtual void updateParameters(double alpha)=0;
	virtual void updateParameters(void (*updateFunction)(double &derivative,double &oldValue, char* parameters))=0;
	virtual void createBufferStorage(int numOfBytes)=0;
	virtual ~NeuralNetwork(){};
	// ~NeuralNetwork()=0;
};

#endif /* NETWORKTYPES_NEURALNETWORK_H_ */
