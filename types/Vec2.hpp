#pragma once

#include <iostream>

#include "Traits.hpp"
#include "Matrix.hpp"
#include "define.hpp"

#define DECOMPOSE_VEC2(vec) vec.x, vec.y

namespace GAME_TITLE {

    template<typename T> requires std::is_arithmetic_v<T>
    struct size;

    template<typename T> requires std::is_arithmetic_v<T>
    struct vec2d {
        vec2d() :
                x(0), y(0) {}

        vec2d(T _x, T _y) :
                x(_x), y(_y) {}

        template<typename C>
        vec2d(size<C> s) :
                x(s.w), y(s.h) {}

        template<typename C>
        vec2d(Mat1x2<C> m) :
                x(m[0][0]), y(m[0][1]) {}

        template<typename C>
        inline operator vec2d<C>() const {
            return vec2d<C>{(C) x, (C) y};
        }

        template<typename C>
        explicit inline operator size<C>() const {
            return size<C>{(C) x, (C) y};
        }

        template<typename C>
        operator Mat1x2<C>() const {
            Mat1x2<C> res;
            res[0][0] = x;
            res[0][1] = y;
            return res;
        }

        T x, y;
    };


    template<typename T>
    std::ostream &operator<<(std::ostream &os, const vec2d<T> &dt) {
        os << "(" << dt.x << ";" << dt.y << ")";

        return os;
    }

    using ivec2d = vec2d<int>;
    using uvec2d = vec2d<unsigned int>;
    using fvec2d = vec2d<float>;
    using dvec2d = vec2d<double>;

// vec-vec operations
    template<typename T1, typename T2>
    inline vec2d<T1> operator+(const vec2d<T1> &left, const vec2d<T2> &right) {
        return vec2d<T1>{left.x + (T2) right.x, left.y + (T2) right.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator-(const vec2d<T1> &left, const vec2d<T2> &right) {
        return vec2d<T1>{left.x - (T2) right.x, left.y - (T2) right.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator*(const vec2d<T1> &left, const vec2d<T2> &right) {
        return vec2d<T1>{left.x * (T2) right.x, left.y * (T2) right.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator/(const vec2d<T2> &right, const vec2d<T1> &left) {
        return vec2d<T1>{left.x / (T2) right.x, left.y / (T2) right.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator+=(vec2d<T1> &first, const vec2d<T2> &second) {
        return vec2d<T1>{first.x += (T1) second.x, first.y += (T1) second.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator-=(vec2d<T1> &first, const vec2d<T2> &second) {
        return vec2d<T1>{first.x -= (T1) second.x, first.y -= (T1) second.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator*=(vec2d<T1> &first, const vec2d<T2> &second) {
        return vec2d<T1>{first.x *= (T1) second.x, first.y *= (T1) second.y};
    }

    template<typename T1, typename T2>
    inline vec2d<T1> operator/=(vec2d<T1> &first, const vec2d<T2> &second) {
        return vec2d<T1>{first.x /= (T1) second.x, first.y /= (T1) second.y};
    }

    template<typename T1, typename T2>
    inline bool operator==(const vec2d<T1> &left, const vec2d<T2> &right) {
        return
                left.x == (T1) right.x &&
                left.y == (T1) right.y;
    }

    template<typename T1, typename T2>
    inline bool operator!=(const vec2d<T1> &left, const vec2d<T2> &right) {
        return
                left.x != (T1) right.x ||
                left.y != (T1) right.y;
    }

// vec-scalar operations
    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator+(const vec2d<T1> &left, T2 right) {
        return vec2d<T1>{left.x + (T1) right, left.y + (T1) right};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator-(const vec2d<T1> &left, T2 right) {
        return vec2d<T1>{left.x - (T1) right, left.y - (T1) right};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator*(const vec2d<T1> &left, T2 right) {
        return vec2d<T1>{left.x * (T1) right, left.y * (T1) right};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator/(const vec2d<T1> &left, T2 right) {
        return vec2d<T1>{left.x / (T1) right, left.y / (T1) right};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator/(T2 left, const vec2d<T1> &right) {
        return vec2d<T1>{ (T1) right / left.x, (T1) right / left.y };
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator+=(vec2d<T1> &first, T2 second) {
        return vec2d<T1>{first.x += (T1) second, first.y += (T1) second};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator-=(vec2d<T1> &first, T2 second) {
        return vec2d<T1>{first.x -= (T1) second, first.y -= (T1) second};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator*=(vec2d<T1> &first, T2 second) {
        return vec2d<T1>{first.x *= (T1) second, first.y *= (T1) second};
    }

    template<typename T1, typename T2>
    requires std::is_arithmetic_v<T2>
    inline vec2d<T1> operator/=(vec2d<T1> &first, T2 second) {
        return vec2d<T1>{first.x /= (T1) second, first.y /= (T1) second};
    }

} // oe
