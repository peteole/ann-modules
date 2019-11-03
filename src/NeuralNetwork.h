/*
 * NeuralNetwork.h
 *
 *  Created on: Jul 1, 2019
 *      Author: olep
 */
#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_

#include "PlugOut.h"
#include "PlugIn.h"

using namespace std;

class NeuralNetwork: public PlugOut, public PlugIn {
public:
	NeuralNetwork(int inputs, int outputs) :
			PlugOut(outputs), PlugIn(inputs) {

	}
	virtual NeuralNetwork* clone()=0;
	virtual void copyParameters(NeuralNetwork *const toCopy)=0;
	virtual void updateOutput()=0;
	virtual void addDerivatives()=0;
	virtual void updateParameters(double alpha)=0;
	virtual ~NeuralNetwork(){};
	// ~NeuralNetwork()=0;
};

#endif /* NEURALNETWORK_H_ */
