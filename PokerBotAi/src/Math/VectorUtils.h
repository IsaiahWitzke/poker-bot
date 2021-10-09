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

template<typename T>
vector<T> operator-(vector<T> a, vector<T> b);

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

// template <typename T>
// T operator*(const vector<T>& a, const vector<T>& b);

template <typename T>
vector<T> elementWiseProduct(const vector<T>& a, const vector<T>& b);

// template <typename T>
// Matrix<T> elementWiseProduct(const Matrix<T>& a, const vector<T>& b);

template <typename T>
void randomizeElem(T &e, T maxValue = 1, T minValue = 0);

template<typename T>
void randomizeVector(vector<T>& v, T maxValue = 1, T minValue = 0);

template<typename T>
void addNoiseToVector(vector<T>& v, T noiseAmplitude);

/**
 * @brief Euclidian size of vector
 * 
 * @tparam T 
 * @param v 
 * @return T 
 */
template<typename T>
T norm(const vector<T>& v);
