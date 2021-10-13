#pragma once

#include "../Math/ScalarFuncs.h"
#include "../Math/Matrix.h"
#include "../Output/Printable.h"
#include <vector>
#include <iostream>

using namespace std;

struct IntermediateData {
    vector<vector<float>> neuronActivations;
    vector<vector<float>> neuronActivationsPreActivation;
    vector<vector<float>> costsWRTActivationsGradient;
    vector<Matrix<float>> costsWRTWeightsGradient;
    vector<vector<float>> costsWRTBiasesGradient;
};

class NeuralNet : public Printable {
private:
    // These are updated and changed thoughout training
    IntermediateData data;

    vector<IntermediateData> intermediateDataBatch;

    int layers;
    float scalarFuncsCompressFactor;  // if using relu, then this is the "compression" on it

public:
    vector<Matrix<float>> weights;
    vector<vector<float>> biases;

    /**
     * @brief Construct a new Neural Net object from an input file
     *
     * @param filePath
     */
    NeuralNet(const string& filePath);

    /**
     * @brief Construct a new Neural Net object
     *
     * @param neuronsInLayer Represents the number of neurons in each layer.
     * Needs to have at least 2 elems (input layer and output layer)
     * First elem in neuronsInLayer is the input layer, last is output.
     */
    NeuralNet(vector<int> neuronsInLayer, float scalarFuncsCompressFactor = 1.0);


    /**
     * @brief to apply the neural net onto an input. Returns the output activations of the final layer of neurons
     *
     * @param in the input layer's activations
     * @return vector<float>
     */
    vector<float> operator()(const vector<float>& in);

    int getNeuronsInLayer(const int layer) const;

    /**
     * @brief given the input layer activation layer (in), updates the activations of every neuron in the network,
     * and the pre-sigmoid value of every neuron in the network
     *
     * @param in the input layer's activations
     */
    void calcIntermediateValues(const vector<float>& in);

    void calcGradients(const vector<float>& requestedOutput);

    void calcIntermediateData(const vector<float>& in, const vector<float>& requestedOutput);

    void calcIntermediateBatchData(
        const vector<vector<float>>& inputsBatch,
        const vector<vector<float>>& expectedOutputsBatch
    );

    /**
     * @brief nudges weights and biases based on the average gradients found in the
     * current intermediateDataBatch
     */
    void makeTrainingStep(float stepScalingFactor = 1.0);

    /**
     * @brief Given a set of inputs and expected outputs, will train the network
     *
     * @param trainingInputs
     * @param trainingExpectedOuts
     * @param batchSize
     * @param stepSize
     */
    void train(
        const vector<vector<float>>& trainingInputs,
        const vector<vector<float>>& trainingExpectedOuts,
        const int batchSize,
        const float stepSize,
        const vector<vector<float>>& testingInputs,
        const vector<vector<float>>& testingExpectedOutputs
    );

    /**
     * @brief Returns the accuracy of the model when tested against a set of testing data.
     * 
     * Calculates error with "Mean absolute percentage error" (MAPE).
     * See: https://en.wikipedia.org/wiki/Mean_absolute_percentage_error
     * 
     * @param testingInputs 
     * @param trainingExpectedOuts 
     * @return float 
     */
    float test(
        const vector<vector<float>>& testingInputs,
        const vector<vector<float>>& testingExpectedOuts
    );

	string toString(int tabSpaces = 0) const;
};