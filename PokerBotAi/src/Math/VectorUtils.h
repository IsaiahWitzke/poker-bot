#pragma once

#include <vector>

using namespace std;

vector<float> operator+(vector<float> a, vector<float> b);

template <typename T>
void randomizeElem(T &e);

template<typename T>
void randomizeVector(vector<T>& v);
