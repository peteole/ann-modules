/*
 * FullyConnectedNetwork.h
 *
 *  Created on: Jul 2, 2019
 *      Author: olep
 */

#ifndef FULLYCONNECTEDNETWORK_H_
#define FULLYCONNECTEDNETWORK_H_
#define _USE_MATH_DEFINES
#include "math.h"
#include "NeuralNetwork.h"
//#include "NetworkContainer.h"

namespace std {

class FullyConnectedNetwork: public NeuralNetwork {
public:
	FullyConnectedNetwork(int *neuronsInLayer = new int[3] { 5, 3, 2 },
			int numOfLayers = 3);
	FullyConnectedNetwork(const FullyConnectedNetwork &toCopy);
	void updateOutput();
	~FullyConnectedNetwork() {
	}
	void addDerivatives();
	void updateParameters(double alpha);
	double a(double x) {
		//return x;
		return 1/(1+pow(M_E,-x));
	}
	double as(double x) {
		double c=a(x);
		return c*(1-c);
	}
	void initializeParameters(){
		for(int i=0;i<numOfLayers-1;i++){
			for(int j=0;j<neuronsInLayer[i+1];j++){
				for(int k=0;k<neuronsInLayer[i];k++){
					w[i][j][k]=1;
				}
			}
		}
	}
	const bool isOriginal;
//private:
	int numOfLayers;
	int *neuronsInLayer;
	double **net;
	double ***w; //1.starting layer 2. output neuron 3. input neuron
	double ***dEdW; //1.starting layer 2. output neuron 3. input neuron
	double **b; //1.layer 2.neuron
	double **dEdB;
	//NetworkContainer *n;
};

} /* namespace std */

#endif /* FULLYCONNECTEDNETWORK_H_ */
