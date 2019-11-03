/*
 * PlugIn.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_PLUGIN_H_
#define NETWORKTYPES_PLUGIN_H_

namespace std {

class PlugIn {
public:
	PlugIn(int inputs):dEdIn(new double[inputs]){
		this->inputs=inputs;
		this->input=new double*[inputs];
	}
	double* getInput(){
		double* toReturn=new double[inputs];
		for(int i=0;i<inputs;i++){
			toReturn[i]=*(input[i]);
		}
		return toReturn;
	}
	double* const dEdIn;
	double** input;
	int inputs;
	~PlugIn(){
		delete dEdIn;
		delete input;
	}
};

} /* namespace std */

#endif /* NETWORKTYPES_PLUGIN_H_ */
