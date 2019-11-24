/*
 * main.cpp
 *
 *  Created on: Jul 1, 2019
 *      Author: olep
 */

#include <iostream>
#include <math.h>

#include "networkTypes/ErrorDefiner.h"
#include "networkTypes/FullyConnectedNetwork.h"
#include "networkTypes/FullyConnectedNetworkTemplate.h"
#include "networkTypes/NetworkContainer.h"
#include "networkTypes/NeuralNetwork.h"
#include "networkTypes/Scaler.h"
#include "training.h"
#include "OCLNetwork.h"
//#include "trainingMethods/Momentum.h"
#include "trainingMethods/RProp.h"
//#include "openCl/openclTools.h"
#include "openCl/OpenClExecuter.h"
using namespace std;

double lin(double x) {
	return x;
}
double lins(double x) {
	return 1;
}
void constructContainer(NetworkContainer &container) {

	//FullyConnectedNetwork with 3 layers with 5,4,2 neurons
	FullyConnectedNetworkTemplate<lin, lins> *network1 =
			new FullyConnectedNetworkTemplate<lin, lins>(new int[3] { 5, 4, 3 },
					3);

	//FullyConnectedNetwork with 3 layers with 5,3,2 neurons
	FullyConnectedNetwork *network2 = new FullyConnectedNetwork(new int[3] { 5,
			5, 3 }, 3);

	//FullyConnectedNetwork with 2 layers with 4,3 neurons
	FullyConnectedNetwork *network3 = new FullyConnectedNetwork(new int[3] { 6,
			4, 3 }, 3);

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
	container.makeNetworkChildOf(network3, network1, 0, 3, 0);
	container.makeNetworkChildOf(network3, network2, 0, 3, 3);
	container.makeNetworkChildOf(errorDef, network3, 0);
	container.makeNetworkChildOf(&(container.out), errorDef);

}
#define numOfExamples 1000

int main() {
	OCLNetwork x(new int[3]{4,3,2},3,"x*4.0","2.0");
	float* a=new float[3];
	float *r=new float[3];
	for(int i=0;i<3;i++){
		a[i]=i;
	}
	x.applyA(a, r, 3);
	for(int i=0;i<3;i++){
		cout<<r[i]<<endl;
	}
	return 0;
	//initializeOpencl();
	//cl_kernel kernel=loadKernel("src/openCl/vector_add_kernel.cl", NULL);
	OpenClExecuter exe;
	/*int *A=new int[3]{1,2,3};
	int *B=new int[3]{1,2,3};
	int *C=exe.add(A, B, 3);
	for(int i=0;i<3;i++){
		cout<<C[i]<<endl;
	}*/

#define col 400
#define row 7
	float *matrix=new float[col*row];
	float *vec=new float[col];
	float *res=new float[row];
	for(int i=0;i<col*row;i++){
		matrix[i]=1;
	}
	for(int i=0;i<col;i++){
		vec[i]=i;
	}
	exe.matrixVectorMult(res, matrix, vec, col, row);
	for(int i=0;i<row;i++){
		cout<<res[i]<<endl;
	}
	return 0;
	NetworkContainer container(10, 3, 3, constructContainer);
	double ***trainingExamples = new double**[numOfExamples];
	for (int i = 0; i < numOfExamples; i++) {
		trainingExamples[i] = new double*[2];
		trainingExamples[i][0] = new double[10];
		trainingExamples[i][1] = new double[3];
		double *input = trainingExamples[i][0];
		for (int j = 0; j < 10; j++) {
			input[j] = 1 * (double) random() / (double) RAND_MAX;
		}
		for (int j = 0; j < 3; j++) {
			trainingExamples[i][1][j] = (input[3 * j] + input[3 * j + 1]
					+ input[3 * j + 2]) / 3;
		}
	}
	for (int i = 0; i < 3; i++) {
		cout << trainingExamples[100][1][i] << "|";
	}
	cout << endl << endl;
	container.updateOutput(trainingExamples[0][0], trainingExamples[0][1]);
	container.printOutput();
	container.updateParameters(0.0);
	/*return 0;
	 container.addDerivatives();
	 container.updateParameters(1);
	 container.updateOutput(trainingExamples[0][0], trainingExamples[0][1]);
	 container.printOutput();
	 return 0;*/
	container.createBufferStorage(RPropByteAmount);
	double minValue = MAXFLOAT;
	NetworkContainer *BestConfig =
			dynamic_cast<NetworkContainer*>(container.clone());
	for (int i = 0; i < 5000; i++) {
		if (true) {
			train(&container, trainingExamples, numOfExamples, 4);
		} else {
			computeDerivatives(&container, trainingExamples, numOfExamples);
		}
		cout << container.value << endl;
		if (container.value + 0.3 < minValue) {
			//if there is significant improvement
			minValue = container.value;
			BestConfig->copyParameters(&container);
		}
		if (container.value > minValue + 1.0) {
			//if the error value increased too much
			container.updateParameters(0.0);	//reset derivatives
			container.copyParameters(BestConfig);	//reset parameters
			container.createBufferStorage(RPropByteAmount);	//reset "momentum gained"
			cout << "resetting to last backup" << endl;
			continue;
		}
		//dynamic_cast<FullyConnectedNetworkTemplate<>*>(container.networks[2])->printWeights();
		cout << endl << endl;
		container.updateParameters(RProp);
		//container.updateParameters(0.0);
	}
	cout << "RProp finished" << endl;
	//OpenClExecuter exe;
	return 0;
}
