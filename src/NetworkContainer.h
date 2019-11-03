/*
 * NetworkContainer.h
 *
 *  Created on: Jul 10, 2019
 *      Author: olep
 */

#ifndef NETWORKCONTAINER_H_
#define NETWORKCONTAINER_H_

#include "list"
#include <iostream>
#include "NeuralNetwork.h"
//#include "ErrorDefiner.h"
//#include "ValueDefiner.h"
//class ValueDefiner;
#include <map>

using namespace std;

class NetworkContainer: public NeuralNetwork {
public:
	NetworkContainer(int inputs, int outputs, int evaluationParameters ,void (*construct)(NetworkContainer &toConstruct));
	void (*construct)(NetworkContainer &toConstruct);
	~NetworkContainer();
	PlugOut in;
	PlugIn out;
	double value = 0;
	unsigned char* memory;
	void getMemory(int bytes){
		this->memory= new unsigned char[bytes];
	}
	void addNeuralNetwork(NeuralNetwork *toAdd);
	void makeNetworkChildOf(PlugIn *child, PlugOut *parent,
			int firstParentPortToUse = 0, int amountOfPortsToUse = -1,
			int firstChildPortToUse = 0);
	void updateParameters(double alpha) override;
	void addDerivatives() override;
	void updateOutput() override;
	void updateOutput(double *input, double *evaluationParameters =
			new double[0]);
	void makeOrder();
	NeuralNetwork **networks = 0;
	int numOfNetworks = 0;
	void printOutput() {
		for (int i = 0; i < out.inputs; i++) {
			cout << "|" << *(out.input[i]);
		}
		cout << "|. Value: " << this->value << endl;
	}
	double *const evaluationParameters;
	const int numOfEvaluationParameters;
	void copyParameters(NeuralNetwork *const toCopy) override{
		NetworkContainer* toUse=dynamic_cast<NetworkContainer*>(toCopy);
		if(!toUse){
			return;
		}
		for(int i=0;i<numOfNetworks;i++){
			networks[i]->copyParameters(toUse->networks[i]);
		}
	}
	NeuralNetwork *clone()override{
		return new NetworkContainer(inputs, outputs, numOfEvaluationParameters,construct);
	}
private:
	struct commitment {
		PlugOut *parent;
		int firstParentPortToUse;
		int amountOfPortsToUse ;
		int firstChildPortToUse;
		int parentID=-1;
	};
	class NetSorter {
	public:
		NeuralNetwork *n;
		list<PlugOut*> before;
		list<PlugIn*> after;
		list<commitment*> connections;
		NetSorter(NeuralNetwork *n) {
			this->n = n;
		}
		bool isAddable() {
			//check if all plugs are plugged in
			for (int i = 0; i < n->inputs; i++) {
				if (n->input[i] == NULL) {
					return false;
				}
			}
			for (int i = 0; i < n->outputs; i++) {
				if (n->dEdOut[i] == NULL) {
					return false;
				}
			}

			//check if any network needs to be added before

			if (before.size() != 1) {
				return false;
			}
			return true;

		}
	};
	list<NetSorter*> networkSorter;
	NetSorter* getSorterOf(NeuralNetwork *n) {
		for (list<NetSorter*>::iterator it = networkSorter.begin();
				it != networkSorter.end(); it++) {
			if ((*it)->n == n) {
				return *it;
			}
		}
		return 0;
	}
	void applyCommitment(commitment toApply);
};

/* namespace std */

#endif /* NETWORKCONTAINER_H_ */
