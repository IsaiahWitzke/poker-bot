#include "ScalarFuncs.h"
#include <math.h>

float sigmoid(float a) {
    return 1 / (1 + exp(-1 * a));
}

float relu(float a) {
    return fmax(0.0, a / 2);
}

float approxSigmoid(float a) {
    return 0.5;
}