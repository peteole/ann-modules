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
	ValueDefiner *def = new ErrorDefiner(3, &container, 0);

	//let the networkContainer know it is about to work with all the networks we use
	container.addNeuralNetwork(network1);
	container.addNeuralNetwork(network2);
	container.addNeuralNetwork(network3);
	container.addNeuralNetwork(def);

	//conntect the networks to each other
	container.makeNetworkChildOf(network1, &(container.in), 0);
	container.makeNetworkChildOf(network2, &(container.in), 5);
	container.makeNetworkChildOf(network3, network1, 0, 2, 0);
	container.makeNetworkChildOf(network3, network2, 0, 2, 2);
	container.makeNetworkChildOf(def, network3, 0);
	container.makeNetworkChildOf(&(container.out), def);

	//let the container determine in which order it has to let the networks evaluate their inputs
	container.makeOrder();

	//compute output for an arbitrary input
	container.updateOutput(new double[10]{3,1,5,1,3,-1,2,5,0,3}, new double[3] { 1, 4, 2 });
	container.printOutput();

	//compute error derivatives
	container.addDerivatives();
	
	//update all parameters
	container.updateParameters(-0.01);
	return 0;
}
