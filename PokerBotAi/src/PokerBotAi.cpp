﻿// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "NeuralNet/NeuralNet.h"
#include "MNIST/MNIST.h"
#include <vector>
#include <string>
#include <iomanip>

#include "testing.h"

using namespace std;

int main()
{
    // int numImgs = 0;
    // int numLabels = 0;
    // int imgSize = 0;
    // unsigned char** imgs = read_mnist_images("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\NNData\\train\\train-images.idx3-ubyte", numImgs, imgSize);
    // unsigned char* labels = read_mnist_labels("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\NNData\\train\\train-labels.idx1-ubyte", numLabels);
    // if(numImgs != numLabels) {
    //     cout << "numImgs != numLabels" << endl;
    // } else {
    //     for(int i = 0; i < 10; ++i) {
    //         int label = labels[i];
    //         cout << label << endl;
    //         printImg(cout, imgs, imgSize, i);
    //     }
    // }

    // return 0;

    vector<int> neuronsInLayer = { 2, 1 };
    NeuralNet nn1(neuronsInLayer);
    // nn1.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");

    // turn this into a proper training set
    vector<vector<float>> trainingSet;
    for(auto inputActivationSet : activatedInputs) {
        vector<float> input(20);
        input[inputActivationSet[0]] = 1.0;
        input[inputActivationSet[1]] = 1.0;
        trainingSet.push_back(input);
    }

    vector<vector<float>> trainingSetOuts;
    for(auto outputActivations : expectedOuts) {
        vector<float> outs(20);
        outs[outputActivations] = 1.0;
        trainingSetOuts.push_back(outs);
    }

    nn1.train(trainingSet, trainingSetOuts, 4);

    nn1.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout2.json");

    // vector<float> in = {0.1, 0.2, 0.3, 0.4};

    // nn.calcIntermediateValues(in);

    // for(int i = 0; i < 20; i++) {
    //     nn.makeTrainingStep(1);
    //     nn.calcIntermediateValues(in);
    // }
    return 0;
}
