// PokerBotAi.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Math/Matrix.h"
#include <vector>

using namespace std;

int main()
{
    Matrix<float> m1(3, 5, 0.65);
    Matrix<float> m2(5, 3, 0.65);
    Matrix<float> m3 = m1 * m2;
    
    int test = 0;
	cout << "Hello world." << endl;
	return 0;
}
