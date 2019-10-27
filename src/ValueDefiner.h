/*
 * ValueDefiner.h
 *
 *  Created on: Oct 10, 2019
 *      Author: olep
 */

#ifndef VALUEDEFINER_H_
#define VALUEDEFINER_H_
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
	void addDerivatives() {
		double *d = getDValueDIn();
		for (int i = 0; i < inputs; i++) {
			dEdIn[i] = *(dEdOut[i]) + d[i];
		}
		delete[] d;
	}
private:
	NetworkContainer *container;
};

 /* namespace std */

#endif /* VALUEDEFINER_H_ */
