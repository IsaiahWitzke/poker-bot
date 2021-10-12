#include "Matrix.h"
#include "VectorUtils.h"

template <typename T>
Matrix<T>::Matrix() {}

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T fillValue) : numRows(rows), numCols(cols) {
    for (size_t i = 0; i < rows; ++i) {
        vector<T> row(cols, fillValue);
        this->m.push_back(row);
    }
}

template <typename T>
Matrix<T>::Matrix(vector<T> v, bool isTranspose) : numRows(v.size()), numCols(1) {
    if (isTranspose) {
        numCols = numRows;
        numRows = 1;
        this->m.push_back(v);
    }
    else {
        for (size_t i = 0; i < v.size(); ++i) {
            vector<T> row(1, v[i]);
            this->m.push_back(row);
        }
    }
}

template <typename T>
Matrix<T>::Matrix(vector<vector<T>> m) : numRows(m.size()), numCols(m[0].size()) {
    this->m = m;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> matrix) {
    m = matrix.m;
    numRows = matrix.numRows;
    numCols = matrix.numCols;
    return *this;
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
Matrix<T> Matrix<T>::operator-(const Matrix<T>& mInput) {
    // TODO: could do some checking of rows/columns here

    Matrix<T> mOut(mInput);

    for (int i = 0; i < this->m.size(); ++i) {
        for (int j = 0; j < this->m[i].size(); ++j) {
            mOut[i][j] -= this->m[i][j];
        }
    }

    return mOut;
}


template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mInput) {
    // TODO: could do some checking of rows/columns here

    // if this is ixb and input is bxj, then output is ixj
    Matrix<T> mOut(this->numRows, mInput.numCols, m[0][0]);

    for (size_t mOutRowIdx = 0; mOutRowIdx < this->numRows; ++mOutRowIdx) {
        for (size_t mOutColIdx = 0; mOutColIdx < mInput.numCols; ++mOutColIdx) {
            T mOutEntry = m[0][0] - m[0][0]; // start at 0 of whatever type that the input matrix is

            // mOutEntry is the dot product of the mOutRowIdx row of this->m and the mOutColIdx col of mInput
            for (size_t i = 0; i < this->numCols; ++i) {
                mOutEntry += this->m[mOutRowIdx][i] * mInput.getElem(i, mOutColIdx);
            }

            mOut[mOutRowIdx][mOutColIdx] = mOutEntry;
        }
    }

    return mOut;    // something something elision... could improve efficiency by retruning ptrs
}

template <typename T>
vector<T> Matrix<T>::operator*(const vector<T>& v) {
    vector<T> vOut(this->numRows, v[0] - v[0]);    // initialize empty vector
    for (int row = 0; row < this->numRows; ++row) {
        for (size_t i = 0; i < this->numCols; ++i) {
            vOut[row] += v[i] * m[row][i];
        }
    }
    return vOut;
}

template <typename T>
void Matrix<T>::operator+=(Matrix<T>& m) {
    for (size_t row = 0; row < this->numRows; ++row) {
        for (size_t col = 0; col < this->numCols; ++col) {
            this->m[row][col] += m[row][col];
        }
    }
}

template <typename T>
void Matrix<T>::operator-=(Matrix<T>& m) {
    for (size_t row = 0; row < this->numRows; ++row) {
        for (size_t col = 0; col < this->numCols; ++col) {
            this->m[row][col] -= m[row][col];
        }
    }
}

template <typename T>
vector<T> Matrix<T>::operator()(const vector<T>& v) {
    return this->operator*(v);  // does this use an extra copy...????
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T s) {
    Matrix<T> mOut(m);
    for (size_t row = 0; row < this->numRows; ++row) {
        for (size_t i = 0; i < this->numCols; ++i) {
            mOut[row][i] *= s;
        }
    }
    return mOut;
}

template <typename T>
void Matrix<T>::randomize(T max, T min) {
    for (size_t i = 0; i < numRows; i++) {
        randomizeVector(m[i], max, min);
    }
}

template <typename T>
void Matrix<T>::addRow(vector<T> row) {
    m.push_back(row);
}

template <typename T>
Matrix<T> Matrix<T>::transpose() {
    Matrix<T> out(numCols, numRows, 0.0);
    for (size_t rowIdx = 0; rowIdx < numRows; rowIdx++) {
        for (size_t colIdx = 0; colIdx < numCols; colIdx++) {
            out[colIdx][rowIdx] = m[rowIdx][colIdx];
        }
    }
    return out;
}

template <typename T>
string Matrix<T>::toString(int indentSpaces) const {
    string out = "";
    string indent = string(indentSpaces, ' ');
    out += indent + "[\n";

    // for each row...
    for (size_t rowIdx = 0; rowIdx < numRows; rowIdx++) {
        out += indent + "    [";
        // for each column...
        for (size_t colIdx = 0; colIdx < numCols; colIdx++) {
            out += to_string(m[rowIdx][colIdx]);
            if (colIdx != numCols - 1)
                out += ", ";
        }
        out += "]";

        if (rowIdx != numRows - 1)
            out += ",";

        out += "\n";
    }
    out += indent + "]";
	return out;
}

template <typename T>
ostream& operator << (ostream& out, const Matrix<T>& nn) {
    out << nn.toString(0);
    return out;
}

// see https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;

template ostream& operator << (ostream& out, const Matrix<int>& nn);
template ostream& operator << (ostream& out, const Matrix<float>& nn);
template ostream& operator << (ostream& out, const Matrix<double>& nn);
