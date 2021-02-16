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
            intermediateData[j] = scalarFuncs.sigmoid(intermediateData[j]);
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
            postSigmoid[j] = scalarFuncs.sigmoid(preSigmoid[j]);
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
                    dcWRTdaCurNeuron *= scalarFuncs.daWRTdz_sigmoid(z[layer + 1][nextLayerNeuron]);
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
    const vector<vector<float>>& dcWRTda
) {
    const int L = l - 1;

    vector<Matrix<float>> dcWRTdw = weights; // initializing the future-output as the weights because a will have the corect dimensions

    // note: because of implementaiton weights[L - 1] in this implementation = w^(L) in the 3B1B videos

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 1; layer--) {
        for (int i = 0; i < weights[layer - 1].cols; i++) {          // i = index of previous layer's neurons
            for (int j = 0; j < weights[layer - 1].rows; j++) {      // j = index of cur layer's neurons
                // start: neuron activation derivative (dc/da) of cur layer
                dcWRTdw[layer - 1][j][i] = dcWRTda[layer][j];
                // multiply: da/dz for cur layer
                dcWRTdw[layer - 1][j][i] *= scalarFuncs.daWRTdz_sigmoid(z[layer][j]);
                // multiply: dz/dw
                dcWRTdw[layer - 1][j][i] *= scalarFuncs.dzWRTdw(a[layer][j]);
            }
        }
    }
    return dcWRTdw;
}

vector<vector<float>> NeuralNet::calcBiasesGradient(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const vector<vector<float>>& dcWRTda
) {
    const int L = l - 1;

    vector<vector<float>> dcWRTdb = biases; // initializing the future-output as the biases because will have the corect dimensions

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 1; layer--) {
        for (int neuron = 0; neuron < biases[layer - 1].size(); neuron++) {
            // start: neuron activation derivative (dc/da) of cur layer
            dcWRTdb[layer - 1][neuron] = dcWRTda[layer][neuron];
            // multiply: da/dz for cur layer
            dcWRTdb[layer - 1][neuron] *= scalarFuncs.daWRTdz_sigmoid(z[layer][neuron]);
            // multiply: dz/db (this line can be removed since dz/db = 1)
            dcWRTdb[layer - 1][neuron] *= scalarFuncs.dzWRTdb();
        }
    }
    return dcWRTdb;
}

void NeuralNet::makeTrainingStep(
    const vector<vector<float>>& a,
    const vector<vector<float>>& z,
    const int requestedOutputIdx
) {
    vector<vector<float>> dcWRTda = calcNeuronActivationsGradient(a, z, requestedOutputIdx);
    vector<Matrix<float>> weightsGradient = calcWeightsGradient(a, z, dcWRTda);
    vector<vector<float>> biasesGradient = calcBiasesGradient(a, z, dcWRTda);

    // print out the cost function (for interest's sake)
    float cost = 0;
    for (size_t i = 0; i < a[l - 1].size(); i++) {
        float curCost = a[l - 1][i];
        if(i == requestedOutputIdx) {
            curCost -= 1;
        }
        cost += curCost * curCost;
    }

    cout << cost << endl;

    // nudge weights
    for (size_t layer = 0; layer < weights.size(); layer++) {
        for (size_t i = 0; i < weights[layer].rows; i++) {        // rows
            for (size_t j = 0; j < weights[layer].cols; j++) {    // cols
                // nudge in direction of negative gradient:
                this->weights[layer][i][j] -= weightsGradient[layer][i][j];
            }
        }
    }

    // nudge biases
    for (size_t layer = 0; layer < biases.size(); layer++) {
        for (size_t neuron = 0; neuron < biases[layer].size(); neuron++) {
            // nudge in direction of negative gradient:
            biases[layer][neuron] -= biasesGradient[layer][neuron];
        }
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