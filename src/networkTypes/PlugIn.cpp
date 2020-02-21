#include "PlugIn.h"
using namespace std;
EMSCRIPTEN_BINDINGS(PlugIn) {
  class_<PlugIn>("PlugIn")
    .property("inputs", &PlugIn::getInputs)
    ;
}