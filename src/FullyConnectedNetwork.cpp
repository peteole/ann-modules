/*
 * FullyConnectedNetwork.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: olep
 */

#include "FullyConnectedNetwork.h"
#include "NeuralNetwork.h"
#include <iostream>
namespace std {
FullyConnectedNetwork::FullyConnectedNetwork(int *neuronsInLayer,
		int numOfLayers) :
		NeuralNetwork(neuronsInLayer[0], neuronsInLayer[numOfLayers - 1]), isOriginal(
				true) {
	if (numOfLayers < 1) {
		cout << "zu wenige Schichten";
		exit(-1);
	}
	this->neuronsInLayer = neuronsInLayer;
	this->numOfLayers = numOfLayers;
	net = new double*[numOfLayers];
	b = new double*[numOfLayers];
	dEdB = new double*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new double[neuronsInLayer[i]];
		b[i] = new double[neuronsInLayer[i]];
		dEdB[i] = new double[neuronsInLayer[i]];
	}
	w = new double**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		w[i] = new double*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			w[i][j] = new double[neuronsInLayer[i]];
		}
	}
	dEdW = new double**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		dEdW[i] = new double*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			dEdW[i][j] = new double[neuronsInLayer[i]];
		}
	}
	this->initializeParameters();
}
FullyConnectedNetwork::FullyConnectedNetwork(
		const FullyConnectedNetwork &toCopy) :NeuralNetwork(toCopy.inputs,toCopy.outputs),
		w(toCopy.w), b(toCopy.b), dEdW(toCopy.dEdW), dEdB(toCopy.dEdB), isOriginal(
				false), neuronsInLayer(toCopy.neuronsInLayer), numOfLayers(
				toCopy.numOfLayers) {
	net = new double*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new double[neuronsInLayer[i]];
	}

}
void FullyConnectedNetwork::printWeights(){
	cout<<"weights:"<<endl;
	for(int i=0;i<numOfLayers-1;i++){
		for(int j=0;j<neuronsInLayer[i+1];j++){
			for(int k=0;k<neuronsInLayer[i];k++){
				cout<<w[i][j][k]<<" ";
			}
			cout<<"|";
		}
		cout<<endl;
	}
	cout<<"biases:"<<endl;
	for(int i=0;i<numOfLayers;i++){
		for(int j=0;j<neuronsInLayer[i];j++){
			cout<<b[i][j]<<" ";
		}
		cout<<endl;
	}
}
void FullyConnectedNetwork::updateOutput() {
	double *output = new double[neuronsInLayer[0]];
	net[0] = getInput();
	for (int i = 0; i < neuronsInLayer[0]; i++) {
		output[i] = a(net[0][i]);
	}
	for (int i = 1; i < numOfLayers; i++) {
		//input = new double[neuronsInLayer[i]];
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			//fuer jedes Neuron der Zielschicht
			double netValue = b[i][j];
			for (int k = 0; k < neuronsInLayer[i - 1]; k++) {
				//fuer jedes Neuron der vorherigen Schicht
				netValue += output[k] * w[i - 1][j][k];
			}
			net[i][j] = netValue;
		}
		delete[] output;
		output = new double[neuronsInLayer[i]];
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			output[j] = a(net[i][j]);
		}
	}
	for (int i = 0; i < neuronsInLayer[numOfLayers - 1]; i++) {
		this->output[i] = output[i];
	}
	delete [] output;
}
void FullyConnectedNetwork::updateParameters(double alpha) {
	if(!isOriginal){
		return;
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				w[i][k][j] += alpha * dEdW[i][k][j];
				dEdW[i][k][j] = 0;
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			this->b[i][j] += alpha * dEdB[i][j];
			dEdB[i][j] = 0;
		}
	}
}
void FullyConnectedNetwork::addDerivatives() {
	int neuronsInLastLayer = neuronsInLayer[numOfLayers - 1];
	double *dEdNet = new double[neuronsInLastLayer];
	for (int i = 0; i < neuronsInLastLayer; i++) {
		dEdNet[i] = *dEdOut[i] * as(net[numOfLayers - 1][i]);
	}
	for (int i = numOfLayers - 2; i >= 0; i--) {
		int neuronsInPrevLayer = neuronsInLayer[i];
		int neuronsInCurrentLayer = neuronsInLayer[i + 1];
		double *newdEdNet = new double[neuronsInPrevLayer];
		//fuer jede Schicht
		for (int j = 0; j < neuronsInPrevLayer; j++) {
			//fuer jedes Neuron der Ausgangsschicht
			double dEdOut = 0;
			double currentOutput = a(net[i][j]);
			for (int k = 0; k < neuronsInCurrentLayer; k++) {
				dEdOut += dEdNet[k] * w[i][k][j];
				dEdW[i][k][j] += currentOutput * dEdNet[k];
			}
			newdEdNet[j] = as(net[i][j]) * dEdOut;
			dEdB[i][j] += newdEdNet[j];
		}
		delete[] dEdNet;
		dEdNet = newdEdNet;
	}
	for (int i = 0; i < inputs; i++) {
		dEdIn[i] = dEdNet[i];
	}
	delete[] dEdNet;
}
} /* namespace std */
