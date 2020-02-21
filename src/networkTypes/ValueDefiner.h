/*
 * ValueDefiner.h
 *
 *  Created on: Oct 10, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_VALUEDEFINER_H_
#define NETWORKTYPES_VALUEDEFINER_H_


#include <emscripten.h>
#include <emscripten/bind.h>
//#include "NetworkContainer.h"
//class NetworkContainer;
#include "NetworkContainer.h"
using namespace std;
//#include "NeuralNetwork.h"

class ValueDefiner: public NeuralNetwork {
public:
	ValueDefiner(int channels, NetworkContainer *container);
	virtual float getValue() {
		return 0;
	}
	virtual float* getDValueDIn() {
		float *d = new float[inputs];
		float oldValue = getValue();
		for (int i = 0; i < inputs; i++) {
			output[i] += 0.001;
			d[i] = (getValue() - oldValue) / 0.001;
			output[i] -= 0.001;
		}
		return d;
	}
	void updateOutput() override;
	void updateParameters(float alpha) override{

	}
	void updateParameters(void (*updateFunction)(float &derivative,float &oldValue, char* parameters))override{

	}
	void createBufferStorage(int numOfBytes)override{

	}
	void addDerivatives() override{
		float *d = getDValueDIn();
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
