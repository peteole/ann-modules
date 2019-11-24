/*
 * FullyConnectedNetwork.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: olep
 */

#include "FullyConnectedNetworkTemplate.h"

#include <iostream>

//#include "../NetworkTypes/NeuralNetwork.h"
namespace std {
template<double a(double),double as(double)>FullyConnectedNetworkTemplate<a,as>::FullyConnectedNetworkTemplate(int *neuronsInLayer,
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
	//lastBFactor = new double*[numOfLayers];
	//lastBSign = new bool*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new double[neuronsInLayer[i]]();
		b[i] = new double[neuronsInLayer[i]]();
		dEdB[i] = new double[neuronsInLayer[i]]();
		//lastBFactor[i] = new double[neuronsInLayer[i]];
		//lastBSign[i] = new bool[neuronsInLayer[i]];
	}
	w = new double**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		w[i] = new double*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			w[i][j] = new double[neuronsInLayer[i]]();
		}
	}
	dEdW = new double**[numOfLayers - 1];
	//lastWFactor = new double**[numOfLayers - 1];
	//lastWSign = new bool**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		dEdW[i] = new double*[neuronsInLayer[i + 1]];
		//lastWFactor[i] = new double*[neuronsInLayer[i + 1]];
		//lastWSign[i] = new bool*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			dEdW[i][j] = new double[neuronsInLayer[i]]();
			//lastWFactor[i][j] = new double[neuronsInLayer[i]];
			//lastWSign[i][j] = new bool[neuronsInLayer[i]];
		}
	}
	this->initializeParameters();
}
template<double a(double),double as(double)>FullyConnectedNetworkTemplate<a,as>::FullyConnectedNetworkTemplate(
		const FullyConnectedNetworkTemplate &toCopy, bool useSameParameters) :
		NeuralNetwork(toCopy.inputs, toCopy.outputs), w(toCopy.w), b(toCopy.b), dEdW(
				toCopy.dEdW), dEdB(toCopy.dEdB), isOriginal(false), neuronsInLayer(
				toCopy.neuronsInLayer), numOfLayers(toCopy.numOfLayers) {
	net = new double*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new double[neuronsInLayer[i]];
	}

}
template<double a(double),double as(double)> NeuralNetwork* FullyConnectedNetworkTemplate<a,as>::clone() {
	return new FullyConnectedNetworkTemplate(neuronsInLayer, numOfLayers);
}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::printWeights() {
	cout << "weights:" << endl;
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			for (int k = 0; k < neuronsInLayer[i]; k++) {
				cout << w[i][j][k] << ":"<<dEdW[i][j][k]<<" ";
			}
			cout << "|";
		}
		cout << endl;
	}
	cout << "biases:" << endl;
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			cout << b[i][j] << ":"<<dEdB[i][j]<<" ";
		}
		cout << endl;
	}
}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::updateOutput() {
	double *output = new double[neuronsInLayer[0]];
	delete net[0];
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
	delete[] output;
}
template<double a(double),double as(double)> FullyConnectedNetworkTemplate<a,as>::~FullyConnectedNetworkTemplate() {
	for (int i = 0; i < numOfLayers; i++) {
		delete net[i];
		delete b[i];
		delete dEdB[i];
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			delete w[i][j];
		}
		delete w[i];
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			delete dEdW[i][j];
		}
		delete dEdW[i];
	}
	delete w;
	delete dEdW;
	delete this->neuronsInLayer;
	delete net;
	delete b;
	delete dEdB;
}
#define minVal 0.00000001
#define maxVal 0.005
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::updateParameters(double alpha) {
	if (!isOriginal) {
		return;
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				w[i][k][j]+=alpha*dEdW[i][k][j];
				//cout<<w[i][k][j]<<endl;
				dEdW[i][k][j] = 0;
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			b[i][j]+=alpha*dEdB[i][j];
			dEdB[i][j] = 0;
		}
	}

}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::addDerivatives() {
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
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::copyParameters(NeuralNetwork *const toCopy) {
	FullyConnectedNetworkTemplate *toUse = dynamic_cast<FullyConnectedNetworkTemplate*>(toCopy);
	if (!toUse)
		return;
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				w[i][k][j] = toUse->w[i][k][j];
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			this->b[i][j] = toUse->b[i][j];
		}
	}
}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::updateParameters(
		void (*updateFunction)(double &derivative, double &oldValue,
				char *parameters)) {
	if (!isOriginal) {
		return;
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				double &currentWeight=w[i][k][j];
				double &currentDerivative=dEdW[i][k][j];
				updateFunction(currentDerivative,currentWeight,updateParameter[i][k][j]);
				currentDerivative = 0;
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			double &currentBias=b[i][j];
			double &currentDerivative=dEdB[i][j];
			updateFunction(currentDerivative,currentBias,bUpdateParameter[i][j]);
			currentDerivative = 0;
		}
	}

}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::createBufferStorage(int numOfBytes) {
	if (this->updateParameter) {
		for (int i = 0; i < numOfLayers - 1; i++) {
			for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
				delete this->updateParameter[i][j];
			}
			delete this->updateParameter[i];
		}
		delete this->updateParameter;
		for(int i=0;i<numOfLayers;i++){
			delete bUpdateParameter[i];
		}
		delete bUpdateParameter;
	}
	this->updateParameter = new char***[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		this->updateParameter[i] = new char**[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			this->updateParameter[i][j] = new char*[neuronsInLayer[i]];
			for(int k=0;k<neuronsInLayer[i];k++){
				updateParameter[i][j][k]=new char[numOfBytes]();
			}

		}
	}
	this->bUpdateParameter=new char**[numOfLayers];
	for(int i=0;i<numOfLayers;i++){
		bUpdateParameter[i]=new char*[neuronsInLayer[i]];
		for(int j=0;j<neuronsInLayer[i];j++){
			bUpdateParameter[i][j]=new char[numOfBytes]();
		}
	}

}
template<double a(double),double as(double)> void FullyConnectedNetworkTemplate<a,as>::addDerivatives(NeuralNetwork *const toCopy) {
	FullyConnectedNetworkTemplate *toUse = dynamic_cast<FullyConnectedNetworkTemplate*>(toCopy);
	if (!toUse)
		return;
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				dEdW[i][k][j] += toUse->dEdW[i][k][j];
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			this->dEdB[i][j] = toUse->dEdB[i][j];
		}
	}
}
} /* namespace std */
