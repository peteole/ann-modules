/*
 * training.cpp
 *
 *  Created on: Nov 1, 2019
 *      Author: olep
 */

#include "training.h"

void computeDerivatives(NetworkContainer* toTrain,double*** trainingExamples, int amountOfExamples){
	for(int i=0;i<amountOfExamples;i++){
		toTrain->updateOutput(trainingExamples[i][0], trainingExamples[i][1]);
		toTrain->addDerivatives();
	}
}

void train(NetworkContainer* toTrain,double*** trainingExamples, int amountOfExamples, int threads){
	NetworkContainer ** copies=new NetworkContainer*[threads];
	copies[0]=toTrain;
	for(int i=1;i<threads;i++){
		copies[i]=dynamic_cast<NetworkContainer*>(toTrain->clone());
		copies[i]->copyParameters(toTrain);
	}
	std::thread **t=new std::thread*[threads];
	int avExamples=amountOfExamples/threads;
	int ne=avExamples;
	for(int i=0;i<threads;i++){
		if(i==threads-1){
			ne+=amountOfExamples%threads;
		}
		t[i]=new std::thread(computeDerivatives,copies[i],&(trainingExamples[i*avExamples]),ne);
	}
	for(int i=0;i<threads;i++){
		t[i]->join();
	}
	for(int i=1;i<threads;i++){
		toTrain->addDerivatives(copies[i]);
		toTrain->value+=copies[i]->value;
		delete copies[i];
	}
}
