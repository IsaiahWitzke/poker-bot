#include "ScalarFuncs.h"
#include <cmath>

ScalarFuncs::ScalarFuncs(float reluCompressFactor) : reluCompressFactor(reluCompressFactor) {}

ScalarFuncs::~ScalarFuncs() {}

float ScalarFuncs::sigmoid(float z) {
    return 1 / (1 + exp(-1 * z));
}

float ScalarFuncs::relu(float z) {
    return fmax(0.0, z * reluCompressFactor);
}

// *****
//
// begin derivatives implementations 
// see https://www.youtube.com/watch?v=tIeHLnjs5U8 for more info (variable names, math)
//
// *****

float ScalarFuncs::dzWRTda(float w) {
    return w;
}

float ScalarFuncs::dzWRTdb() {
    return 1;
}

float ScalarFuncs::daWRTdz_sigmoid(float z) {
    return (exp(-1 * z) / pow(1 + exp(-1 * z), 2));
}

float ScalarFuncs::daWRTdz_relu(float z) {
    if (z < 0) {
        return 0;
    }
    else {
        return reluCompressFactor;
    }
}


float ScalarFuncs::dcWRTda(float a, float y) {
    return (2 * (a - y));   // I think?
}
