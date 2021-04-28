#pragma once

#include "../Math/ScalarFuncs.h"
#include "../Math/Matrix.h"
#include <vector>
#include <iostream>

using namespace std;

struct IntermediateData {
    vector<vector<float>> neuronActivations;
    vector<vector<float>> neuronActivationsPreSigmoid;
    vector<vector<float>> costsWRTActivationsGradient;
    vector<Matrix<float>> costsWRTWeightsGradient;
    vector<vector<float>> costsWRTBiasesGradient;
};

class NeuralNet {
private:
    vector<Matrix<float>> weights;
    vector<vector<float>> biases;

    // These are updated and changed thoughout training
    IntermediateData data;

    vector<IntermediateData> intermediateDataBatch;

    int layers;
    float scalarFuncsCompressFactor;  // if using relu, then this is the "compression" on it
    
public:

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

    void calcCostsWRTActivationsGradient(const int requestedOutputIdx);

    void calcCostsWRTWeightsGradient();

    void calcCostsWRTBiasesGradient();

    void calcIntermediateData(const vector<float>& in, const int requestedOutputIdx);

    void calcIntermediateBatchData(const vector<vector<float>>& inBatch, const vector<int> requestedOutputIdxBatch);

    void makeTrainingStep(const int requestedOutputIdx);

    /**
     * @brief train the network on a set of data
     *
     * @param trainingInputSet the set of input layer values
     * @param expectedOutNeuron expectedOutNeuron[i] is the index of the
     * output layer neuron whose value to be 1.00, with all others to be 0.00
     * for trainingInputSet[i]
     * @param batchSize the size of each training batch
     */
    void train(const vector<vector<float>>& trainingSet, const vector<int>& expectedOutNeuron, const int batchSize = 10);


    void writeToFile(const string& pathToFile);

    /**
     * @brief Outputs the data in the neural network in a json format that can be parsed back in again for future use
     *
     * @param out output stream to file
     * @param nn neural network
     * @return ostream&
     */
    friend ostream& operator << (ostream& out, const NeuralNet& nn);
};