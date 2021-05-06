#include "NeuralNet.h"
#include "../Math/VectorUtils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <stdlib.h>

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
        srand(1);
        Matrix<float> weights(neuronsInLayer[i + 1], neuronsInLayer[i], 0.0);     // each row reps all the links from input neuron to next neurons
        weights.randomize();
        this->weights.push_back(weights);
        vector<float> biases(neuronsInLayer[i + 1], 0.0);
        randomizeVector(biases);
        this->biases.push_back(biases);
    }
}

vector<float> NeuralNet::operator()(const vector<float>& in) {
    calcIntermediateValues(in);
    return data.neuronActivations[layers - 1];
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
    data.neuronActivationsPreActivation = { in };

    // temp intermediate data vectors which will update for each layer
    vector<float> preActivationValues;
    vector<float> previousLayerActivations = in;

    // for each layer... ("forward pass")
    for (size_t i = 0; i < layers - 1; i++) {
        // (this layer's data before activation function)
        //            = (weights matrix) * (input values or previous layer vector) + (biases vector)
        // z[i] = W^[i] a^[i-1] + b^[i]
        data.neuronActivationsPreActivation.push_back(
            (weights[i] * data.neuronActivations[i]) + biases[i]
        );


        // Non-linear activation function time!
        // a^[i] = ReLU(W^[i] a^[i-1] + b^[i]) = ReLU(z^[i])
        // OR....
        // a^[i] = sigmoid(W^[i] a^[i-1] + b^[i]) = sigmoid(z^[i])
        // NOTE: we don't apply the activation function to the final layer
        if (i != layers - 2) {
            data.neuronActivations.push_back(
                relu(data.neuronActivationsPreActivation[i + 1])
            );
        }
        else {
            data.neuronActivations.push_back(
                data.neuronActivationsPreActivation[i + 1]
            );
        }
    }
}

void NeuralNet::calcGradients(const vector<float>& requestedOutput) {
    const int L = layers - 2;

    // initializing the future-output as the biases/weights because each will have the corect dimensions
    data.costsWRTBiasesGradient = biases;
    data.costsWRTWeightsGradient = weights;

    // see the notation used in the stanford course notes for "delata"
    // From what I understand, is that the delata represents dC/dz
    vector<vector<float>> deltas(layers - 1);

    // start at back (output layer) and propogate towards the back
    for (int layer = L; layer >= 0; layer--) {
        if (layer == L) {
            deltas[layer] = (data.neuronActivations[layer + 1] - requestedOutput);
        }
        else {
            deltas[layer] = elementWiseProduct(
                weights[layer + 1].transpose() * deltas[layer + 1],
                daWRTdz_relu(data.neuronActivationsPreActivation[layer + 1])
            );
        }

        vector<float> test = data.neuronActivations[layer];
        Matrix<float> previousLayerActivationTranspose(test, true);
        Matrix<float> deltaCurLayerMatrix(deltas[layer]);

        data.costsWRTWeightsGradient[layer] = deltaCurLayerMatrix * previousLayerActivationTranspose;

        data.costsWRTBiasesGradient[layer] = deltas[layer];
    }
}

void NeuralNet::makeTrainingStep(float stepScalingFactor) {

    // first, calculate the average gradients (initialize with the same sizes, just all elements are 0)
    vector<Matrix<float>> averageCostsWRTWeightsGradient = data.costsWRTWeightsGradient * 0;
    vector<vector<float>> averageCostsWRTBiasesGradient = data.costsWRTBiasesGradient * 0;

    for (size_t i = 0; i < intermediateDataBatch.size(); ++i) {
        averageCostsWRTWeightsGradient = averageCostsWRTWeightsGradient + intermediateDataBatch[i].costsWRTWeightsGradient;
        averageCostsWRTBiasesGradient = averageCostsWRTBiasesGradient + intermediateDataBatch[i].costsWRTBiasesGradient;
    }

    float weightedAverageFactor = 1.0 / intermediateDataBatch.size();
    averageCostsWRTWeightsGradient = averageCostsWRTWeightsGradient * weightedAverageFactor;
    averageCostsWRTBiasesGradient = averageCostsWRTBiasesGradient * weightedAverageFactor;

    // nudge in direction of negative gradient:
    this->weights = weights + (averageCostsWRTWeightsGradient * -1.0 * stepScalingFactor);
    this->biases = biases + (averageCostsWRTBiasesGradient * -1.0 * stepScalingFactor);
}

void NeuralNet::calcIntermediateData(
    const vector<float>& in,
    const vector<float>& requestedOutput
) {
    calcIntermediateValues(in);
    calcGradients(requestedOutput);
}

void NeuralNet::calcIntermediateBatchData(
    const vector<vector<float>>& inputsBatch,
    const vector<vector<float>>& expectedOutputsBatch
) {
    intermediateDataBatch.clear();
    for (size_t i = 0; i < inputsBatch.size(); i++) {
        calcIntermediateData(inputsBatch[i], expectedOutputsBatch[i]);
        intermediateDataBatch.push_back(data);
    }
}

void NeuralNet::train(
    const vector<vector<float>>& trainingSetInputs,
    const vector<vector<float>>& trainingSetExpectedOuts,
    const int batchsize,
    const float stepSize
) {
    vector<vector<float>> inputsBatch;
    vector<vector<float>> expectedOutputsBatch;
    const size_t trainingSetSize = trainingSetInputs.size();

    cout << "training... training set size = " << trainingSetSize << ", batch size = " << batchsize << ", step size = " << stepSize << endl;
    for (int i = 0; i < trainingSetSize; ++i) {
        if ((i % batchsize == 0 && i != 0) || i == trainingSetSize - 1) {
            // cout << "On set entry " << i << " of " << trainingSetSize << endl;
            calcIntermediateBatchData(inputsBatch, expectedOutputsBatch);
            makeTrainingStep(stepSize);
            inputsBatch.clear();
            expectedOutputsBatch.clear();
        }
        inputsBatch.push_back(trainingSetInputs[i]);
        expectedOutputsBatch.push_back(trainingSetExpectedOuts[i]);

    }
}

float NeuralNet::test(
    const vector<vector<float>>& testingInputs,
    const vector<vector<float>>& trainingExpectedOuts
) {
    float intermediateMAPE = 0.0;
    for (size_t i = 0; i < testingInputs.size(); i++) {
        vector<float> actualOut = (*this)(testingInputs[i]);    // apply the neural net to the testing input using the overloaded () operator
        // error = (actual - expected) / (actual)
        intermediateMAPE += abs(norm(actualOut - trainingExpectedOuts[i]) / norm(actualOut));
    }
    return intermediateMAPE / testingInputs.size();
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

