#include "NeuralNetwork.h"
using namespace std;
// Binding code
EMSCRIPTEN_BINDINGS(neuralNetwork) {
  class_<NeuralNetwork, base<PlugIn>>("NeuralNetwork")
      .function("addDerivatives",
                select_overload<void()>(&NeuralNetwork::addDerivatives))
      .function("createBufferStorage", &NeuralNetwork::createBufferStorage)
      .function("updateOutput", &NeuralNetwork::updateOutput)
      .function("updateParameters", &NeuralNetwork::jsUpdateParameters)
                //select_overload<void(int)>(&NeuralNetwork::createBufferStorage))
      .property("outputs", &PlugOut::getOutputs)
      .property("output", &PlugOut::getOutput)
      .function("PlugOut", &NeuralNetwork::getPlugOut,allow_raw_pointers());
}