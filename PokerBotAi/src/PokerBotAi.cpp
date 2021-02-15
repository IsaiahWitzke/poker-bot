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
    NeuralNet nn({4, 5, 3});

    vector<vector<float>> a = {{}};
    vector<vector<float>> z = {{}};
    vector<float> in = {1.0, 2.0, 3.0, 4.0};

    nn.calcIntermediateValues(in, a, z);
    vector<float> res = nn(in);

    cout << "Hello world." << endl;
    return 0;
}
