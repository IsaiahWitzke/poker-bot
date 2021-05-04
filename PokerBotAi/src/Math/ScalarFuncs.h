#pragma once

#include <vector>

using namespace std;

float sigmoid(float z);

vector<float> relu(const vector<float>& z, float reluCompressFactor = 1.0);

float relu(float z, float reluCompressFactor = 1.0);

// the following are implementations of derivatives that can be found in here: https://www.youtube.com/watch?v=tIeHLnjs5U8

float dzWRTdw(float a);

float dzWRTda_previous(float w);
float daWRTdz_sigmoid(float z);

float daWRTdz_relu(float z, float reluCompressFactor = 1.0);

vector<float> daWRTdz_relu(vector<float> z, float reluCompressFactor = 1.0);

template <typename T>
T dcWRTda(T a, T y);
