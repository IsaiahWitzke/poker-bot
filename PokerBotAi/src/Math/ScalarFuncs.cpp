#include "ScalarFuncs.h"
#include <cmath>
#include <vector>
#include "VectorUtils.h"

using namespace std;

float sigmoid(float z) {
    return 1 / (1 + exp(-1 * z));
}

vector<float> sigmoid(vector<float> z) {
    vector<float> out(z);
    for(size_t i = 0; i < z.size(); i++) {
        out[i] = sigmoid(out[i]);
    }
    return out;
}

float relu(float z, float reluCompressFactor) {
    return fmax(0.0, z * reluCompressFactor);
}

vector<float> relu(const vector<float>& z, float reluCompressFactor) {
    vector<float> out(z);
    for(size_t i = 0; i < z.size(); i++) {
        out[i] = relu(out[i], reluCompressFactor);
    }
    return out;
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

vector<float> daWRTdz_relu(vector<float> z, float reluCompressfactor) {
    vector<float> out(z);
    for (size_t i = 0; i < z.size(); i++) {
        out[i] = daWRTdz_relu(z[i]);
    }
    return out;
}

vector<float> daWRTdz_sigmoid(vector<float> z) {
    vector<float> out(z);
    for (size_t i = 0; i < z.size(); i++) {
        out[i] = daWRTdz_sigmoid(z[i]);
    }
    return out;
}

/*

Cost = sum{over all output neurons} 1/2(outActivation - expectedOutActivation)^2

so...

dCost with respect to dActivation = outActivation - expectedOutActivation

*/

template <typename T>
T dcWRTda(T a, T y) {
    return (a - y);   // I think?
}

template float dcWRTda(float a, float y);
template vector<float> dcWRTda(vector<float> a, vector<float> y);