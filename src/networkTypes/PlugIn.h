/*
 * PlugIn.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_PLUGIN_H_
#define NETWORKTYPES_PLUGIN_H_
#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;

namespace std {

class PlugIn {
public:
	PlugIn(int inputs):dEdIn(new float[inputs]){
		this->inputs=inputs;
		this->input=new float*[inputs];
	}
	float* getInput(){
		float* toReturn=new float[inputs];
		for(int i=0;i<inputs;i++){
			toReturn[i]=*(input[i]);
		}
		return toReturn;
	}
	float* const dEdIn;
	float** input;
	int inputs;
	int getInputs() const{return inputs;}
	~PlugIn(){
		delete[] dEdIn;
		delete[] input;
	}
};



} /* namespace std */
#endif /* NETWORKTYPES_PLUGIN_H_ */
