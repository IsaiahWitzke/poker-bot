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
void randomizeElem(T &e, T max, T min) {
    if (std::is_same<T, int>::value) {
        // NOTE: casting to int just to get rid of compile-time errors
        e = (rand() % (int) (max - min)) - min;  // wont be great if max - min is close to max(int)
    }
    else { 
        // return a rand number between [-1, 1]
        // e = 2 * (static_cast<T>(rand()) / static_cast<T>(RAND_MAX)) - 1;
        // return a rand number between [0, 1]
        e = (static_cast<T>(rand()) / static_cast<T>(RAND_MAX));
        e = e * (max - min) - min;  // scale to min-max range
    }
}

template<typename T>
void randomizeVector(vector<T>& v, T maxValue, T minValue) {
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

template<typename T>
void addNoiseToVector(vector<T>& v, T noiseAmplitude) {
    vector<T> noise = vector<T>(v.size());
    randomizeVector(
        noise,
        -noiseAmplitude,
        noiseAmplitude
    );
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


template void randomizeElem(int &e, int max, int min);
template void randomizeElem(float &e, float max, float min);
template void randomizeElem(double &e, double max, double min);

template void randomizeVector(vector<int>& v, int max, int min);
template void randomizeVector(vector<float>& v, float max, float min);
template void randomizeVector(vector<double>& v, double max, double min);

template void addNoiseToVector(vector<int>& v, int noiseAmplitude);
template void addNoiseToVector(vector<float>& v, float noiseAmplitude);
template void addNoiseToVector(vector<double>& v, double noiseAmplitude);