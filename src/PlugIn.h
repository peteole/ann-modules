/*
 * PlugIn.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

namespace std {

class PlugIn {
public:
	PlugIn(int inputs):dEdIn(new double[inputs]){
		this->inputs=inputs;
		//this->dEdIn=new double[inputs];
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
};

} /* namespace std */

#endif /* PLUGIN_H_ */
