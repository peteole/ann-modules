/*
 * Momentum.h
 *
 *  Created on: Nov 3, 2019
 *      Author: olep
 */

#ifndef TRAININGMETHODS_MOMENTUM_H_
#define TRAININGMETHODS_MOMENTUM_H_
#define a 0.0001
#define beta 0.9

#define momentumParameterBytes 8

/*
 * needs 8 bytes for the parameters
 */
void MomentumDescent(double &derivative, double &oldValue,char* parameters){
	double &lastUpdate=*reinterpret_cast<double*>(parameters);
	double update=a*(derivative+beta*lastUpdate);
	oldValue-=update;
	lastUpdate=update;
}


#endif /* TRAININGMETHODS_MOMENTUM_H_ */
