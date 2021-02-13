#pragma once

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class Matrix {
private:
    // wrappedMatrix[i][j]: ith row, jth col
    vector<vector<T>> wrappedMatrix;
public:
    const int rows;
    const int cols;

    Matrix(int rows, int cols, T fillValue);
    Matrix(vector<vector<T>> m);    // TODO: make copy assignment constructor
    ~Matrix();

    vector<T>& operator[](const unsigned int i);     // accesses row vectors

    Matrix<T> operator+(const Matrix<T>& m);        // matrix-matrix addition
    Matrix<T> operator*(const Matrix<T>& m);        // matrix-matrix multiplication
    vector<T> operator*(const vector<T>& v);        // matrix * vector multiplication
    vector<T> operator()(const vector<T>& v);       // same as matrix * vector multiplication
    Matrix<T> operator*(const T s);                 // scalar * matrix multiplication
};

