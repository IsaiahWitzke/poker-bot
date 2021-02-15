#include "ScalarFuncs.h"
#include <cmath>

ScalarFuncs::ScalarFuncs(float reluCompressFactor) : reluCompressFactor(reluCompressFactor) {}

ScalarFuncs::~ScalarFuncs() {}

float ScalarFuncs::sigmoid(float a) {
    return 1 / (1 + exp(-1 * a));
}

float ScalarFuncs::sigmoidPrime(float a) {
    return (exp(-1 * a) / pow(1 + exp(-1 * a), 2));
}

float ScalarFuncs::relu(float a) {
    return fmax(0.0, a * reluCompressFactor);
}

float ScalarFuncs::reluPrime(float a) {
    if (a < 0) {
        return 0;
    }
    else {
        return reluCompressFactor;
    }
}

// TODO: implement fully later (if needed)
float ScalarFuncs::approxSigmoid(float a) {
    return 0.5;
}