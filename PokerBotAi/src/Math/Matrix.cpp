#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T fillValue) : rows(rows), cols(cols) {
    for (int i = 0; i < rows; ++i) {
        vector<T> row(cols, fillValue);
        this->wrappedMatrix.push_back(row);
    }
}

template <typename T>
Matrix<T>::Matrix(vector<vector<T>> m) : rows(m.size()), cols(m[0].size()) {
    this->wrappedMatrix = m;
}

template <typename T>
Matrix<T>::~Matrix() {}

template <typename T>
vector<T>& Matrix<T>::operator[](const unsigned int i) {
    return wrappedMatrix[i];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) {
    // TODO: could do some checking of rows/columns here

    // um... may create memory leaks... TODO: test with valgrind later
    Matrix<T> retMatrix(m);

    for (int i = 0; i < wrappedMatrix.size(); ++i) {
        for (int j = 0; j < wrappedMatrix[i].size(); ++j) {
            retMatrix[i][j] += wrappedMatrix[i][j];
        }
    }

    return retMatrix;
}

// see https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;
