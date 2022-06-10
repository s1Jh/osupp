#pragma once

#include <iostream>

#include "Matrix.hpp"
#include "define.hpp"

#define DECOMPOSE_VEC4 (vec) vec.x, vec.y, vec.z, vec.w

NS_BEGIN

template<typename T>
struct vec4d {
    vec4d() :
            x(0), y(0), z(0), w(0) {}

    vec4d(T _x, T _y, T _z, T _w) :
            x(_x), y(_y), z(_z), w(_w) {}

    template<typename C>
    vec4d(Mat1x4 <C> m) :
            x(m[0][0]), y(m[0][1]), z(m[0][2]), w(m[0][3]) {}

    template<typename C>
    inline operator vec4d<C>() const {
        return vec4d<C>{(C) x, (C) y, (C) z, (C) w};
    }

    template<typename C>
    explicit operator Mat1x4<C>() const {
        Mat1x4<C> res;
        res[0][0] = x;
        res[0][1] = y;
        res[0][2] = z;
        res[0][3] = w;
        return res;
    }

    T x, y, z, w;
};


template<typename T>
std::ostream &operator<<(std::ostream &os, const vec4d<T> &dt) {
    os << "("
       << dt.x << ";"
       << dt.y << ";"
       << dt.z << ";"
       << dt.w << ")";

    return os;
}

using ivec4d = vec4d<int>;
using uvec4d = vec4d<unsigned int>;
using fvec4d = vec4d<float>;
using dvec4d = vec4d<double>;

// vec-vec operations
template<typename T1, typename T2>
inline vec4d<T1> operator+(const vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x + (T1) right.x, left.y + (T1) right.y, left.z + (T1) right.z, left.w + (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator-(const vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x - (T1) right.x, left.y - (T1) right.y, left.z - (T1) right.z, left.w - (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator*(const vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x * (T1) right.x, left.y * (T1) right.y, left.z * (T1) right.z, left.w * (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator/(const vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x / (T1) right.x, left.y / (T1) right.y, left.z / (T1) right.z, left.w / (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator+=(vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x += (T1) right.x, left.y += (T1) right.y, left.z += (T1) right.z, left.w += (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator-=(vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x -= (T1) right.x, left.y -= (T1) right.y, left.z -= (T1) right.z, left.w -= (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator*=(vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x *= (T1) right.x, left.y *= (T1) right.y, left.z *= (T1) right.z, left.w *= (T1) right.w};
}

template<typename T1, typename T2>
inline vec4d<T1> operator/=(vec4d<T1> &left, const vec4d<T2> &right) {
    return vec4d<T1>{left.x /= (T1) right.x, left.y /= (T1) right.y, left.z /= (T1) right.z, left.w /= right.w};
}

template<typename T1, typename T2>
inline bool operator==(const vec4d<T1> &left, const vec4d<T2> &right) {
    return
            left.x == (T1) right.x &&
            left.y == (T1) right.y &&
            left.z == (T1) right.z &&
            left.w == (T1) right.w;
}

template<typename T1, typename T2>
inline bool operator!=(const vec4d<T1> &left, const vec4d<T2> &right) {
    return
            left.x != (T1) right.x ||
            left.y != (T1) right.y ||
            left.z != (T1) right.z ||
            left.w != (T1) right.w;
}

// vec-scalar operations
template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator+(const vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x + (T1) right, left.y + (T1) right, left.z + (T1) right, left.w + (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator-(const vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x - (T1) right, left.y - (T1) right, left.z - (T1) right, left.w - (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator*(const vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x * (T1) right, left.y * (T1) right, left.z * (T1) right, left.w * (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator/(const vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x / (T1) right, left.y / (T1) right, left.z / (T1) right, left.w / (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator+=(vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x += (T1) right, left.y += (T1) right, left.z += (T1) right, left.w += (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator-=(vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x -= (T1) right, left.y -= (T1) right, left.z -= (T1) right, left.w -= (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator*=(vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x *= (T1) right, left.y *= (T1) right, left.z *= (T1) right, left.w *= (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec4d<T1> operator/=(vec4d<T1> &left, T2 right) {
    return vec4d<T1>{left.x /= (T1) right, left.y /= (T1) right, left.z /= (T1) right, left.w /= (T1) right};
}

NS_END
