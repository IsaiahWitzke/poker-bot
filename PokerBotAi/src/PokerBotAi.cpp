// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "NeuralNet/NeuralNet.h"
#include "MNIST/MNIST.h"
#include <vector>
#include <string>
#include <iomanip>
#include "Math/VectorUtils.h"
#include "Testing/TestData.h"
#include <stdlib.h>

#include "testing.h"

using namespace std;

void doMnist() {
    int width = 10;

    vector<vector<float>> trainingInputsNew;
    vector<vector<float>> trainingExpectedOutputsNew;
    for(size_t i = 0; i < 20000; i++) {
        vector<float> trainingIn(width);
        vector<float> trainingExpectedOut(width);
        int digit1 = rand() % width;
        // int digit2 = rand() % 10;
        trainingIn[i % 10] = 1;
        // trainingIn[digit2 + 10] = 1;
        // int sum = digit1 + digit2;
        trainingExpectedOut[i % 10] = 1;
        trainingInputsNew.push_back(trainingIn);
        trainingExpectedOutputsNew.push_back(trainingExpectedOut);
    }

    vector<vector<float>> testingInputsNew;
    vector<vector<float>> testingExpectedOutputsNew;
    for(size_t i = 0; i < 50; i++) {
        vector<float> testingIn(width);
        vector<float> testingExpectedOut(width);
        // int digit1 = (int)(10 * testingInputs[i][0]);
        // int digit2 = (int)(10 * testingInputs[i][1]);
        int digit1 = rand() % width;
        int digit2 = rand() % 10;
        testingIn[digit1] = 1;
        // testingIn[digit2 + 10] = 1;
        // int sum = digit1 + digit2;
        testingExpectedOut[digit1] = 1;
        testingInputsNew.push_back(testingIn);
        testingExpectedOutputsNew.push_back(testingExpectedOut);
    }

    vector<int> neuronsInLayer = { width, width, width };
    // NeuralNet nn2("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout2.json");
    NeuralNet nn2(neuronsInLayer);
    nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");
    nn2.train(trainingInputsNew, trainingExpectedOutputsNew, 10, 0.1, testingInputsNew, testingExpectedOutputsNew);
    nn2.test(testingInputsNew, testingExpectedOutputsNew);
    Matrix<float> testMatrix = nn2.weights[0] * nn2.weights[1];
    // float inaccuracy = nn2.test(testingInputsNew, testingExpectedOutputsNew);
    // cout << inaccuracy << endl;
    
    // nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout2.json");
    
    // vector<int> neuronsInLayer = { 2, 2, 2 };
    // NeuralNet nn2(neuronsInLayer);
    // nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");
    // nn2.train(trainingInputs, trainingExpectedOutputs, 10, 0.5, testingInputs, testingExpectedOutputs);
    // float inaccuracy = nn2.test(testingInputs, testingExpectedOutputs);
    // cout << inaccuracy << endl;
    
    nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout2.json");
}

void doMnistAttempt1() {
    int numImgs = 0;
    int numLabels = 0;
    int imgSize = 0;
    unsigned char** imgs = read_mnist_images("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\NNData\\train\\train-images.idx3-ubyte", numImgs, imgSize);
    unsigned char* labels = read_mnist_labels("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\NNData\\train\\train-labels.idx1-ubyte", numLabels);
    vector<vector<float>> imagesVectorTraining = charArrImgsToVectors(imgs, imgSize, 0, 10000);
    vector<vector<float>> labelsVectorTraining = charArrLabelsToVectors(labels, 0, 10000);

    vector<vector<float>> imagesVectorTesting = charArrImgsToVectors(imgs, imgSize, 10001, 10050);
    vector<vector<float>> labelsVectorTesting = charArrLabelsToVectors(labels, 10001, 10050);
    
    vector<int> neuronsInLayer = { 784, 500, 300, 100, 10 };

    NeuralNet nn2(neuronsInLayer);
    nn2.train(imagesVectorTraining, labelsVectorTraining, 100, 0.1, imagesVectorTesting, labelsVectorTesting);
    nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");
    

    float inaccuracy = nn2.test(imagesVectorTesting, labelsVectorTesting);
    cout << "inaccuracy: " << inaccuracy << endl;

    // if(numImgs != numLabels) {
    //     cout << "numImgs != numLabels" << endl;
    // } else {
    //     for(int i = 0; i < 10; ++i) {
    //         int label = labels[i];
    //         cout << label << endl;
    //         printImg(cout, imgs, imgSize, i);
    //     }
    // }
}

void doLinearRegressionTesting() {
	TestData testData(2, 1, 100, 5, TestData::LINEAR);

    vector<int> neuronsInLayer = { 2, 1 };
    // NeuralNet nn2("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout_linear.json");
    NeuralNet nn2(neuronsInLayer);
    nn2.writeToFile("data/linear-before-training.json");
    nn2.train(
		testData.trainingInputs,
		testData.trainingExpectedOuts,
		10,
		0.001,
		testData.testingInputs,
		testData.testingExpectedOuts
	);
    nn2.writeToFile("linear-after-training.json");
    nn2.test(testData.testingInputs, testData.testingExpectedOuts);
}

int main() {
	doLinearRegressionTesting();
    return 0;
}
