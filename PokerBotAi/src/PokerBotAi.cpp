// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "NeuralNet/NeuralNet.h"
#include "MNIST/MNIST.h"
#include <vector>
#include <string>
#include <iomanip>

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

    NeuralNet nn({4, 5, 3});

    nn.writeToFile("C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\poker-bot\\PokerBotAi\\testout.json");

    // vector<float> in = {0.1, 0.2, 0.3, 0.4};

    // nn.calcIntermediateValues(in);

    // for(int i = 0; i < 20; i++) {
    //     nn.makeTrainingStep(1);
    //     nn.calcIntermediateValues(in);
    // }
    return 0;
}
