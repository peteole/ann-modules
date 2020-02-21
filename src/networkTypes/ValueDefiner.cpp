/*
 * ValueDefiner.cpp
 *
 *  Created on: Oct 10, 2019
 *      Author: olep
 */

#include "ValueDefiner.h"

using namespace std;
ValueDefiner::ValueDefiner(int channels, NetworkContainer *container) :
		NeuralNetwork(channels,channels) {
	this->container = container;
}
void ValueDefiner::updateOutput() {
	for (int i = 0; i < inputs; i++) {
		output[i] = *(input[i]);
	}
	this->container->value += this->getValue();
}
 /* namespace std */
EMSCRIPTEN_BINDINGS(ValueDefiner) {
  class_<ValueDefiner, base<NeuralNetwork>>("ValueDefiner");
}