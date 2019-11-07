/*
 * Sigmoid.cpp
 *
 *  Created on: Nov 4, 2019
 *      Author: olep
 */

#include "Sigmoid.h"

double sigmoid(double x){
	return 1 / (1 + pow(M_E, -x));
}
double sigmoids(double x){
	double c=sigmoid(x);
	return c*(1-c);
}

