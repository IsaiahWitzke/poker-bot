#include "VectorUtils.h"
#include <stdlib.h>
#include <math.h>

template<typename T>
vector<T> operator+(vector<T> a, vector<T> b) {
    const int size = a.size();
    vector<T> out = a;
    for (size_t i = 0; i < size; i++) {
        out[i] = out[i] + b[i];
    }
    return out;
}

template<typename T>
vector<T> operator-(vector<T> a, vector<T> b) {
    const int size = a.size();
    vector<T> out = a;
    for (size_t i = 0; i < size; i++) {
        out[i] = out[i] - b[i];
    }
    return out;
}

template<typename T>
vector<T> operator*(vector<T> a, float b) {
    vector<T> out = a;
    for (size_t i = 0; i < a.size(); i++) {
        out[i] = a[i] * b;
    }
    return out;
}

// template <typename T>
// T operator*(const vector<T>& a, const vector<T>& b) {
//     T sum = 0;
//     for (size_t i = 0; i < a.size(); i++) {
//         sum += a[i] * b[i];
//     }
//     return sum;
// }

template <typename T>
void randomizeElem(T &e) {
    if (std::is_same<T, int>::value) {
        e = rand();
    }
    else {  // return a rand number between [0, 1]
        e = (static_cast<T>(rand()) / static_cast<T>(RAND_MAX));
    }
}

template<typename T>
void randomizeVector(vector<T>& v) {
    for (auto& e : v) {
        randomizeElem(e);
    }
}

template <typename T>
vector<T> elementWiseProduct(const vector<T>& a, const vector<T>& b) {
    vector<T> out;
    for (size_t i = 0; i < a.size(); i++) {
        out.push_back(a[i] * b[i]);
    }
    return out;
}

template<typename T>
T norm(const vector<T>& v) {
    T intermediateSum = 0;
    for(auto e : v) {
        intermediateSum += (e * e);
    }
    return sqrt(intermediateSum);
}

// template <typename T>
// Matrix<T> elementWiseProduct(const Matrix<T>& a, const vector<T>& b) {
//     Matrix<T> out(a);
//     for (size_t i = 0; i < b.size(); i++) {
//         out[i] = out[i] * b;
//     }
//     return out;
// }

// add more templates here (if needed)....
template float norm(const vector<float>& v);

template vector<float> elementWiseProduct(const vector<float>& a, const vector<float>& b);
// template Matrix<float> elementWiseProduct(const Matrix<float>& a, const vector<float>& b);

// template vector<float> operator*(const vector<float>& a, const vector<float>& b);

template vector<int> operator+(vector<int> a, vector<int> b);
template vector<float> operator+(vector<float> a, vector<float> b);
template vector<Matrix<float>> operator+(vector<Matrix<float>> a, vector<Matrix<float>> b);
template vector<vector<float>> operator+(vector<vector<float>> a, vector<vector<float>> b);

template vector<int> operator-(vector<int> a, vector<int> b);
template vector<float> operator-(vector<float> a, vector<float> b);
template vector<Matrix<float>> operator-(vector<Matrix<float>> a, vector<Matrix<float>> b);
template vector<vector<float>> operator-(vector<vector<float>> a, vector<vector<float>> b);

template vector<float> operator*(vector<float> a, float b);
template vector<vector<float>> operator*(vector<vector<float>> a, float b);
template vector<Matrix<float>> operator*(vector<Matrix<float>> a, float b);


template void randomizeElem(int &e);
template void randomizeElem(float &e);
template void randomizeElem(double &e);

template void randomizeVector(vector<int>& v);
template void randomizeVector(vector<float>& v);
template void randomizeVector(vector<double>& v);