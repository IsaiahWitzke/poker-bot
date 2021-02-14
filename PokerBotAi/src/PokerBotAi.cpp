// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "Math/ScalarFuncs.h"
#include "NeuralNet/NeuralNet.h"
#include <vector>

using namespace std;

int main()
{
    NeuralNet nn({4, 5, 3});
    vector<float> res = nn({1.0, 2.0, 3.0, 4.0});

    cout << "Hello world." << endl;
    return 0;
}
