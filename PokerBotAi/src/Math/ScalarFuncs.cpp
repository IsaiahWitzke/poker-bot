#include "ScalarFuncs.h"
#include <cmath>


float sigmoid(float z) {
    return 1 / (1 + exp(-1 * z));
}

float relu(float z, float reluCompressFactor) {
    return fmax(0.0, z * reluCompressFactor);
}

// *****
//
// begin derivatives implementations 
// see https://www.youtube.com/watch?v=tIeHLnjs5U8 for more info (variable names, math)
//
// *****

float dzWRTdw(float a) {
    return a;
}

float dzWRTdb() {
    return 1;
}

float dzWRTda_previous(float w) {
    return w;
}

float daWRTdz_sigmoid(float z) {
    return (exp(-1 * z) / pow(1 + exp(-1 * z), 2));
}

float daWRTdz_relu(float z, float reluCompressfactor) {
    if (z < 0) {
        return 0;
    }
    else {
        return reluCompressfactor;
    }
}


float dcWRTda(float a, float y) {
    return (2 * (a - y));   // I think?
}
