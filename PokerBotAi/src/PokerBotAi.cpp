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
    
    vector<int> neuronsInLayer = { 2, 2, 1 };
    NeuralNet nn2(neuronsInLayer);
    nn2.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");
    vector<float> errors;
    for (size_t batchSize = 1; batchSize < 11; batchSize++) {
        NeuralNet nn1("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout1.json");
        nn1.train(trainingInputs, trainingExpectedOutputs, batchSize, 0.5);
        float inaccuracy = nn1.test(testingInputs, testingExpectedOutputs);
        cout << inaccuracy << endl;
        errors.push_back(inaccuracy);
        nn1.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout2.json");
    }
    cout << endl << endl << endl;
    for(auto err : errors) {
        cout << err << endl;
    }
    return 0;
}
