#include "PlugOut.h"
using namespace std;

EMSCRIPTEN_BINDINGS(PlugOut) {
  class_<PlugOut>("PlugOut")
  .property("outputs", &PlugOut::getOutputs)
  .property("output",&PlugOut::getOutput)
  ;
}