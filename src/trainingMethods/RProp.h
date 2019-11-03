/*
 * RProp.h
 *
 *  Created on: Nov 3, 2019
 *      Author: olep
 */

#ifndef TRAININGMETHODS_RPROP_H_
#define TRAININGMETHODS_RPROP_H_
#include <math.h>
#define minVal 0.00000001
#define maxVal 0.005

int getByteLength(){
	return sizeof(bool)+sizeof(double);
}
void RProp(double &derivative, double &oldValue,char* parameters){
	bool &lastSign=*(reinterpret_cast<bool*>(parameters));
	double &lastFactor=*(reinterpret_cast<double*>(parameters+1));
	double factor;
	bool newSign = derivative> 0;
	if (newSign == lastSign) {
		factor = min(lastFactor * 1.1, maxVal);
	} else {
		factor = max(lastFactor * 0.1, minVal);
	}
	lastSign = newSign;
	lastFactor = factor;
	if (newSign) {
		oldValue -= factor;
	} else {
		oldValue += factor;
	}
}

#endif /* TRAININGMETHODS_RPROP_H_ */
