#pragma once

#include <vector>
#include "Matrix.h"

using namespace std;

/**
 * @brief vector-vector addition
 * 
 * @tparam T Any type that allows "T"-"T" addition
 * @param a 
 * @param b 
 * @return vector<T> 
 */
template<typename T>
vector<T> operator+(vector<T> a, vector<T> b);

/**
 * @brief Vector-scalar multiplication
 * 
 * @tparam T Any type that allows right-side scalar-"T"-multiplication
 * @param a 
 * @param b 
 * @return vector<T> 
 */
template <typename T>
vector<T> operator*(vector<T> a, float b);

template <typename T>
void randomizeElem(T &e);

template<typename T>
void randomizeVector(vector<T>& v);
