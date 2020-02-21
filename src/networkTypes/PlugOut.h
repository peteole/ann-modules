/*
 * PlugOut.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_PLUGOUT_H_
#define NETWORKTYPES_PLUGOUT_H_
#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;

namespace std {

class PlugOut {
public:
  bool operator==(const PlugOut p) { return &p == this; }
  PlugOut(int outputs = 2) : output(new float[outputs]) {
    this->outputs = outputs;
    this->dEdOut = new float *[outputs];
  }
  float *const output;
  float *getDEdOut() {
    float *toReturn = new float[outputs];
    for (int i = 0; i < outputs; i++) {
      toReturn[i] = *dEdOut[i];
    }
    return toReturn;
  }

  int outputs;
  int getOutputs() const { return outputs; }
  string getOutput() const{
	  string toReturn="|";
	  for(int i=0;i<outputs;i++){
		  toReturn+=to_string(output[i]);
		  toReturn+="|";
	  }
	  return toReturn;
  }
  float **dEdOut;
  ~PlugOut() {
    delete[] output;
    delete[] dEdOut;
  }
};

} /* namespace std */
#endif /* NETWORKTYPES_PLUGOUT_H_ */
