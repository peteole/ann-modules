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
	/*void makeChildOf(PlugOut *parent, int firstOutputToUse) {
		input = new double*[inputs];
		for (int i = 0; i < inputs; i++) {
			input[i] = &(parent->output[firstOutputToUse + i]);
			parent->dEdOut[i + firstOutputToUse] = &(dEdIn[i]);
		}
	}*/
	NeuralNetwork(int inputs, int outputs) :
			PlugOut(outputs), PlugIn(inputs) {

	}
	template<typename T> void toArray(T **toConvert, T * &target, int len) {
		//T *toReturn = new T[len];
		for (int i = 0; i < len; i++) {
			target[i] = *toConvert[i];
		}
		//return toReturn;
	}
	virtual void updateOutput()=0;
	virtual void addDerivatives()=0;
	virtual void updateParameters(double alpha)=0;
	virtual ~NeuralNetwork(){};
	// ~NeuralNetwork()=0;
};

#endif /* NEURALNETWORK_H_ */
