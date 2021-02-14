#include "NeuralNet.h"
#include "../Math/ScalarFuncs.h"
#include "../Math/VectorUtils.h"
#include <iostream>

NeuralNet::NeuralNet(istream in) {
    std::cout << "neural net" << std::endl;
}

NeuralNet::NeuralNet(vector<int> neuronsInLayer) {
    // TODO: param checking

    // initialize all weights and biases to random values
    for (size_t i = 0; i < neuronsInLayer.size() - 1; i++) {
        Matrix<float> weights(neuronsInLayer[i + 1], neuronsInLayer[i], 0.0);     // each row reps all the links from input neuron to next neurons
        weights.randomize();
        this->weights.push_back(weights);
        vector<float> biases(neuronsInLayer[i + 1], 0.0);
        randomizeVector(biases);
        this->biases.push_back(biases);
    }
}

vector<float> NeuralNet::operator()(const vector<float> in) {
    const int layers = weights.size();  // doesn't include the input layer
    vector<float> intermediateData = in;
    for (size_t i = 1; i < layers; i++) {
        // next data = weights matrix * input values + biases
        intermediateData = weights[i](intermediateData) + biases[i];
        for (size_t j = 0; j < intermediateData.size(); j++) {
            intermediateData[j] = relu(intermediateData[j]);
        }
    }
    return intermediateData;
}
