#include "NeuralNet.h"
#include "../Math/VectorUtils.h"
#include <iostream>

NeuralNet::NeuralNet(istream in, float scalarFuncsCompressFactor) : l(-1), scalarFuncs(scalarFuncsCompressFactor) {}

NeuralNet::NeuralNet(vector<int> neuronsInLayer, float scalarFuncsCompressFactor) :
    l(neuronsInLayer.size()), scalarFuncs(scalarFuncsCompressFactor) {
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


void NeuralNet::calcIntermediateValues(
    const vector<float>& in,
    vector<vector<float>>& a,
    vector<vector<float>>& z
) {
    // NOTE: we dont apply biases or sigmoid to input layer
    a = { in };
    z = { in };

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


// to calc delta cost wrt the activations of every neuron in the network
vector<vector<float>> NeuralNet::calcNeuronActivationsGradient(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const int requestedOutputIdx
) {
    const int L = l - 1;

    vector<vector<float>> dcWRTda = a; // initializing the future-output as the intermediate activations because a will have the corect dimensions

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 0; layer--) {
        for (size_t neuron = 0; neuron < dcWRTda[layer].size(); neuron++) {
            if (layer == L) { // for the end layer, the solutions are trivial
                float y = 0.0;
                if (neuron == requestedOutputIdx) {
                    y = 1.0;
                }
                dcWRTda[layer][neuron] = this->scalarFuncs.dcWRTda(a[layer][neuron], y);
            }
            else {  // otherwise, changes in activation of the curent neuron has effects on each following neuron
                dcWRTda[layer][neuron] = 0.0;
                for (size_t nextLayerNeuron = 0; nextLayerNeuron < dcWRTda[layer + 1].size(); nextLayerNeuron++) {
                    // start with: dz(L+1)/da(L)
                    float dcWRTdaCurNeuron = scalarFuncs.dzWRTda_previous(weights[layer][nextLayerNeuron][neuron]);
                    // multiply: da(L+1)/dz(L+1)
                    dcWRTdaCurNeuron *= scalarFuncs.daWRTdz_relu(z[layer + 1][nextLayerNeuron]);
                    // multiply: dc(L+1)/da(L+1)
                    dcWRTdaCurNeuron *= dcWRTda[layer + 1][nextLayerNeuron];  // starts to get recursive here!

                    dcWRTda[layer][neuron] += dcWRTdaCurNeuron;
                }
            }
        }
    }
    return dcWRTda;
}

vector<Matrix<float>> NeuralNet::calcWeightsGradient(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const vector<vector<float>>& dcWRTda,
    const int requestedOutputIdx
) {
    const int L = l - 1;

    vector<Matrix<float>> dcWRTdw = weights;

    // start at front (output layer) and propogate backwards
    for (size_t layer = L; layer >= 0; layer--) {

    }

}

vector<vector<float>> NeuralNet::calcBiasesGradient(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const vector<vector<float>>& dcWRTda,
    const int requestedOutputIdx
) {

}

void NeuralNet::makeTrainingStep(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const int requestedOutputIdx
) {

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