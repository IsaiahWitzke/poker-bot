#pragma once

#include "../Math/ScalarFuncs.h"
#include "../Math/Matrix.h"
#include <vector>
#include <iostream>

using namespace std;

class NeuralNet {
private:
    vector<Matrix<float>> weights;
    vector<vector<float>> biases;

    const int l;  // number of layers in the network

    ScalarFuncs scalarFuncs;
public:

    /**
     * @brief Construct a new Neural Net object from an input (file) stream
     *
     * @param model the steam from which the pretrained net's data comes from
     */
    NeuralNet(istream in, float scalarFuncsCompressFactor = 1.0);

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
     * @param in
     * @return vector<float>
     */
    vector<float> operator()(const vector<float>& in);


    /**
     * @brief given the input layer activation layer (in), sets a to the activations of every neuron in the network,
     * and z to the pre-sigmoid value of every neuron in the network
     *
     * @param in
     * @param a
     * @param z
     */
    void calcIntermediateValues(const vector<float>& in, vector<vector<float>>& a, vector<vector<float>>& z);

    vector<vector<float>> calcNeuronActivationsGradient(const vector<vector<float>>& a, const vector<vector<float>>& z, const int requestedOutputIdx);

    vector<Matrix<float>> calcWeightsGradient(
        const vector<vector<float>>& a,
        const vector<vector<float>>& z,
        const vector<vector<float>>& dcWRTda,
        const int requestedOutputIdx
    );

    vector<vector<float>> calcBiasesGradient(
        const vector<vector<float>>& a,
        const vector<vector<float>>& z,
        const vector<vector<float>>& dcWRTda,
        const int requestedOutputIdx
    );

    void makeTrainingStep(
        const vector<vector<float>>& a,
        const vector<vector<float>>& z,
        const int requestedOutputIdx
    );

    /**
     * @brief train the network on a set of data
     *
     * @param trainingInputSet the set of input layer values
     * @param expectedOutNeuron expectedOutNeuron[i] is the index of the
     * output layer neuron whose value to be 1.00, with all others to be 0.00
     * for trainingInputSet[i]
     */
    void train(const vector<vector<float>>& trainingSet, const vector<int>& expectedOutNeuron);
};