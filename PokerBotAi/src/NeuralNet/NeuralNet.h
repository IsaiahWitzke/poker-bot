#pragma once

#include "../Math/Matrix.h"
#include <vector>
#include <iostream>

using namespace std;

class NeuralNet {
private:
    vector<Matrix<float>> weights;
    vector<vector<float>> biases;

public:

    /**
     * @brief Construct a new Neural Net object from an input (file) stream
     *
     * @param model the steam from which the pretrained net's data comes from
     */
    NeuralNet(istream in);

    /**
     * @brief Construct a new Neural Net object
     *
     * @param neuronsInLayer Represents the number of neurons in each layer.
     * Needs to have at least 2 elems (input layer and output layer)
     * First elem in neuronsInLayer is the input layer, last is output.
     */
    NeuralNet(vector<int> neuronsInLayer);


    /**
     * @brief to apply the neural net onto an input
     *
     * @param in
     * @return vector<float>
     */
    vector<float> operator()(const vector<float> in);
};