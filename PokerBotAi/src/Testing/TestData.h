#pragma once

#include <vector>
#include "../Math/Matrix.h"

using namespace std;

/**
 * @brief A class that generates and contains test inputs and
 * outputs that can be used to test the neural network
 * 
 * TODO: make me a templated class
 */
class TestData {
public:
    enum DataSetType {
        LINEAR,
        POLYNOMIAL	// not implemented
    };

    vector<vector<float>> trainingInputs;
    vector<vector<float>> trainingExpectedOuts;
    vector<vector<float>> testingInputs;
    vector<vector<float>> testingExpectedOuts;

    // really should have some abstract "Function" class and Matrix should inherit it...
    // but for now only testing with linear transformations
    Matrix<float> generatingOperation;

    TestData(
        size_t nInputRows,
        size_t nOutRows,
        size_t nTrainingSets,
        int trainToTestRatio = 4,
        DataSetType dataSet = TestData::LINEAR,
        float noiseAmplitude = 0.0
    );
    ~TestData();

private:
    void generateDataFor(
        vector<vector<float>>& inputs,
        vector<vector<float>>& outputs,
        size_t nInputRows,
        size_t nOutRows,
        size_t nSets,
        float noiseAmplitude = 1.0
    );
};