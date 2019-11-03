/*
 * PlugOut.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef NETWORKTYPES_PLUGOUT_H_
#define NETWORKTYPES_PLUGOUT_H_

namespace std {

class PlugOut {
public:
	bool operator==(const PlugOut p){
		return &p==this;
	}
	PlugOut(int outputs=2):output(new double[outputs]){
		this->outputs=outputs;
		this->dEdOut=new double*[outputs];
	}
	double* const output;
	double* getDEdOut(){
		double* toReturn=new double[outputs];
		for(int i=0;i<outputs;i++){
			toReturn[i]=*dEdOut[i];
		}
		return toReturn;
	}

	int outputs;
	double** dEdOut;
	~PlugOut(){
		delete output;
		delete dEdOut;
	}
};

} /* namespace std */

#endif /* NETWORKTYPES_PLUGOUT_H_ */
