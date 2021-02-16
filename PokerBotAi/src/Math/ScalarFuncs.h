#pragma once

class ScalarFuncs
{
private:
    const float reluCompressFactor;
public:
    ScalarFuncs(float reluCompressFactor = 1.0);
    ~ScalarFuncs();

    float sigmoid(float z);
    

    float relu(float z);
    
    // the following are implementations of derivatives that can be found in here: https://www.youtube.com/watch?v=tIeHLnjs5U8

    float dzWRTdw(float a);
    float dzWRTdb();
    float dzWRTda_previous(float w);
    float daWRTdz_sigmoid(float z);
    float daWRTdz_relu(float z);
    float dcWRTda(float a, float y);
};
