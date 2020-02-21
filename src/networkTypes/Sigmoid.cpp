/*
 * Sigmoid.cpp
 *
 *  Created on: Nov 4, 2019
 *      Author: olep
 */

#include "Sigmoid.h"

float sigmoid(float x){
	return 1 / (1 + pow(M_E, -x));
}
float sigmoids(float x){
	float c=sigmoid(x);
	return c*(1-c);
}

