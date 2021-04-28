#include "NeuralNet.h"
#include "../Math/VectorUtils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

void getLayerDataFromJsonStr(
    const string& jsonStr,
    Matrix<float>& weights,
    vector<float>& biases
) {
    smatch matches;

    // weights
    regex weightsDataPattern("*\"weights\" *:[ \\n]*\\[([0-9\\n \":[,\\].-]*)]*)");
    regex_search(jsonStr, matches, weightsDataPattern);
    string weightsDataString = matches[0].str();
    regex weightsDataRowsPattern("\[.*]");
    regex_search(weightsDataString, matches, weightsDataRowsPattern);
    weights.numRows = matches.size();

    vector<vector<float>> m;

    for (size_t i = 0; i < matches.size(); ++i) {
        string rowStr = matches[i].str();
        regex elementsPattern("[-.0-9]+");
        smatch elements;
        regex_search(rowStr, elements, elementsPattern);

        if (i == 0) {
            weights.numCols = elements.size();
        }

        vector<float> row;
        for (auto elem : elements) {
            row.push_back(stof(elem.str()));
        }
        weights.addRow(row);
    }

    // biases
    regex biasesDataPattern("*\"biases\" *:[ \\n]*\\[([0-9\\n ,.-]*)]*)");
    regex_search(jsonStr, matches, biasesDataPattern);
    string biasesDataString = matches[0].str();
    regex biasesElementsPattern("[-.0-9]+");
    regex_search(weightsDataString, matches, biasesElementsPattern);
    for (auto bias : matches) {
        biases.push_back(stof(bias.str()));
    }
}

NeuralNet::NeuralNet(const string& filePath) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw runtime_error("file (" + filePath + ") does not exists");
    }

    smatch matches;
    string file;
    string line;
    while (getline(inFile, line)) {
        file += line + '\n';
    }

    // layers
    regex layersPattern(" *\"layers\" *: *([\\w-]*)");
    regex_search(file, matches, layersPattern);
    for(auto a : matches) {
        cout << a.str() << endl;
    }
    layers = stoi(matches[1].str());

    // scalarFuncsCompressFactor
    regex scalarFuncsCompressFactorPattern(" *\"scalarFuncsCompressFactor\" *: *([\\w\\.-]*)");
    regex_search(file, matches, scalarFuncsCompressFactorPattern);
    scalarFuncsCompressFactor = stof(matches[1].str());

    // layer data
    regex layerDataPattern(" *\"layer_[0-9]+\" *:[ \n]*{([\\w\\n \":[,\\].-]*)}");
    regex_search(file, matches, layerDataPattern);
    for (size_t i = 0; i < matches.size() - 1; ++i) {   // i < matches.size() - 1 because the last layer doesn't have any relevant data
        Matrix<float> layerWeights;
        vector<float> layerBiases;
        getLayerDataFromJsonStr(matches[i], layerWeights, layerBiases);
        weights.push_back(layerWeights);
        biases.push_back(layerBiases);
    }
}

NeuralNet::NeuralNet(vector<int> neuronsInLayer, float scalarFuncsCompressFactor) :
    layers(neuronsInLayer.size()), scalarFuncsCompressFactor(scalarFuncsCompressFactor) {
    // TODO: param checking

    // initialize all weights and biases to random values
    for (size_t i = 0; i < layers - 1; i++) {
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
    for (size_t i = 0; i < layers - 1; i++) {
        // next data = weights matrix * input values + biases
        intermediateData = weights[i](intermediateData) + biases[i];
        for (size_t j = 0; j < intermediateData.size(); j++) {
            intermediateData[j] = sigmoid(intermediateData[j]);
        }
    }
    return intermediateData;
}

int NeuralNet::getNeuronsInLayer(const int layer) const {
    if (layer == layers - 1) {
        return weights[layer - 1].numRows;
    }
    else {
        return weights[layer].numCols;
    }

}


void NeuralNet::calcIntermediateValues(const vector<float>& in) {
    // NOTE: we dont apply biases or sigmoid to input layer
    data.neuronActivations = { in };
    data.neuronActivationsPreSigmoid = { in };

    // temp intermediate data vectors which will update for each layer
    vector<float> preSigmoid;
    vector<float> postSigmoid = in;

    // for each layer...
    for (size_t i = 0; i < layers - 1; i++) {
        // next data = (weights matrix) * (input values vector) + (biases vector)
        preSigmoid = (weights[i] * postSigmoid) + biases[i];
        data.neuronActivationsPreSigmoid.push_back(preSigmoid);

        postSigmoid = preSigmoid;

        // for each neuron...
        for (size_t j = 0; j < preSigmoid.size(); j++) {
            // apply the sigmoid
            postSigmoid[j] = sigmoid(preSigmoid[j]);
        }
        data.neuronActivations.push_back(postSigmoid);
    }
}


// to calc delta cost wrt the activations of every neuron in the network
// TODO: generalize this. "requestedOutputIdx" should be a vector of requested output values
void NeuralNet::calcCostsWRTActivationsGradient(const int requestedOutputIdx) {
    const int L = layers - 1;

    data.costsWRTActivationsGradient = data.neuronActivations; // initializing the future-output as the intermediate activations because a will have the corect dimensions

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 0; layer--) {
        for (size_t neuron = 0; neuron < data.costsWRTActivationsGradient[layer].size(); neuron++) {
            if (layer == L) { // for the end layer, the solutions are trivial
                float y = 0.0;
                if (neuron == requestedOutputIdx) {
                    y = 1.0;
                }
                data.costsWRTActivationsGradient[layer][neuron] = dcWRTda(data.neuronActivations[layer][neuron], y);
            }
            else {  // otherwise, changes in activation of the curent neuron has effects on each following neuron
                data.costsWRTActivationsGradient[layer][neuron] = 0.0;
                for (size_t nextLayerNeuron = 0; nextLayerNeuron < data.costsWRTActivationsGradient[layer + 1].size(); nextLayerNeuron++) {
                    // start with: dz(L+1)/da(L)
                    float costsWRTActivationsGradientCurNeuron = dzWRTda_previous(weights[layer][nextLayerNeuron][neuron]);
                    // multiply: da(L+1)/dz(L+1)
                    costsWRTActivationsGradientCurNeuron *= daWRTdz_sigmoid(data.neuronActivationsPreSigmoid[layer + 1][nextLayerNeuron]);
                    // multiply: dc(L+1)/da(L+1)
                    costsWRTActivationsGradientCurNeuron *= data.costsWRTActivationsGradient[layer + 1][nextLayerNeuron];  // starts to get recursive here!

                    data.costsWRTActivationsGradient[layer][neuron] += costsWRTActivationsGradientCurNeuron;
                }
            }
        }
    }
}

void NeuralNet::calcCostsWRTWeightsGradient() {
    const int L = layers - 1;
    data.costsWRTWeightsGradient = weights; // initializing the future-output as the weights because a will have the corect dimensions

    // note: because of implementaiton weights[L - 1] in this implementation = w^(L) in the 3B1B videos

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 1; layer--) {
        for (int i = 0; i < weights[layer - 1].numCols; i++) {          // i = index of previous layer's neurons
            for (int j = 0; j < weights[layer - 1].numRows; j++) {      // j = index of cur layer's neurons
                // start: neuron activation derivative (dc/da) of cur layer
                data.costsWRTWeightsGradient[layer - 1][j][i] = data.costsWRTActivationsGradient[layer][j];
                // multiply: da/dz for cur layer
                data.costsWRTWeightsGradient[layer - 1][j][i] *= daWRTdz_sigmoid(data.neuronActivationsPreSigmoid[layer][j]);
                // multiply: dz/dw
                data.costsWRTWeightsGradient[layer - 1][j][i] *= dzWRTdw(data.neuronActivations[layer][j]);
            }
        }
    }
}

void NeuralNet::calcCostsWRTBiasesGradient() {
    const int L = layers - 1;

    data.costsWRTBiasesGradient = biases; // initializing the future-output as the biases because will have the corect dimensions

    // start at front (output layer) and propogate backwards
    for (int layer = L; layer >= 1; layer--) {
        for (int neuron = 0; neuron < biases[layer - 1].size(); neuron++) {
            // start: neuron activation derivative (dc/da) of cur layer
            data.costsWRTBiasesGradient[layer - 1][neuron] = data.costsWRTActivationsGradient[layer][neuron];
            // multiply: da/dz for cur layer
            data.costsWRTBiasesGradient[layer - 1][neuron] *= daWRTdz_sigmoid(data.neuronActivationsPreSigmoid[layer][neuron]);
            // multiply: dz/db (this line can be removed since dz/db = 1)
            data.costsWRTBiasesGradient[layer - 1][neuron] *= dzWRTdb();
        }
    }
}

void NeuralNet::makeTrainingStep(
    const int requestedOutputIdx
) {
    calcCostsWRTActivationsGradient(requestedOutputIdx);
    calcCostsWRTWeightsGradient();
    calcCostsWRTBiasesGradient();

    // print out the cost function (for interest's sake)
    float cost = 0;
    for (size_t i = 0; i < data.neuronActivations[layers - 1].size(); i++) {
        float curCost = data.neuronActivations[layers - 1][i];
        if (i == requestedOutputIdx) {
            curCost -= 1;
        }
        cost += curCost * curCost;
    }
    cout << cost << endl;

    // nudge weights
    for (size_t layer = 0; layer < weights.size(); layer++) {
        for (size_t i = 0; i < weights[layer].numRows; i++) {        // rows
            for (size_t j = 0; j < weights[layer].numCols; j++) {    // cols
                // nudge in direction of negative gradient:
                this->weights[layer][i][j] -= data.costsWRTWeightsGradient[layer][i][j];
            }
        }
    }

    // nudge biases
    for (size_t layer = 0; layer < biases.size(); layer++) {
        for (size_t neuron = 0; neuron < biases[layer].size(); neuron++) {
            // nudge in direction of negative gradient:
            biases[layer][neuron] -= data.costsWRTBiasesGradient[layer][neuron];
        }
    }
}

void NeuralNet::calcIntermediateData(const vector<float>& in, const int requestedOutputIdx) {
    calcIntermediateValues(in);
    calcCostsWRTActivationsGradient(requestedOutputIdx);
    calcCostsWRTWeightsGradient();
    calcCostsWRTBiasesGradient();
}

void NeuralNet::calcIntermediateBatchData(
    const vector<vector<float>>& inBatch,
    const vector<int> requestedOutputIdxBatch
) {
    for (size_t i = 0; i < inBatch.size(); i++) {
        calcIntermediateData(inBatch[i], requestedOutputIdxBatch[i]);
        intermediateDataBatch.push_back(data);
    }
}

void NeuralNet::train(const vector<vector<float>>& trainingSet, const vector<int>& expectedOutNeuron, const int batchsize) {
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

ostream& operator << (ostream& out, const NeuralNet& nn) {
    out << "{\n";
    out << "    \"layers\" : " << nn.layers << "," << endl;
    out << "    \"scalarFuncsCompressFactor\" : " << nn.scalarFuncsCompressFactor << "," << endl;
    // for each layer
    for (int layer = 0; layer < nn.layers; ++layer) {
        out << "    \"layer_" << layer << "\" : {" << endl;
        out << "        \"layer\" : " << layer << "," << endl;
        out << "        \"neurons\" : " << nn.getNeuronsInLayer(layer) << "," << endl;

        // dealing with weights
        out << "        \"weights\" : " << endl;
        out << "        [" << endl;
        Matrix<float> weights = nn.weights[layer];
        for (size_t rowIdx = 0; rowIdx < weights.numRows; rowIdx++) {
            out << "            [";
            for (size_t colIdx = 0; colIdx < weights.numCols; colIdx++) {
                out << weights[rowIdx][colIdx];
                if (colIdx != weights.numCols - 1) {
                    out << ", ";
                }
            }
            out << "]";
            if (rowIdx != weights.numRows - 1) {
                out << "," << endl;
            }
            else {
                out << endl;
            }
        }
        out << "        ]," << endl;

        // dealing with biases

        out << "        \"biases\" : [";
        vector<float> biases = nn.biases[layer];
        for (size_t i = 0; i < biases.size(); i++) {
            out << biases[i];
            if (i != biases.size() - 1) {
                out << ", ";
            }
        }
        out << "]" << endl;

        out << "    }";

        if (layer != nn.layers - 1) {
            out << "," << endl;
        }
        else {
            out << endl;
        }
    }
    out << "}";
    return out;
}

void NeuralNet::writeToFile(const string& pathToFile) {
    ofstream outFile(pathToFile);
    if (outFile.is_open()) {
        outFile << *this;
        outFile.close();
    }
    else {
        cout << "Unable to open file";
    }
}

