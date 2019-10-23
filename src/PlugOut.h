/*
 * PlugOut.h
 *
 *  Created on: Aug 15, 2019
 *      Author: olep
 */

#ifndef PLUGOUT_H_
#define PLUGOUT_H_

namespace std {

class PlugOut {
public:
	bool operator==(const PlugOut p){
		return &p==this;
	}
	PlugOut(int outputs=2):output(new double[outputs]){
		this->outputs=outputs;
		//this->output=new double[outputs];
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

};

} /* namespace std */

#endif /* PLUGOUT_H_ */
