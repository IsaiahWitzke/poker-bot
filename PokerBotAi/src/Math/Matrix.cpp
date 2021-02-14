#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T fillValue) : rows(rows), cols(cols) {
    for (int i = 0; i < rows; ++i) {
        vector<T> row(cols, fillValue);
        this->m.push_back(row);
    }
}

template <typename T>
Matrix<T>::Matrix(vector<vector<T>> m) : rows(m.size()), cols(m[0].size()) {
    this->m = m;
}

template <typename T>
Matrix<T>::~Matrix() {}

template <typename T>
T Matrix<T>::getElem(size_t row, size_t col) const {
    return this->m[row][col];
}

template <typename T>
vector<T>& Matrix<T>::operator[](const size_t i) {
    return this->m[i];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& mInput) {
    // TODO: could do some checking of rows/columns here

    Matrix<T> mOut(mInput);

    for (int i = 0; i < this->m.size(); ++i) {
        for (int j = 0; j < this->m[i].size(); ++j) {
            mOut[i][j] += this->m[i][j];
        }
    }

    return mOut;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mInput) {
    // TODO: could do some checking of rows/columns here

    // if this is ixb and input is bxj, then output is ixj
    Matrix<T> mOut(this->rows, mInput.cols, m[0][0]);

    for (size_t mOutRowIdx = 0; mOutRowIdx < this->rows; ++mOutRowIdx) {
        for (size_t mOutColIdx = 0; mOutColIdx < mInput.cols; ++mOutColIdx) {
            T mOutEntry = m[0][0] - m[0][0]; // start at 0 of whatever type that the input matrix is

            // mOutEntry is the dot product of the mOutRowIdx row of this->m and the mOutColIdx col of mInput
            for (size_t i = 0; i < this->cols; ++i) {
                mOutEntry += this->m[mOutRowIdx][i] * mInput.getElem(i, mOutColIdx);
            }

            mOut[mOutRowIdx][mOutColIdx] = mOutEntry;
        }
    }

    return mOut;    // something something elision... could improve efficiency by retruning ptrs
}

template <typename T>
vector<T> Matrix<T>::operator*(const vector<T>& v) {
    vector<T> vOut(this->rows, v[0] - v[0]);    // initialize empty vector
    for(int row = 0; row < this->rows; ++row) {
        for (size_t i = 0; i < this->cols; ++i)
        {
            vOut[row] += v[i] * m[row][i];
        }
    }
    return vOut;
}

template <typename T>
vector<T> Matrix<T>::operator()(const vector<T>& v) {
    return this->operator*(v);  // does this imply an extra copy...????
}

// see https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor ...

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;
