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
template<float a(float),float as(float)>FullyConnectedNetworkTemplate<a,as>::FullyConnectedNetworkTemplate(int *neuronsInLayer,
		int numOfLayers) :
		NeuralNetwork(neuronsInLayer[0], neuronsInLayer[numOfLayers - 1]), isOriginal(
				true) {
	if (numOfLayers < 1) {
		cout << "zu wenige Schichten";
		exit(-1);
	}
	this->neuronsInLayer = neuronsInLayer;
	this->numOfLayers = numOfLayers;
	net = new float*[numOfLayers];
	b = new float*[numOfLayers];
	dEdB = new float*[numOfLayers];
	//lastBFactor = new float*[numOfLayers];
	//lastBSign = new bool*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new float[neuronsInLayer[i]]();
		b[i] = new float[neuronsInLayer[i]]();
		dEdB[i] = new float[neuronsInLayer[i]]();
		//lastBFactor[i] = new float[neuronsInLayer[i]];
		//lastBSign[i] = new bool[neuronsInLayer[i]];
	}
	w = new float**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		w[i] = new float*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			w[i][j] = new float[neuronsInLayer[i]]();
		}
	}
	dEdW = new float**[numOfLayers - 1];
	//lastWFactor = new float**[numOfLayers - 1];
	//lastWSign = new bool**[numOfLayers - 1];
	for (int i = 0; i < numOfLayers - 1; i++) {
		dEdW[i] = new float*[neuronsInLayer[i + 1]];
		//lastWFactor[i] = new float*[neuronsInLayer[i + 1]];
		//lastWSign[i] = new bool*[neuronsInLayer[i + 1]];
		for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
			dEdW[i][j] = new float[neuronsInLayer[i]]();
			//lastWFactor[i][j] = new float[neuronsInLayer[i]];
			//lastWSign[i][j] = new bool[neuronsInLayer[i]];
		}
	}
	this->initializeParameters();
}
template<float a(float),float as(float)>FullyConnectedNetworkTemplate<a,as>::FullyConnectedNetworkTemplate(
		const FullyConnectedNetworkTemplate &toCopy, bool useSameParameters) :
		NeuralNetwork(toCopy.inputs, toCopy.outputs), w(toCopy.w), b(toCopy.b), dEdW(
				toCopy.dEdW), dEdB(toCopy.dEdB), isOriginal(false), neuronsInLayer(
				toCopy.neuronsInLayer), numOfLayers(toCopy.numOfLayers) {
	net = new float*[numOfLayers];
	for (int i = 0; i < numOfLayers; i++) {
		net[i] = new float[neuronsInLayer[i]];
	}

}
template<float a(float),float as(float)> NeuralNetwork* FullyConnectedNetworkTemplate<a,as>::clone() {
	return new FullyConnectedNetworkTemplate(neuronsInLayer, numOfLayers);
}
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::printWeights() {
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::updateOutput() {
	float *output = new float[neuronsInLayer[0]];
	delete net[0];
	net[0] = getInput();
	for (int i = 0; i < neuronsInLayer[0]; i++) {
		output[i] = a(net[0][i]);
	}
	for (int i = 1; i < numOfLayers; i++) {
		//input = new float[neuronsInLayer[i]];
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			//fuer jedes Neuron der Zielschicht
			float netValue = b[i][j];
			for (int k = 0; k < neuronsInLayer[i - 1]; k++) {
				//fuer jedes Neuron der vorherigen Schicht
				netValue += output[k] * w[i - 1][j][k];
			}
			net[i][j] = netValue;
		}
		delete[] output;
		output = new float[neuronsInLayer[i]];
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			output[j] = a(net[i][j]);
		}
	}
	for (int i = 0; i < neuronsInLayer[numOfLayers - 1]; i++) {
		this->output[i] = output[i];
	}
	delete[] output;
}
template<float a(float),float as(float)> FullyConnectedNetworkTemplate<a,as>::~FullyConnectedNetworkTemplate() {
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::updateParameters(float alpha) {
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::addDerivatives() {
	int neuronsInLastLayer = neuronsInLayer[numOfLayers - 1];
	float *dEdNet = new float[neuronsInLastLayer];
	for (int i = 0; i < neuronsInLastLayer; i++) {
		dEdNet[i] = *dEdOut[i] * as(net[numOfLayers - 1][i]);
	}
	for (int i = numOfLayers - 2; i >= 0; i--) {
		int neuronsInPrevLayer = neuronsInLayer[i];
		int neuronsInCurrentLayer = neuronsInLayer[i + 1];
		float *newdEdNet = new float[neuronsInPrevLayer];
		//fuer jede Schicht
		for (int j = 0; j < neuronsInPrevLayer; j++) {
			//fuer jedes Neuron der Ausgangsschicht
			float dEdOut = 0;
			float currentOutput = a(net[i][j]);
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::copyParameters(NeuralNetwork *const toCopy) {
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::updateParameters(
		void (*updateFunction)(float &derivative, float &oldValue,
				char *parameters)) {
	if (!isOriginal) {
		return;
	}
	for (int i = 0; i < numOfLayers - 1; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			for (int k = 0; k < neuronsInLayer[i + 1]; k++) {
				float &currentWeight=w[i][k][j];
				float &currentDerivative=dEdW[i][k][j];
				updateFunction(currentDerivative,currentWeight,updateParameter[i][k][j]);
				currentDerivative = 0;
			}
		}
	}
	for (int i = 0; i < numOfLayers; i++) {
		for (int j = 0; j < neuronsInLayer[i]; j++) {
			float &currentBias=b[i][j];
			float &currentDerivative=dEdB[i][j];
			updateFunction(currentDerivative,currentBias,bUpdateParameter[i][j]);
			currentDerivative = 0;
		}
	}

}
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::createBufferStorage(int numOfBytes) {
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
template<float a(float),float as(float)> void FullyConnectedNetworkTemplate<a,as>::addDerivatives(NeuralNetwork *const toCopy) {
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
EMSCRIPTEN_BINDINGS(FullyConnectedNetworkTemplate) {
  class_<FullyConnectedNetworkTemplate<>, base<NeuralNetwork>>(
      "FullyConnectedNetworkTemplate")
	  .property("neuronsInLayer",&FullyConnectedNetworkTemplate<>::getNeuronsInLayer)
      .constructor(getFCN, allow_raw_pointers())
      .constructor<>();

}
} /* namespace std */
