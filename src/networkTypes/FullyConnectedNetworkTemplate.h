/*
 * FullyConnectedNetwork.h
 *
 *  Created on: Jul 2, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_
#define NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_

#define _USE_MATH_DEFINES
#include "NeuralNetwork.h"
#include "math.h"
#include "stringVector.h"
#include <vector>
//#include "NetworkContainer.h"
#include "Sigmoid.h"

#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;

namespace std {

template <float a(float) = &sigmoid, float as(float) = &sigmoids>
class FullyConnectedNetworkTemplate : public NeuralNetwork {
public:
  FullyConnectedNetworkTemplate(int *neuronsInLayer = new int[3]{5, 3, 2},
                                int numOfLayers = 3);
  FullyConnectedNetworkTemplate(const FullyConnectedNetworkTemplate &toCopy,
                                bool useSameParameters = true);
  NeuralNetwork *clone() override;
  void updateOutput() override;
  void copyParameters(NeuralNetwork *const toCopy) override;
  void addDerivatives(NeuralNetwork *const toAdd) override;
  ~FullyConnectedNetworkTemplate();
  void addDerivatives() override;
  void printWeights();
  void updateParameters(float alpha) override;
  /*float a(float x) {
   //return act(x);
   //return x;
   return 1 / (1 + pow(M_E, -x));
   }
   float as(float x) {
   //return 1;
   float c = a(x);
   return c * (1 - c);
   }*/
  void updateParameters(void (*updateFunction)(float &derivative,
                                               float &oldValue,
                                               char *parameters)) override;
  void createBufferStorage(int numOfBytes) override;
  // template<typename T> void createBufferStorage();
  void initializeParameters() {
    for (int i = 0; i < numOfLayers - 1; i++) {
      for (int j = 0; j < neuronsInLayer[i + 1]; j++) {
        for (int k = 0; k < neuronsInLayer[i]; k++) {
          w[i][j][k] = 0.1 * (float)random() / (float)RAND_MAX - 0.05;
          // lastWFactor[i][j][k]=0.00001;
        }
      }
    }
    for (int i = 0; i < numOfLayers; i++) {
      for (int j = 0; j < neuronsInLayer[i]; j++) {
        // b[i][j]=0; //= 0.1 * (float) random() / (float) RAND_MAX - 0.05;
        // lastBFactor[i][j]=0.00001;
      }
    }
  }
  const bool isOriginal;
  // private:
  int numOfLayers;
  int *neuronsInLayer;
  float **net;
  float ***w;    // 1.starting layer 2. output neuron 3. input neuron
  float ***dEdW; // 1.starting layer 2. output neuron 3. input neuron
  // float ***lastWFactor; //1.starting layer 2. output neuron 3. input neuron
  // bool ***lastWSign; //1.starting layer 2. output neuron 3. input neuron
  float **b; // 1.layer 2.neuron
  float **dEdB;
  // bool **lastBSign;
  // float **lastBFactor;
  char ****updateParameter = 0;
  char ***bUpdateParameter = 0;
  string getNeuronsInLayer() const{
	  string toReturn="|";
	  for(int i=0;i<numOfLayers;i++){
		  toReturn+=to_string(neuronsInLayer[i]);
		  toReturn+="|";
	  }
	  return toReturn;
  }
  // NetworkContainer *n;
};
FullyConnectedNetworkTemplate<> *getFCN(string in) {
  vector<int> *nil = stringToVector(in);
  return new FullyConnectedNetworkTemplate<>(getArrayFromVector(*nil),
                                             nil->size());
}
} /* namespace std */

#endif /* NETWORKTYPES_FULLYCONNECTEDNETWORKTEMPLATE_H_ */
