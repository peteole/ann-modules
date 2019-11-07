/*
 * NetworkContainer.cpp
 *
 *  Created on: Jul 10, 2019
 *      Author: olep
 */

#include "NetworkContainer.h"

#include <exception>
#include <algorithm>
#include <iostream>

//#include "NeuralNetwork.h"
//#include "PlugOut.h"
using namespace std;

NetworkContainer::NetworkContainer(int inputs, int outputs,
		int evaluationParameters,
		void (*construct)(NetworkContainer &toConstruct)) :
		evaluationParameters(new double[evaluationParameters]), numOfEvaluationParameters(
				evaluationParameters), in(inputs), out(outputs), NeuralNetwork(
				inputs, outputs) {
	this->construct = construct;
	construct(*this);
	makeOrder();
}
NetworkContainer::NetworkContainer(int inputs, int outputs,
		int evaluationParameters) :
		evaluationParameters(new double[evaluationParameters]), numOfEvaluationParameters(
				evaluationParameters), in(inputs), out(outputs), NeuralNetwork(
				inputs, outputs) {
}
void NetworkContainer::updateOutput() {
	double *target = this->in.output;
	for (int i = 0; i < this->inputs; i++) {
		target[i] = *(this->input[i]);
	}
	for (int i = 0; i < numOfNetworks; i++) {
		networks[i]->updateOutput();
	}
}
void NetworkContainer::updateOutput(double *input,
		double *evaluationParameters) {
	for (int i = 0; i < inputs; i++) {
		this->in.output[i] = input[i];
	}
	for (int i = 0; i < numOfEvaluationParameters; i++) {
		this->evaluationParameters[i] = evaluationParameters[i];
	}
	for (int i = 0; i < numOfNetworks; i++) {
		networks[i]->updateOutput();
	}
}
void NetworkContainer::addDerivatives() {
	for (int i = numOfNetworks - 1; i >= 0; i--) {
		networks[i]->addDerivatives();
	}
}
void NetworkContainer::makeNetworkChildOf(PlugIn *child, PlugOut *parent,
		int firstParentPortToUse, int amountOfPortsToUse,
		int firstChildPortToUse) {
	if (amountOfPortsToUse == -1) {
		amountOfPortsToUse = child->inputs;
	}
	for (int i = 0; i < amountOfPortsToUse; i++) {
		child->input[i + firstChildPortToUse] =
				&(parent->output[firstParentPortToUse + i]);
		parent->dEdOut[i + firstParentPortToUse] = &(child->dEdIn[i
				+ firstChildPortToUse]);
	}
	if (parent != &in && child != &out) {
		NeuralNetwork *parentNetwork = static_cast<NeuralNetwork*>(parent);
		NeuralNetwork *childNetwork = static_cast<NeuralNetwork*>(child);
		getSorterOf(parentNetwork)->after.push_back(child);
		getSorterOf(childNetwork)->before.push_back(parent);
	}
}
void NetworkContainer::makeOrder() {
	list<NetSorter*> sorterCopy(networkSorter);
	networks = new NeuralNetwork*[networkSorter.size()];
	for (int index = 0; !sorterCopy.empty(); index++) {
		for (list<NetSorter*>::iterator it = sorterCopy.begin();
				it != sorterCopy.end(); it++) {
			if ((*it)->before.empty()) {
				networks[index] = (*it)->n;
				for (NetSorter *k : sorterCopy) {
					PlugOut *p = dynamic_cast<PlugOut*>((*it)->n);
					k->before.remove(p);
				}
				sorterCopy.remove(*it);
				break;
			}
		}
	}

}
void NetworkContainer::updateParameters(
		void (*updateFunction)(double &derivative, double &oldValue,
				char *parameters)) {
	for (int i = numOfNetworks - 1; i >= 0; i--) {
		networks[i]->updateParameters(updateFunction);
	}
	this->value = 0;
}
void NetworkContainer::createBufferStorage(int numOfBytes) {
	for (int i = numOfNetworks - 1; i >= 0; i--) {
		networks[i]->createBufferStorage(numOfBytes);
	}
}
void NetworkContainer::updateParameters(double alpha) {
	for (int i = numOfNetworks - 1; i >= 0; i--) {
		networks[i]->updateParameters(alpha);
	}
	this->value = 0;
}
void NetworkContainer::addNeuralNetwork(NeuralNetwork *toAdd) {
	NetSorter *s = new NetSorter(toAdd);
	networkSorter.push_back(s);
	numOfNetworks++;
}
/* namespace std */
