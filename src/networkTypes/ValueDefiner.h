/*
 * ValueDefiner.h
 *
 *  Created on: Oct 10, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_VALUEDEFINER_H_
#define NETWORKTYPES_VALUEDEFINER_H_

using namespace std;
//#include "NetworkContainer.h"
//class NetworkContainer;
#include "NetworkContainer.h"
//#include "NeuralNetwork.h"

class ValueDefiner: public NeuralNetwork {
public:
	ValueDefiner(int channels, NetworkContainer *container);
	virtual double getValue() {
		return 0;
	}
	virtual double* getDValueDIn() {
		double *d = new double[inputs];
		double oldValue = getValue();
		for (int i = 0; i < inputs; i++) {
			output[i] += 0.001;
			d[i] = (getValue() - oldValue) / 0.001;
			output[i] -= 0.001;
		}
		return d;
	}
	void updateOutput();
	void updateParameters(double alpha) override{

	}
	void updateParameters(void (*updateFunction)(double &derivative,double &oldValue, char* parameters))override{

	}
	void createBufferStorage(int numOfBytes)override{

	}
	void addDerivatives() {
		double *d = getDValueDIn();
		for (int i = 0; i < inputs; i++) {
			dEdIn[i] = *(dEdOut[i]) + d[i];
		}
		delete[] d;
	}
	void copyParameters(NeuralNetwork *const toCopy)override{

	}
	void addDerivatives(NeuralNetwork *const toCopy)override{

	}
	NetworkContainer *container;
};

 /* namespace std */

#endif /* NETWORKTYPES_VALUEDEFINER_H_ */
