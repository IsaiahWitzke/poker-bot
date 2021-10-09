#include "TestData.h"
#include "../Math/VectorUtils.h"

/**
 * @brief generates nSets amount of input/expected output data pairs and adds the
 * pairs to inputs and outputs
 *
 */
void TestData::generateDataFor(
    vector<vector<float>>& inputs,
    vector<vector<float>>& outputs,
    size_t nInputRows,
    size_t nOutRows,
    size_t nSets,
    float noiseAmplitude
) {
    for (size_t i = 0; i < nSets; i++) {
        inputs.emplace_back(vector<float>(nInputRows));
        randomizeVector(inputs[i], 10.0f);					// random input, with random elements in [0, 10]
        outputs.push_back(generatingOperation * inputs[i]);	// what the input produces

        vector<float> noise(nOutRows);
        randomizeVector(noise, noiseAmplitude, 0.0f);		// only positive noise????

        // add the noise onto the expected output data
        outputs[i] = outputs[i] + noise;
    }
}

TestData::TestData(
    size_t nInputRows,
    size_t nOutRows,
    size_t nTrainingSets,
    int trainToTestRatio,
    DataSetType dataSetType,
    float noiseAmplitude
) {
    size_t nTestSets = nTrainingSets / trainToTestRatio;

    // what is the correlation between the input and the output going to be?
    generatingOperation = Matrix<float>(nOutRows, nInputRows, 0);

    if (dataSetType == TestData::LINEAR) {
        generatingOperation.randomize(10);	// matrix with random elements in [0, 10]
    }
    else {
        throw runtime_error("other types of TestData generation not implemented");
    }

    generateDataFor(this->trainingInputs, this->trainingExpectedOuts, nInputRows, nOutRows, nTrainingSets, noiseAmplitude);
    generateDataFor(this->testingInputs, this->testingExpectedOuts, nInputRows, nOutRows, nTestSets, noiseAmplitude);
}

TestData::~TestData() {}