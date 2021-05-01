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
    smatch match;

    regex weightsDataPattern("\"weights\" *:([ \\n\\[\\]0-9-.,]*)");
    regex_search(jsonStr, match, weightsDataPattern);
    string weightsDataString = match[0].str();
    regex weightsDataRowsPattern("\\[.*\\]");
    smatch rowmatch;

    weights.numRows = 0;
    while (regex_search(weightsDataString, rowmatch, weightsDataRowsPattern)) {
        weights.numRows++;
        string rowStr = rowmatch[0].str();
        regex elementsPattern("[-.0-9]+");
        smatch element;

        weights.numCols = 0;
        vector<float> row;
        while (regex_search(rowStr, element, elementsPattern)) {
            weights.numCols++;
            row.push_back(stof(element[0].str()));
            rowStr = element.suffix();
        }
        weights.addRow(row);
        weightsDataString = rowmatch.suffix();
    }

    // biases
    regex biasesDataPattern(" *\"biases\" *:[ \\n]*\\[[0-9\\n ,.-]*\\]");
    regex_search(jsonStr, match, biasesDataPattern);
    string biasesDataString = match[0].str();
    regex biasesElementsPattern("[-.0-9]+");
    while (regex_search(biasesDataString, match, biasesElementsPattern)) {
        biases.push_back(stof(match[0].str()));
        biasesDataString = match.suffix();
    }
}

NeuralNet::NeuralNet(const string& filePath) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw runtime_error("file (" + filePath + ") does not exists");
    }

    smatch match;
    string file;
    string line;
    while (getline(inFile, line)) {
        file += line + '\n';
    }

    // layers
    regex layersPattern(" *\"layers\" *: *([\\w-]*)");
    regex_search(file, match, layersPattern);
    for (auto a : match) {
        cout << a.str() << endl;
    }
    layers = stoi(match[1].str());

    // scalarFuncsCompressFactor
    regex scalarFuncsCompressFactorPattern(" *\"scalarFuncsCompressFactor\" *: *([\\w\\.-]*)");
    regex_search(file, match, scalarFuncsCompressFactorPattern);
    scalarFuncsCompressFactor = stof(match[1].str());

    // layer data
    string layerDataPatternStr = " *\"layer_[0-9]+\" *:[ \\n]*\\{([\\w\n \":[,\\].-]*)\\}";
    regex layerDataPattern(layerDataPatternStr);
    while (regex_search(file, match, layerDataPattern)) {
        Matrix<float> layerWeights;
        vector<float> layerBiases;
        string layerDataStr = match[1].str();
        getLayerDataFromJsonStr(layerDataStr, layerWeights, layerBiases);
        // last element (maybe) is going to be empty, so skip it
        if (layerWeights.numRows == 0) {
            break;
        }
        weights.push_back(layerWeights);
        biases.push_back(layerBiases);
        file = match.suffix();
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

void NeuralNet::makeTrainingStep(float stepScalingFactor) {

    // first, calculate the average gradients (initialize with the same sizes, just all elements are 0)
    vector<Matrix<float>> averageCostsWRTWeightsGradient = data.costsWRTWeightsGradient * 0;
    vector<vector<float>> averageCostsWRTBiasesGradient = data.costsWRTBiasesGradient * 0;

    float weightedAverageFactor = 1.0 / intermediateDataBatch.size();
    for (size_t i = 0; i < intermediateDataBatch.size(); ++i) {
        averageCostsWRTWeightsGradient = averageCostsWRTWeightsGradient + (intermediateDataBatch[i].costsWRTWeightsGradient * weightedAverageFactor);
        averageCostsWRTBiasesGradient = averageCostsWRTBiasesGradient + (intermediateDataBatch[i].costsWRTBiasesGradient * weightedAverageFactor);
    }

    // nudge in direction of negative gradient:
    this->weights = weights + (averageCostsWRTWeightsGradient * -1.0 * stepScalingFactor);
    this->biases = biases + (averageCostsWRTBiasesGradient * -1.0 * stepScalingFactor);
}

void NeuralNet::calcIntermediateData(const vector<float>& in, const int requestedOutputIdx) {
    calcIntermediateValues(in);
    calcCostsWRTActivationsGradient(requestedOutputIdx);
    calcCostsWRTWeightsGradient();
    calcCostsWRTBiasesGradient();
}

void NeuralNet::calcIntermediateBatchData(
    const vector<vector<float>>& inputsBatch,
    const vector<int> expectedOutputsBatch
) {
    intermediateDataBatch.clear();
    for (size_t i = 0; i < inputsBatch.size(); i++) {
        calcIntermediateData(inputsBatch[i], expectedOutputsBatch[i]);
        intermediateDataBatch.push_back(data);
    }
}

void NeuralNet::train(
    const vector<vector<float>>& trainingSet,
    const vector<int>& expectedOuts,
    const int batchsize
) {
    vector<vector<float>> inputsBatch;
    vector<int> expectedOutputsBatch;
    cout << "training..." << endl;
    for (int i = 0; i < trainingSet.size(); ++i) {
        if ((i % batchsize == 0 && i != 0) || i == trainingSet.size() - 1) {
            cout << "On set entry " << i << " of " << trainingSet.size() << endl;
            calcIntermediateBatchData(inputsBatch, expectedOutputsBatch);
            makeTrainingStep(3.0);
            inputsBatch.clear();
            expectedOutputsBatch.clear();
        }
        inputsBatch.push_back(trainingSet[i]);
        expectedOutputsBatch.push_back(expectedOuts[i]);

    }
}

ostream& operator << (ostream& out, const NeuralNet& nn) {
    out << "{\n";
    out << "    \"layers\" : " << nn.layers << "," << endl;
    out << "    \"scalarFuncsCompressFactor\" : " << nn.scalarFuncsCompressFactor << "," << endl;
    // for each layer
    for (int layer = 0; layer < nn.layers; ++layer) {
        out << "    \"layer_" << layer << "\" : {" << endl;
        out << "        \"layer\" : " << layer << "," << endl;
        out << "        \"neurons\" : " << nn.getNeuronsInLayer(layer);

        // dealing with final layer... no weights/biases should really be stored here
        if (layer == nn.layers - 1) {

            out << endl << "    }" << endl;
            break;
        }
        else {
            out << "," << endl;
        }

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

