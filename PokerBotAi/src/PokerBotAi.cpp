// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include "Math/Sigmoid.h"
#include <vector>

using namespace std;

int main()
{
    Matrix<int> m1(2, 3, 1);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[0][2] = 3;
    m1[1][0] = 4;
    m1[1][1] = 5;
    m1[1][2] = 6;
    vector<int> v1 = { 7, 9, 11 };
    vector<int> v2 = m1(v1);

    cout << sigmoid(0.1) << " " <<
        sigmoid(0.2) << " " <<
        sigmoid(0.3) << " " <<
        sigmoid(0.4) << " " <<
        sigmoid(0.5) << " " <<
        sigmoid(0.6) << " " <<
        sigmoid(0.7) << " " <<
        sigmoid(0.8) << " " <<
        sigmoid(0.9) << " " <<
        sigmoid(1.0) << " " << endl;


    cout << "Hello world." << endl;
    return 0;
}
