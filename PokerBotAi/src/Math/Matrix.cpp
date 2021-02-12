#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T fillValue) {
    for (int i = 0; i < cols; ++i) {
        vector<T> col(rows, fillValue);
        this->wrappedMatrix.push_back(col);
    }
}
