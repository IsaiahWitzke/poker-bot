#include "VectorUtils.h"
#include <stdlib.h>

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
vector<T> operator*(vector<T> a, float b) {
    vector<T> out = a;
    for (size_t i = 0; i < a.size(); i++) {
        out[i] = a[i] * b;
    }
    return out;
}

template <typename T>
void randomizeElem(T &e) {
    if (std::is_same<T, int>::value) {
        e = rand();
    }
    else {  // return a rand number between [-1, 1]
        e = 2 * (static_cast<T>(rand()) / static_cast<T>(RAND_MAX)) - 1;
    }
}

template<typename T>
void randomizeVector(vector<T>& v) {
    for (auto& e : v) {
        randomizeElem(e);
    }
}

// add more templates here (if needed)....
template vector<int> operator+(vector<int> a, vector<int> b);
template vector<float> operator+(vector<float> a, vector<float> b);
template vector<Matrix<float>> operator+(vector<Matrix<float>> a, vector<Matrix<float>> b);
template vector<vector<float>> operator+(vector<vector<float>> a, vector<vector<float>> b);

template vector<float> operator*(vector<float> a, float b);
template vector<vector<float>> operator*(vector<vector<float>> a, float b);
template vector<Matrix<float>> operator*(vector<Matrix<float>> a, float b);


template void randomizeElem(int &e);
template void randomizeElem(float &e);
template void randomizeElem(double &e);

template void randomizeVector(vector<int>& v);
template void randomizeVector(vector<float>& v);
template void randomizeVector(vector<double>& v);