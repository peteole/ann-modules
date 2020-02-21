/*
 * ErrorDefiner.h
 *
 *  Created on: Jul 9, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_ERRORDEFINER_H_
#define NETWORKTYPES_ERRORDEFINER_H_
#include "ValueDefiner.h"
namespace std {

class ErrorDefiner: public ValueDefiner {
public:

	ErrorDefiner(int channels, NetworkContainer *container,int firstChannelToUse) :
			ValueDefiner(channels, container) {
		this->targetValues = &(container->evaluationParameters[firstChannelToUse]);
		firstChannelUsed=firstChannelToUse;
	}
	ErrorDefiner(int channels, float *weights, NetworkContainer *container,int firstChannelToUse) :
			ValueDefiner(channels, container) {
		this->targetValues = &(container->evaluationParameters[firstChannelToUse]);
		this->weights = weights;
		firstChannelUsed=firstChannelToUse;
	}
	float getValue() override;
	float* getDValueDIn() override{
		float *d = new float[inputs];
		if (weights == 0) {
			for (int i = 0; i < inputs; i++) {
				d[i] = *input[i] - targetValues[i];
			}
		} else {
			for (int i = 0; i < inputs; i++) {
				d[i] = weights[i] * (*input[i] - targetValues[i]);
			}
		}
		return d;
	}
	float *targetValues;
	float *weights = 0;
	NeuralNetwork* clone() override{
		return new ErrorDefiner(inputs,container,firstChannelUsed);
	}
	~ErrorDefiner(){
		delete targetValues;
		delete weights;
	}
private:
	int firstChannelUsed;
};

} /* namespace std */

#endif /* NETWORKTYPES_ERRORDEFINER_H_ */
