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
#define channels 1
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
			for(int i=0;i<3;i++){
				target[i]=(input[3*i]+input[3*i+1]+input[3*i+2])/3;
			}
			container.updateOutput(input, target);
			container.addDerivatives();
		}
		cout<<container.value<<endl;
		container.updateParameters(-1);
	}
	return 0;
		/*
		 //compute output for an arbitrary input
		 container.updateOutput(new double[10], new double[3] { 1, 4, 2 });
		 container.printOutput();

		 //compute error derivatives
		 container.addDerivatives();
		 cout << network1->dEdB[1][1] << endl;


		 //compute derivative numerically
		 double h = 1e-7;//small change
		 double oldV = container.value;
		 network1->b[1][1] += h;//change parameter by h

		 //compute new error value
		 container.value = 0;
		 container.updateOutput(new double[10], new double[4] { 1, 4, 2, 3 });
		 double newV = container.value;
		 cout << (newV - oldV) / h << endl;//print numerical derivative
		 return 0;
		 /*
		 NetworkContainer c(channels,channels,channels);
		 double *input = new double[channels];
		 FullyConnectedNetwork *n = new FullyConnectedNetwork(new int[3] { channels,25, channels },
		 3);
		 n->printWeights();
		 FullyConnectedNetwork *m = new FullyConnectedNetwork(*n);
		 Scaler *mult = new Scaler(channels);
		 Scaler *mult2 = new Scaler(channels);
		 mult->scaleFactor=2;
		 mult2->scaleFactor=0.1;
		 ValueDefiner *def = new ErrorDefiner(channels, &c, 0);
		 c.addNeuralNetwork(n);
		 c.addNeuralNetwork(m);
		 c.addNeuralNetwork(mult);
		 c.addNeuralNetwork(mult2);
		 c.addNeuralNetwork(def);
		 c.makeNetworkChildOf(mult2, &(c.in), 0);
		 c.makeNetworkChildOf(n, mult2, 0);
		 c.makeNetworkChildOf(m, n, 0);
		 c.makeNetworkChildOf(mult, m, 0);
		 c.makeNetworkChildOf(def, mult, 0);
		 c.makeNetworkChildOf(&(c.out), def, 0);
		 c.makeOrder();
		 c.updateOutput(input, new double[channels]);
		 c.addDerivatives();
		 double h = 1e-7;
		 double oldV = c.value;
		 cout << n->dEdB[1][1] << endl;
		 n->b[1][1] += h;
		 c.value = 0;
		 c.updateOutput(input, new double[channels] );

		 double newV = c.value;
		 cout << (newV - oldV) / h << endl;
		 //return 0;
		 for (int i = 0; i < 20000; i++) {
		 for (int k = 0; k < 50; k++) {
		 for (int j = 0; j < channels; j++) {
		 input[j] = 1 * (double) random() / (double) RAND_MAX;
		 }
		 //c.updateOutput(input, new double[5] { 70, 1, 3, 1, 2 });
		 c.updateOutput(input, input);
		 c.addDerivatives();
		 }
		 //c.printOutput();
		 if(i%100==0){
		 cout<<"="<<c.value<<endl;
		 }
		 c.updateParameters(-0.1);
		 /*n->printWeights();
		 cout << "scalefactors: " << mult2->scaleFactor << " "
		 << mult->scaleFactor << endl;
		 }
		 cout << mult2->scaleFactor << "|" << mult->scaleFactor << endl;
		 n->printWeights();
		 //c.updateOutput(input, new double[3] { 7, 1, 3 });
		 return 0;*/
	}
