/*
 * training.h
 *
 *  Created on: Nov 1, 2019
 *      Author: olep
 */

#ifndef TRAINING_H_
#define TRAINING_H_
#include <thread>

#include "networkTypes/NetworkContainer.h"

void computeDerivatives(NetworkContainer* toTrain,double*** trainingExamples, int amountOfExamples);

void train(NetworkContainer* toTrain,double*** trainingExamples, int amountOfExamples, int threads);



#endif /* TRAINING_H_ */
