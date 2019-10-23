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
	NetworkContainer(int inputs, int outputs, int evaluationParameters=0);
	~NetworkContainer();
	PlugOut in;
	PlugIn out;
	double value=0;
	/*template<typename T> void moveElement(T *&array, int toMove,
			int targetPosition) {
		T element = array[toMove];
		if (toMove < targetPosition) {
			for (int i = toMove; i < targetPosition; i++) {
				array[i] = array[i + 1];
			}
			array[targetPosition] = element;
		}
		if (toMove > targetPosition) {
			for (int i = toMove; i > targetPosition; i--) {
				array[i] = array[i - 1];
			}
			array[targetPosition] = element;
		}
	}
	template<typename T> int getIndexOf(T *&array, T element, int length) {
		for (int i = 0; i < length; i++) {
			if (array[i] == element) {
				return i;
			}
		}
		return -1;
	}*/

	void addNeuralNetwork(NeuralNetwork *toAdd);
	void makeNetworkChildOf(PlugIn *child, PlugOut *parent, int firstPortToUse);
	void updateParameters(double alpha) override;
	void addDerivatives() override;
	//double *totalInput;
	void updateOutput() override;
	void updateOutput(double * input, double* evaluationParameters=new double[0]);
	void makeOrder();
	NeuralNetwork **networks = 0;
	//ErrorDefiner *errorDefiner;
	int numOfNetworks = 0;
	void printOutput(){
		for(int i=0;i<out.inputs;i++){
			cout<<"|"<<*(out.input[i]);
		}
		cout<<"|. Value: "<<this->value<<endl;
	}
	double * const evaluationParameters;
	const int numOfEvaluationParameters;
private:
	class NetSorter {
	public:
		NeuralNetwork *n;
		list<PlugOut*> before;
		list<PlugIn*> after;
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
	//map<NeuralNetwork*, NetSorter*> netToSorter;
	NetSorter* getSorterOf(NeuralNetwork *n) {
		for (list<NetSorter*>::iterator it = networkSorter.begin();
				it != networkSorter.end(); it++) {
			if ((*it)->n == n) {
				return *it;
			}
		}
		return 0;
	}
};

 /* namespace std */

#endif /* NETWORKCONTAINER_H_ */
