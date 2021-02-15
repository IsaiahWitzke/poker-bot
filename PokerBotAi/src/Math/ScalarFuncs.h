#pragma once

class ScalarFuncs
{
private:
    const float reluCompressFactor;
public:
    ScalarFuncs(float reluCompressFactor = 1.0);
    ~ScalarFuncs();

    float sigmoid(float a);
    float sigmoidPrime(float a);

    float relu(float a);
    float reluPrime(float a);

    float approxSigmoid(float a);   // if needed, implement me (for speed)
    float approxSigmoidPrime(float a);

};
