/*
 * RProp.h
 *
 *  Created on: Nov 3, 2019
 *      Author: olep
 */

#ifndef TRAININGMETHODS_RPROP_H_
#define TRAININGMETHODS_RPROP_H_
#include <algorithm>
#define minVal 0.00000001
#define maxVal 0.005

#define RPropByteAmount 5
void RProp(float &derivative, float &oldValue,char* parameters);

#endif /* TRAININGMETHODS_RPROP_H_ */
