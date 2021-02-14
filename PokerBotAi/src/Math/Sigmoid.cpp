#include "Sigmoid.h"
#include <math.h>

float sigmoid(float a) {
    return 1 / (1 + exp(-1 * a));
}

float approxSigmoid(float a) {
    return 0.5;
}