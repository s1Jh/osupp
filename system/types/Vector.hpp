// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 27.01.23.
//
#pragma once

#include "define.hpp"

#include "Matrix.hpp"
#include "ToFromString.hpp"

#include <array>
#include <cstring>
#include <initializer_list>
#include <type_traits>

#define DECOMPOSE_VEC2(vec) vec[0], vec[1]
#define DECOMPOSE_VEC3(vec) vec[0], vec[1], vec[2]
#define DECOMPOSE_VEC4(vec) vec[0], vec[1], vec[2], vec[3]

namespace PROJECT_NAMESPACE
{

template<typename T> requires std::is_arithmetic_v<T>
struct size3;

template<typename T> requires std::is_arithmetic_v<T>
struct size;

//namespace detail
//{

template<typename T, unsigned long Comps> requires std::is_arithmetic_v<T> && (Comps > 0)
struct vec
{
protected:
    T components[Comps] = {};

public:
    using Type = T;
    const static unsigned int Size = Comps;

    inline constexpr T operator[] (unsigned long i) const noexcept
    {
        return components[i % Size];
    }

    inline constexpr T& operator[] (unsigned long i) noexcept
    {
        return components[i % Size];
    }

    inline constexpr explicit vec(T values ...) noexcept
        : vec(std::initializer_list<T>(values))
    {
    }

    inline constexpr explicit vec(const Mat<T, 1, Comps>& mat) noexcept
    {
        auto &comps = vec<T, Comps>::components;
        const unsigned long len = vec<T, Comps>::Size;

        for (unsigned long i = 0; i < len; i++) {
            comps[i] = mat.at(i, 0);
        }
    }

    inline constexpr vec(std::initializer_list<T> l) noexcept
    {
        auto &comps = vec<T, Comps>::components;

        const unsigned long len = vec<T, Comps>::Size;
        unsigned long i = 0;
        for (const auto &val : l) {
            comps[i] = val;
            i++;
            if (len == i) {
                break;
            }
        }
    }

    inline constexpr vec() noexcept
    {
        for (unsigned long i = 0; i < Size; i++) {
            components[i] = 0;
        }
    }

    inline constexpr vec(const vec &left) noexcept
    {
        for (unsigned long i = 0; i < Size; i++) {
            components[i] = left[i];
        }
    }

    inline constexpr vec(vec &&left) noexcept
    {
        for (unsigned long i = 0; i < Size; i++) {
            components[i] = left[i];
        }
    }

    inline constexpr vec &operator=(vec &&left) noexcept
    {
        if (this == &left) {
            return *this;
        }

        for (unsigned long i = 0; i < Size; i++) {
            components[i] = left[i];
        }

        return *this;
    }

    inline constexpr vec &operator=(const vec &left) noexcept
    {
        if (this == &left) {
            return *this;
        }

        for (unsigned long i = 0; i < Size; i++) {
            components[i] = left[i];
        }

        return *this;
    }

    template <typename C>
    inline constexpr operator Mat<C, 1, Comps>() const noexcept
    {
        Mat<C, 1, Comps> res;
        for (unsigned long i = 0; i < Comps; i++) {
            res.at(i, 0) = static_cast<C>(components[i]);
        }
        return res;
    }

    template<typename C>
    requires std::is_base_of_v<vec<typename C::Type, Comps>, C>
    inline constexpr operator C() const noexcept
    {
        C res;
        for (unsigned long i = 0; i < Comps; i++) {
            res[i] = static_cast<typename C::Type>(components[i]);
        }
        return res;
    }
};

//}

//template<typename T, unsigned long Comps>
//struct vec: public detail::vector_like<T, Comps>
//{
//    using Parent = detail::vector_like<T, Comps>;
//    using Parent::vector_like;
//    using Parent::operator[];
//};
//
//template<typename T>
//struct vec<T, 2>: public detail::vector_like<T, 2>
//{
//    using Parent = detail::vector_like<T, 2>;
//
//    using Parent::vector_like;
//    using Parent::operator[];
//
//    inline constexpr vec<T, 2>() noexcept : Parent() {}
//
//    inline constexpr vec<T, 2>(std::initializer_list<T> l) noexcept : Parent(l) {}
//
//    inline constexpr explicit vec<T, 2>(T values ...) noexcept
//    : Parent(values)
//    {
//    }
//
//    inline constexpr vec<T, 2>& operator= (const vec<T, 2>& left) noexcept {
//        Parent::operator=(left); return *this;
//    }
//    inline constexpr vec<T, 2>& operator= (vec<T, 2>&& left) noexcept {
//        Parent::operator=(left); return *this;
//    }
//    inline constexpr vec<T, 2>(const vec<T, 2>& left) noexcept : Parent(dynamic_cast<const Parent&>(left)) {}
//    inline constexpr vec<T, 2>(vec<T, 2>&& left) noexcept : Parent(dynamic_cast<Parent&&>(left)) {}
//
//    T &x{Parent::components[0]};
//    T &y{Parent::components[1]};
//
//    template <typename C>
//    constexpr operator size<C> () const noexcept
//    {
//        return { static_cast<C>(this->components[0]), static_cast<C>(this->components[1]) };
//    }
//};
//
//template<typename T>
//struct vec<T, 3>: public detail::vector_like<T, 3>
//{
//    using Parent = detail::vector_like<T, 3>;
//    using Parent::vector_like;
//    using Parent::operator[];
//
//    inline constexpr vec<T, 3>() noexcept : Parent() {}
//
//    inline constexpr vec<T, 3>(std::initializer_list<T> l) noexcept : Parent(l) {}
//
//    inline constexpr vec<T, 3>& operator= (const vec<T, 3>& left) noexcept {
//        Parent::operator=(left); return *this;
//    }
//    inline constexpr vec<T, 3>& operator= (vec<T, 3>&& left) noexcept {
//        Parent::operator=(left); return *this;
//    }
//    inline constexpr vec<T, 3>(const vec<T, 3>& left) noexcept : Parent(left) {}
//    inline constexpr vec<T, 3>(vec<T, 3>&& left) noexcept : Parent(left) {}
//
//    T &x = Parent::components[0];
//    T &y = Parent::components[1];
//    T &z = Parent::components[2];
//
//    template <typename C>
//    constexpr operator size3<C> () const noexcept
//    {
//        return {
//            static_cast<C>(this->components[0]),
//            static_cast<C>(this->components[1]),
//            static_cast<C>(this->components[2])
//        };
//    }
//};
//
//template<typename T>
//struct vec<T, 4>: public detail::vector_like<T, 4>
//{
//    using Parent = detail::vector_like<T, 4>;
//    using Parent::vector_like;
//    using Parent::operator[];
//
//    T &x = Parent::components[0];
//    T &y = Parent::components[1];
//    T &z = Parent::components[2];
//    T &w = Parent::components[3];
//};

template<typename T, unsigned long Comps>
std::string ToString(const vec<T, Comps>& vec)
{
    std::string result("(");
    for (unsigned long i = 0; i < Comps; i++) {
        result += std::to_string(vec[i]);
        if (i != Comps - 1) {
            result += ',';
        }
    }
    return result + ')';
}

template<typename T, unsigned long Comps>
std::optional<vec<T, Comps>> FromString(const std::string& str)
{
    WRAP_CONSTEXPR_ASSERTION("Not implemeneted!");
    return {};
}

template<typename T>
using vec2d = vec<T, 2>;

using ivec2d = vec2d<int>;
using uvec2d = vec2d<unsigned int>;
using fvec2d = vec2d<float>;
using dvec2d = vec2d<double>;

template<typename T>
using vec3d = vec<T, 3>;

using ivec3d = vec3d<int>;
using uvec3d = vec3d<unsigned int>;
using fvec3d = vec3d<float>;
using dvec3d = vec3d<double>;

template<typename T>
using vec4d = vec<T, 4>;

using ivec4d = vec4d<int>;
using uvec4d = vec4d<unsigned int>;
using fvec4d = vec4d<float>;
using dvec4d = vec4d<double>;

template<typename T1, unsigned long C1, typename T2>
inline vec<T1, C1> operator+(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    vec<T1, C1> ret;
    for (unsigned long i = 0; i < C1; i++) {
        ret[i] = left[i] + right[i];
    }
    return ret;
}

template<typename T1, unsigned long C1, typename T2>
inline vec<T1, C1> operator-(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    vec<T1, C1> ret;
    for (unsigned long i = 0; i < C1; i++) {
        ret[i] = left[i] - right[i];
    }
    return ret;
}

template<typename T1, unsigned long C1, typename T2>
inline vec<T1, C1> operator*(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    vec<T1, C1> ret;
    for (unsigned long i = 0; i < C1; i++) {
        ret[i] = left[i] * right[i];
    }
    return ret;
}

template<typename T1, unsigned long C1, typename T2>
inline vec<T1, C1> operator/(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    vec<T1, C1> ret;
    for (unsigned long i = 0; i < C1; i++) {
        ret[i] = left[i] / right[i];
    }
    return ret;
}

template<typename T1, unsigned long C1, typename T2>
inline void operator+=(vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    for (unsigned long i = 0; i < C1; i++) {
        left[i] += right[i];
    }
}

template<typename T1, unsigned long C1, typename T2>
inline void operator-=(vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    for (unsigned long i = 0; i < C1; i++) {
        left[i] -= right[i];
    }
}

template<typename T1, unsigned long C1, typename T2>
inline void operator*=(vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    for (unsigned long i = 0; i < C1; i++) {
        left[i] *= right[i];
    }
}

template<typename T1, unsigned long C1, typename T2>
inline void operator/=(vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    for (unsigned long i = 0; i < C1; i++) {
        left[i] /= right[i];
    }
}

template<typename T1, unsigned long C1, typename T2>
inline bool operator==(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    for (unsigned long i = 0; i < C1; i++) {
        if (left[i] != right[i]) {
            return false;
        }
    }
    return true;
}

template<typename T1, unsigned long C1, typename T2>
inline bool operator!=(const vec<T1, C1> &left, const vec<T2, C1> &right) noexcept
{
    return !(left == right);
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator+(const vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] + right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator-(const vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] - right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator*(const vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] * right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator/(const vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] / right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator+=(vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] += right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator-=(vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] -= right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator*=(vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] *= right;
    }
    return ret;
}

template<typename T1, unsigned long C, typename T2>
requires std::is_arithmetic_v<T2>
inline vec<T1, C> operator/=(vec<T1, C> &left, T2 right) noexcept
{
    vec<T1, C> ret;
    for (unsigned long i = 0; i < C; i++) {
        ret[i] = left[i] /= right;
    }
    return ret;
}

}
