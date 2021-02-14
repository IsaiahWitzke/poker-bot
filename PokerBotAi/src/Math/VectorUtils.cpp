#include "VectorUtils.h"
#include <stdlib.h>

vector<float> operator+(vector<float> a, vector<float> b) {
    const int size = a.size();
    vector<float> out = a;
    for (size_t i = 0; i < size; i++) {
        out[i] += b[i];
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

template void randomizeElem(int &e);
template void randomizeElem(float &e);
template void randomizeElem(double &e);

template void randomizeVector(vector<int>& v);
template void randomizeVector(vector<float>& v);
template void randomizeVector(vector<double>& v);