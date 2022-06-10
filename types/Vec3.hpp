#pragma once

#include <iostream>
#include <cmath>

#include "Matrix.hpp"
#include "define.hpp"

#define DECOMPOSE_VEC3 (vec) vec.x, vec.y, vec.z

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct size3;

template<typename T>
struct vec3d {
    vec3d() :
            x(0), y(0), z(0) {}

    vec3d(T _x, T _y, T _z) :
            x(_x), y(_y), z(_z) {}

    template<typename C>
    vec3d(size3<C> s) :
            x(s.w), y(s.h), z(s.l) {}

    template<typename C>
    vec3d(Mat1x3 <C> m) :
            x(m[0][0]), y(m[0][1]), z(m[0][2]) {}

    template<typename C>
    inline operator vec3d<C>() const {
        return vec3d<C>{(C) x, (C) y, (C) z};
    }

    template<typename C>
    explicit inline operator size3<C>() const {
        return size3<C>{(C) x, (C) y, (C) z};
    }

    template<typename C>
    operator Mat1x3<C>() const {
        Mat1x3<C> res;
        res[0][0] = x;
        res[0][1] = y;
        res[0][2] = z;
        return res;
    }

    T x, y, z;
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const vec3d<T> &dt) {
    os << "("
       << dt.x << ";"
       << dt.y << ";"
       << dt.z << ")";

    return os;
}

using ivec3d = vec3d<int>;
using uvec3d = vec3d<unsigned int>;
using fvec3d = vec3d<float>;
using dvec3d = vec3d<double>;

// vec-vec operations
template<typename T1, typename T2>
inline vec3d<T2> operator+(const vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T2>{left.x + (T1) right.x, left.y + (T1) right.y, left.z + (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator-(const vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x - (T1) right.x, left.y - (T1) right.y, left.z - (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator*(const vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x * (T1) right.x, left.y * (T1) right.y, left.z * (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator/(const vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x / (T1) right.x, left.y / (T1) right.y, left.z / (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator+=(vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x += (T1) right.x, left.y += (T1) right.y, left.z += (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator-=(vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x -= (T1) right.x, left.y -= (T1) right.y, left.z -= (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator*=(vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x *= (T1) right.x, left.y *= (T1) right.y, left.z *= (T1) right.z};
}

template<typename T1, typename T2>
inline vec3d<T1> operator/=(vec3d<T1> &left, const vec3d<T2> &right) {
    return vec3d<T1>{left.x /= (T1) right.x, left.y /= (T1) right.y, left.z /= (T1) right.z};
}

template<typename T1, typename T2>
inline bool operator==(const vec3d<T1> &left, const vec3d<T2> &right) {
    return
            left.x == (T1) right.x &&
            left.y == (T1) right.y &&
            left.z == (T1) right.z;
}

template<typename T1, typename T2>
inline bool operator!=(const vec3d<T1> &left, const vec3d<T2> &right) {
    return
            left.x != (T1) right.x ||
            left.y != (T1) right.y ||
            left.z != (T1) right.z;
}

// vec-scalar operations
template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator+(const vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x + (T1) right, left.y + (T1) right, left.z + (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator-(const vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x - (T1) right, left.y - (T1) right, left.z - (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator*(const vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x * (T1) right, left.y * (T1) right, left.z * (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator/(const vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x / (T1) right, left.y / (T1) right, left.z / (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator+=(vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x += (T1) right, left.y += (T1) right, left.z += (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator-=(vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x -= (T1) right, left.y -= (T1) right, left.z -= (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator*=(vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x *= (T1) right, left.y *= (T1) right, left.z *= (T1) right};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline vec3d<T1> operator/=(vec3d<T1> &left, T2 right) {
    return vec3d<T1>{left.x /= (T1) right, left.y /= (T1) right, left.z /= right};
}

NS_END
