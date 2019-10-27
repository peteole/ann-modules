/*
 * main.cpp
 *
 *  Created on: Jul 1, 2019
 *      Author: olep
 */

#include <iostream>
#include "NetworkContainer.h"
#include "NeuralNetwork.h"
#include "FullyConnectedNetwork.h"
#include "ErrorDefiner.h"
#include <math.h>

#include "Scaler.h"
using namespace std;
int main() {
	//10 inputs, 3 outputs, therefore 3 evaluation parameters
	NetworkContainer container(10, 3, 3);

	//FullyConnectedNetwork with 3 layers with 5,4,2 neurons
	FullyConnectedNetwork *network1 = new FullyConnectedNetwork(new int[3] { 5,
			4, 2 }, 3);

	//FullyConnectedNetwork with 3 layers with 5,3,2 neurons
	FullyConnectedNetwork *network2 = new FullyConnectedNetwork(new int[3] { 5,
			3, 2 }, 3);

	//FullyConnectedNetwork with 2 layers with 4,3 neurons
	FullyConnectedNetwork *network3 = new FullyConnectedNetwork(new int[2] { 4,
			3 }, 2);

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
	container.makeNetworkChildOf(network3, network1, 0, 2, 0);
	container.makeNetworkChildOf(network3, network2, 0, 2, 2);
	container.makeNetworkChildOf(errorDef, network3, 0);
	container.makeNetworkChildOf(&(container.out), errorDef);

	//let the container determine in which order it has to let the networks evaluate their inputs
	container.makeOrder();

	//create array for the training inputs
	double *input = new double[10];
	double *target=new double[3];
	for (int i = 0; i < 800000; i++) {
		for (int k = 0; k < 5; k++) {
			for (int j = 0; j < 10; j++) {
				input[j] = 1 * (double) random() / (double) RAND_MAX;
			}
			for(int j=0;j<3;j++){
				target[j]=(input[3*j]+input[3*j+1]+input[3*j+2])/3;
			}
			container.updateOutput(input, target);
			container.addDerivatives();
		}
		cout<<container.value<<endl;
		container.updateParameters(-1);
	}
	return 0;
}
