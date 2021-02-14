// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "Math/ScalarFuncs.h"
#include "NeuralNet/NeuralNet.h"
#include "MNIST/MNIST.h"
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

int main()
{
    NeuralNet nn({4, 5, 3});
    vector<float> res = nn({1.0, 2.0, 3.0, 4.0});

    int imgSize = 0;
    int numImages = 0;
    string pathToImages = "C:\\Users\\witzk\\OneDrive\\Desktop\\Projects\\NNData\\train\\train-images.idx3-ubyte";
    unsigned char** images = read_mnist_images(pathToImages, numImages, imgSize);

    printImg(cout, images, imgSize, 4);
    
    cout << "Hello world." << endl;

    delete images;
    return 0;
}
