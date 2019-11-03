/*
 * main.cpp
 *
 *  Created on: Jul 1, 2019
 *      Author: olep
 */

#include <iostream>
#include <math.h>

#include "networkTypes/ErrorDefiner.h"
#include "networkTypes/FullyConnectedNetwork.h"
#include "networkTypes/NetworkContainer.h"
#include "networkTypes/NeuralNetwork.h"
#include "networkTypes/Scaler.h"
#include "training.h"
//#include "trainingMethods/Momentum.h"
#include "trainingMethods/RProp.h"
using namespace std;

void constructContainer(NetworkContainer &container) {

	//FullyConnectedNetwork with 3 layers with 5,4,2 neurons
	FullyConnectedNetwork *network1 = new FullyConnectedNetwork(new int[3] { 5,
			4, 3 }, 3);

	//FullyConnectedNetwork with 3 layers with 5,3,2 neurons
	FullyConnectedNetwork *network2 = new FullyConnectedNetwork(new int[3] { 5,
			5, 3 }, 3);

	//FullyConnectedNetwork with 2 layers with 4,3 neurons
	FullyConnectedNetwork *network3 = new FullyConnectedNetwork(new int[3] { 6,
			4,3 }, 3);

	//ValueDefiner with one channel for each output of the networkContainer
	ValueDefiner *errorDef = new ErrorDefiner(3, &container, 0);
	//let the networkContainer know it is about to work with all the networks we use
	container.addNeuralNetwork(network1);
	container.addNeuralNetwork(network2);
	container.addNeuralNetwork(network3);
	container.addNeuralNetwork(errorDef);

	//conntect the networks to each other
	container.makeNetworkChildOf(network1, &(container.in), 0);
	container.makeNetworkChildOf(network2, &(container.in), 5);
	container.makeNetworkChildOf(network3, network1, 0, 3, 0);
	container.makeNetworkChildOf(network3, network2, 0, 3, 3);
	container.makeNetworkChildOf(errorDef, network3, 0);
	container.makeNetworkChildOf(&(container.out), errorDef);

}
#define numOfExamples 1000
int main() {
	NetworkContainer container(10, 3, 3, constructContainer);
	double ***trainingExamples = new double**[numOfExamples];
	for (int i = 0; i < numOfExamples; i++) {
		trainingExamples[i] = new double*[2];
		trainingExamples[i][0] = new double[10];
		trainingExamples[i][1] = new double[3];
		double *input = trainingExamples[i][0];
		for (int j = 0; j < 10; j++) {
			input[j] = 1 * (double) random() / (double) RAND_MAX;
		}
		for (int j = 0; j < 3; j++) {
			trainingExamples[i][1][j] = (input[3 * j] + input[3 * j + 1]
					+ input[3 * j + 2]) / 3;
		}
	}
	for(int i=0;i<3;i++){
		cout<<trainingExamples[0][1][i]<<"|";
	}
	container.createBufferStorage(RPropByteAmount);
	double minValue=MAXFLOAT;
	NetworkContainer *BestConfig=dynamic_cast<NetworkContainer*>(container.clone());
	for (int i = 0; i < 5000; i++) {
		train(&container, trainingExamples, numOfExamples, 4);
		cout<<container.value<<endl;
		if(container.value+0.3<minValue){
			//if there is significant improvement
			minValue=container.value;
			BestConfig->copyParameters(&container);
		}
		if(container.value>minValue+1.0){
			//if the error value increased too much
			container.updateParameters(0.0);//reset derivatives
			container.copyParameters(BestConfig);//reset parameters
			container.createBufferStorage(RPropByteAmount);//reset "momentum gained"
			cout<<"resetting to last backup"<<endl;
			continue;
		}
		container.updateParameters(RProp);
	}
	cout<<"RProp finished"<<endl;
	return 0;
}
