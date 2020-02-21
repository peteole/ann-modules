/*
 * NetworkContainer.h
 *
 *  Created on: Jul 10, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_NETWORKCONTAINER_H_
#define NETWORKTYPES_NETWORKCONTAINER_H_

#include "list"
#include <iostream>
#include <map>

#include "NeuralNetwork.h"

using namespace std;

class NetworkContainer : public NeuralNetwork {
public:
  NetworkContainer(int inputs, int outputs, int evaluationParameters,
                   void (*construct)(NetworkContainer &toConstruct));
  NetworkContainer(int inputs, int outputs, int evaluationParameters);
  void (*construct)(NetworkContainer &toConstruct);
  PlugOut in;
  PlugIn out;
  float value = 0;
  float getValue() const{return value;}
  void addNeuralNetwork(NeuralNetwork *toAdd);
  void makeNetworkChildOf(PlugIn *child, PlugOut *parent,
                          int firstParentPortToUse = 0,
                          int amountOfPortsToUse = -1,
                          int firstChildPortToUse = 0);
  void makeNetworkChildOfWrap(NeuralNetwork *child, NeuralNetwork *parent,
                              int firstParentPortToUse = 0,
                              int amountOfPortsToUse = -1,
                              int firstChildPortToUse = 0) {
    if (child == this) {
      makeNetworkChildOf(&(this->out),parent,firstParentPortToUse,amountOfPortsToUse,firstChildPortToUse);
    }else if(parent==this){
      makeNetworkChildOf(child,&(this->in),firstParentPortToUse,amountOfPortsToUse,firstChildPortToUse);
    }else{
      makeNetworkChildOf(child,parent,firstParentPortToUse,amountOfPortsToUse,firstChildPortToUse);
    }
  }
  void updateParameters(float alpha) override;
  void addDerivatives() override;
  void updateOutput() override;
  void updateOutput(float *input, float *evaluationParameters = new float[0]);
  bool makeOrder();
  NeuralNetwork **networks = 0;
  int numOfNetworks = 0;
  void printOutput() {
    for (int i = 0; i < out.inputs; i++) {
      cout << "|" << *(out.input[i]);
    }
    cout << "|. Value: " << this->value << endl;
  }
  float *const evaluationParameters;
  const int numOfEvaluationParameters;
  void copyParameters(NeuralNetwork *const toCopy) override {
    NetworkContainer *toUse = dynamic_cast<NetworkContainer *>(toCopy);
    if (!toUse) {
      return;
    }
    for (int i = 0; i < numOfNetworks; i++) {
      networks[i]->copyParameters(toUse->networks[i]);
    }
  }
  void addDerivatives(NeuralNetwork *const toCopy) override {
    NetworkContainer *toUse = dynamic_cast<NetworkContainer *>(toCopy);
    if (!toUse) {
      return;
    }
    for (int i = 0; i < numOfNetworks; i++) {
      networks[i]->addDerivatives(toUse->networks[i]);
    }
  }
  NeuralNetwork *clone() override {
    return new NetworkContainer(inputs, outputs, numOfEvaluationParameters,
                                construct);
  }
  ~NetworkContainer() {
    for (int i = 0; i < numOfNetworks; i++) {
      delete networks[i];
    }
    delete networks;
  }
  void updateParameters(void (*updateFunction)(float &derivative,
                                               float &oldValue,
                                               char *parameters)) override;
  void createBufferStorage(int numOfBytes) override;
  int getNumOfNetworks() const { return numOfNetworks; }
  PlugOut getIn() const { return in; }
  PlugIn getOut() const { return (this->out); }

private:
  struct commitment {
    PlugOut *parent;
    int firstParentPortToUse;
    int amountOfPortsToUse;
    int firstChildPortToUse;
    int parentID = -1;
  };
  class NetSorter {
  public:
    NeuralNetwork *n;
    list<PlugOut *> before;
    list<PlugIn *> after;
    list<commitment *> connections;
    NetSorter(NeuralNetwork *n) { this->n = n; }
    bool isAddable() {
      // check if all plugs are plugged in
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

      // check if any network needs to be added before

      if (before.size() != 1) {
        return false;
      }
      return true;
    }
  };
  list<NetSorter *> networkSorter;
  NetSorter *getSorterOf(NeuralNetwork *n) {
    for (list<NetSorter *>::iterator it = networkSorter.begin();
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

#endif /* NETWORKTYPES_NETWORKCONTAINER_H_ */
