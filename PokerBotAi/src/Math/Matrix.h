#pragma once

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class Matrix {
private:
    vector<vector<T>> wrappedMatrix;
public:
    Matrix(int rows, int cols, T fillValue);
};
