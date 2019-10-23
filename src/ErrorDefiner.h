/*
 * ErrorDefiner.h
 *
 *  Created on: Jul 9, 2019
 *      Author: olep
 */

#ifndef ERRORDEFINER_H_
#define ERRORDEFINER_H_
#include "ValueDefiner.h"
//#include "NetworkContainer.h"
namespace std {

class ErrorDefiner: public ValueDefiner {
public:

	ErrorDefiner(int channels, NetworkContainer *container,int firstChannelToUse) :
			ValueDefiner(channels, container) {
		// TODO Auto-generated constructor stub
		/*this->inputs = channels;
		 this->outputs = channels;
		 this->input = new double*[inputs];
		 this->dEdIn = new double[inputs];
		 this->output = new double[outputs];
		 this->dEdOut = new double*[outputs];*/
		this->targetValues = &(container->evaluationParameters[firstChannelToUse]);
	}
	ErrorDefiner(int channels, double *weights, NetworkContainer *container) :
			ValueDefiner(channels, container) {
		this->targetValues = new double[channels];

		//this(channels);
		this->weights = weights;
	}
	double getValue() override;
	double* getDValueDIn() override{
		double *d = new double[inputs];
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
	/*void updateOutput() {
	 toArray(input, output, inputs);
	 }
	 void addDerivatives() {
	 double *input = getInput();
	 if (weights == 0) {
	 for (int i = 0; i < inputs; i++) {
	 dEdIn[i] = *dEdOut[i] + input[i]-*targetValues[i];
	 }
	 } else {
	 for (int i = 0; i < inputs; i++) {
	 dEdIn[i] = *dEdOut[i] + weights[i]*(input[i]-*targetValues[i]);
	 }
	 }
	 }
	 void updateParameters() {
	 }*/
	double *targetValues;
	double *weights = 0;
};

} /* namespace std */

#endif /* ERRORDEFINER_H_ */
