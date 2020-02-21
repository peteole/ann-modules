#include "RProp.h"
void RProp(float &derivative, float &oldValue, char *parameters) {
  bool &lastSign = *(reinterpret_cast<bool *>(parameters));
  float &lastFactor = *(reinterpret_cast<float *>(parameters + 1));
  float factor;
  bool newSign = derivative > 0;
  if (newSign == lastSign) {
    factor = std::min(lastFactor * 1.1, maxVal);
  } else {
    factor = std::max(lastFactor * 0.1, minVal);
  }
  lastSign = newSign;
  lastFactor = factor;
  if (newSign) {
    oldValue -= factor;
  } else {
    oldValue += factor;
  }
}