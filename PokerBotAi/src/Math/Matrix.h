#pragma once

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class Matrix {
private:
    // m acts as a wrapped version of a matrix
    // m[i][j]: ith row, jth col
    vector<vector<T>> m;

public:
    int numRows;
    int numCols;

    Matrix();
    Matrix(int rows, int cols, T fillValue);
    Matrix(vector<T> v, bool isTranspose = false);  // initialized matrix with 1 column = v
    Matrix(vector<vector<T>> m);
    Matrix<T>& operator=(const Matrix<T> matrix);
    ~Matrix();

    T getElem(size_t row, size_t col) const;
    vector<T>& operator[](const size_t i);          // accesses row vectors

    Matrix<T> operator+(const Matrix<T>& m);        // matrix-matrix addition
    Matrix<T> operator-(const Matrix<T>& m);        // matrix-matrix subtraction
    Matrix<T> operator*(const Matrix<T>& m);        // matrix-matrix multiplication
    vector<T> operator*(const vector<T>& v);        // matrix * vector multiplication
    vector<T> operator()(const vector<T>& v);       // same as matrix * vector multiplication
    Matrix<T> operator*(const T s);                 // scalar * matrix multiplication
    void operator+=(Matrix<T>& m);
    void operator-=(Matrix<T>& m);

    void addRow(vector<T> row);

    Matrix<T> transpose();                          // returns the transposed version of this matrix

    void randomize();
};
