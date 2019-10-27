/*
 * Multiplier.h
 *
 *  Created on: Oct 26, 2019
 *      Author: olep
 */

#ifndef SCALER_H_
#define SCALER_H_

#include "NeuralNetwork.h"

namespace std {

class Scaler: public NeuralNetwork {
public:
	double scaleFactor = 1;
	double dValueDScaleFactor = 0;
	Scaler(int channels) :
			NeuralNetwork(channels, channels) {

	}
	void updateOutput() override {
		for (int i = 0; i < inputs; i++) {
			output[i] = *input[i] * scaleFactor;
		}
	}
	void addDerivatives() {
		for (int i = 0; i < inputs; i++) {
			dValueDScaleFactor += *dEdOut[i] * *input[i];
			dEdIn[i]=*dEdOut[i]*scaleFactor;
		}
	}
	void updateParameters(double alpha){
		scaleFactor+=alpha*dValueDScaleFactor;
		dValueDScaleFactor=0;
	}
};

} /* namespace std */

#endif /* SCALER_H_ */
