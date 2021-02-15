#include "NeuralNet.h"
#include "../Math/VectorUtils.h"
#include <iostream>

NeuralNet::NeuralNet(istream in, float scalarFuncsCompressFactor) : l(-1), scalarFuncs(scalarFuncsCompressFactor) {}

NeuralNet::NeuralNet(vector<int> neuronsInLayer, float scalarFuncsCompressFactor) : l(neuronsInLayer.size()), scalarFuncs(scalarFuncsCompressFactor) {
    // TODO: param checking

    // initialize all weights and biases to random values
    for (size_t i = 0; i < l - 1; i++) {
        Matrix<float> weights(neuronsInLayer[i + 1], neuronsInLayer[i], 0.0);     // each row reps all the links from input neuron to next neurons
        weights.randomize();
        this->weights.push_back(weights);
        vector<float> biases(neuronsInLayer[i + 1], 0.0);
        randomizeVector(biases);
        this->biases.push_back(biases);
    }
}

vector<float> NeuralNet::operator()(const vector<float>& in) {
    // NOTE: we dont apply biases or sigmoid to input layer


    vector<float> intermediateData = in;
    for (size_t i = 0; i < l - 1; i++) {
        // next data = weights matrix * input values + biases
        intermediateData = weights[i](intermediateData) + biases[i];
        for (size_t j = 0; j < intermediateData.size(); j++) {
            intermediateData[j] = scalarFuncs.relu(intermediateData[j]);
        }
    }
    return intermediateData;
}


void NeuralNet::calcIntermediateValues(const vector<float>& in, vector<vector<float>>& a, vector<vector<float>>& z) {
    // NOTE: we dont apply biases or sigmoid to input layer
    a = {in};
    z = {in};

    // temp intermediate data vectors which will update for each layer
    vector<float> preSigmoid;
    vector<float> postSigmoid = in;
    
    // for each layer...
    for (size_t i = 0; i < l - 1; i++) {
        // next data = (weights matrix) * (input values vector) + (biases vector)
        preSigmoid = (weights[i] * postSigmoid) + biases[i];
        z.push_back(preSigmoid);
        
        postSigmoid = preSigmoid;

        // for each neuron...
        for (size_t j = 0; j < preSigmoid.size(); j++) {
            // apply the sigmoid
            postSigmoid[j] = scalarFuncs.relu(preSigmoid[j]);
        }
        a.push_back(postSigmoid);
    }
}

void NeuralNet::train(const vector<vector<float>>& trainingSet, const vector<int>& expectedOutNeuron) {
    // for (size_t i = 0; i < trainingSet.size(); i++) {
    //     // output of all layers
    //     vector<vector<float>> a();  

    //     for (size_t resultIdx = 0; resultIdx < trainingResults.size(); resultIdx++) {
    //         // calc dC[resultIdx]/da[j]^(L)

    //         for (size_t outputNeuronIdx = 0; outputNeuronIdx < trainingResults[resultIdx].size(); outputNeuronIdx++) {

    //         }

    //     }

    //     vector<float> weightsGradient(trainingResults[0].size(), 0.0);
    //     vector<float> biasesGradient(trainingResults[0].size(), 0.0);
    // }
}