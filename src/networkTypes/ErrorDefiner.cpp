/*
 * ErrorDefiner.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: olep
 */

#include "ErrorDefiner.h"

namespace std {
float ErrorDefiner::getValue(){
	float value = 0;
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
EMSCRIPTEN_BINDINGS(ErrorDefiner) {
  class_<ErrorDefiner, base<ValueDefiner>>("ErrorDefiner")
      .constructor<int,NetworkContainer*,int>();
}
} /* namespace std */
