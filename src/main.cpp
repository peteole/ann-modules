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
using namespace std;
int main() {
	NetworkContainer c(5, 5, 5);
	double *input = new double[5] { 1, 9, 3, 4, 5 };
	FullyConnectedNetwork *n = new FullyConnectedNetwork(new int[3] { 5, 4, 5 },
			3);
	FullyConnectedNetwork *m = new FullyConnectedNetwork(*n);
	ValueDefiner *def = new ErrorDefiner(5, &c, 0);
	c.addNeuralNetwork(n);
	c.addNeuralNetwork(m);
	c.addNeuralNetwork(def);
	c.makeNetworkChildOf(n, &(c.in), 0);
	c.makeNetworkChildOf(m, n, 0);
	c.makeNetworkChildOf(def, m, 0);
	c.makeNetworkChildOf(&(c.out), def, 0);
	c.makeOrder();
	c.updateOutput(input, new double[5] { 7, 1, 3, 1, 2 });
	c.addDerivatives();
	double h=1e-7;
	double oldV=c.value;
	cout<<n->dEdB[1][1]<<endl;
	n->b[1][1]+=h;
	c.value=0;
	c.updateOutput(input, new double[5] { 7, 1, 3, 1, 2 });

	double newV=c.value;
	cout<<(newV-oldV)/h<<endl;
	//return 0;
	for (int i = 0; i < 100000; i++) {
		for (int k = 0; k < 50; k++) {
			for (int j = 0; j < 5; j++) {
				input[j] = (double) random() / (double) RAND_MAX;
			}
			c.updateOutput(input, new double[5] { 0.7, 0.1, 0.3, 0.1, 0.2 });
			c.addDerivatives();
		}
		c.printOutput();
		c.updateParameters(-0.00005);
	}
	//c.updateOutput(input, new double[3] { 7, 1, 3 });
	return 0;
}
