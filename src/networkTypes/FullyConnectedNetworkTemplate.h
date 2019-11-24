/*
 * FullyConnectedNetwork.h
 *
 *  Created on: Jul 2, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_
#define NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_

#define _USE_MATH_DEFINES
#include "math.h"
#include "NeuralNetwork.h"
//#include "NetworkContainer.h"
#include "Sigmoid.h"
namespace std {
template<double a(double)=&sigmoid, double as(double)=&sigmoids> class FullyConnectedNetworkTemplate: public NeuralNetwork {
public:
	FullyConnectedNetworkTemplate(int *neuronsInLayer = new int[3] { 5, 3, 2 },
			int numOfLayers = 3);
	FullyConnectedNetworkTemplate(const FullyConnectedNetworkTemplate &toCopy,
			bool useSameParameters = true);
	NeuralNetwork* clone() override;
	void updateOutput();
	void copyParameters(NeuralNetwork *const toCopy) override;
	void addDerivatives(NeuralNetwork *const toAdd) override;
	~FullyConnectedNetworkTemplate();
	void addDerivatives() override;
	void printWeights();
	void updateParameters(double alpha);
	/*double a(double x) {
	 //return act(x);
	 //return x;
	 return 1 / (1 + pow(M_E, -x));
	 }
	 double as(double x) {
	 //return 1;
	 double c = a(x);
	 return c * (1 - c);
	 }*/
	void updateParameters(
			void (*updateFunction)(double &derivative, double &oldValue,
					char *parameters)) override;
	void createBufferStorage(int numOfBytes) override;
	//template<typename T> void createBufferStorage();
	void initializeParameters() {
		for (int i = 0; i < numOfLayers - 1; i++) {
			for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
				for (int k = 0; k < neuronsInLayer[i]; k++) {
					w[i][j][k] = 0.1 * (double) random() / (double) RAND_MAX
							- 0.05;
					//lastWFactor[i][j][k]=0.00001;
				}
			}
		}
		for (int i = 0; i < numOfLayers; i++) {
			for (int j = 0; j < neuronsInLayer[i]; j++) {
				//b[i][j]=0; //= 0.1 * (double) random() / (double) RAND_MAX - 0.05;
				//lastBFactor[i][j]=0.00001;
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
	//double ***lastWFactor; //1.starting layer 2. output neuron 3. input neuron
	//bool ***lastWSign; //1.starting layer 2. output neuron 3. input neuron
	double **b; //1.layer 2.neuron
	double **dEdB;
	//bool **lastBSign;
	//double **lastBFactor;
	char ****updateParameter = 0;
	char ***bUpdateParameter = 0;
	//NetworkContainer *n;
};

} /* namespace std */

#endif /* NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_ */
