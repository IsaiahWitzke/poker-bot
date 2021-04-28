#pragma once


float sigmoid(float z);


float relu(float z, float reluCompressFactor = 1.0);

// the following are implementations of derivatives that can be found in here: https://www.youtube.com/watch?v=tIeHLnjs5U8

float dzWRTdw(float a);
float dzWRTdb();
float dzWRTda_previous(float w);
float daWRTdz_sigmoid(float z);
float daWRTdz_relu(float z, float reluCompressFactor = 1.0);
float dcWRTda(float a, float y);
