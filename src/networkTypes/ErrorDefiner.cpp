/*
 * ErrorDefiner.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: olep
 */

#include "ErrorDefiner.h"

namespace std {
double ErrorDefiner::getValue(){
	double value = 0;
	if (weights == 0) {
		for (int i = 0; i < inputs; i++) {
			value += (*input[i] - targetValues[i])
					* (*input[i] - targetValues[i]);
		}
	} else {
		for (int i = 0; i < inputs; i++) {
			value += (weights[i] * (*input[i] - targetValues[i]))
					* (weights[i] * (*input[i] - targetValues[i]));
		}
	}
	return value/2;
}
} /* namespace std */
