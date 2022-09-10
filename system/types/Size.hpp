/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include <iostream>

#include "Vec2.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct size
{
    constexpr size() = default;

    constexpr size(T _w, T _h)
        : w(_w), h(_h)
    {}

    template<typename C>
    constexpr size(vec2d<C> v)
        : w((C) v.x), h((C) v.y)
    {}

    template<typename C>
    constexpr operator size<C>() const
    {
        return size<C>{(C) w, (C) h};
    }

    template<typename C>
    constexpr explicit operator vec2d<C>() const
    {
        return vec2d<C>{(C) w, (C) h};
    }

    T w, h;
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const size<T> &dt)
{
    os << dt.w << "x" << dt.h;

    return os;
}

using isize = size<int>;
using usize = size<unsigned int>;
using fsize = size<float>;
using dsize = size<double>;

template<typename T1, typename T2>
inline size<T1> operator*(const size<T1> &lhs, const size<T2> &rhs)
{
    return size<T1>{lhs.w * rhs.w, lhs.h * rhs.h};
}

template<typename T1, typename T2>
inline size<T1> operator/(const size<T1> &lhs, const size<T2> &rhs)
{
    return size<T1>{lhs.w / rhs.w, lhs.h / rhs.h};
}

template<typename T1, typename T2>
inline size<T1> operator*=(size<T1> &lhs, const size<T2> &rhs)
{
    return size<T1>{lhs.w *= rhs.w, lhs.h *= rhs.h};
}

template<typename T1, typename T2>
inline size<T1> operator/=(size<T1> &lhs, const size<T2> &rhs)
{
    return size<T1>{lhs.w /= rhs.w, lhs.h /= rhs.h};
}

template<typename T1, typename T2>
inline bool operator==(const size<T1> &lhs, const size<T2> &rhs)
{
    return lhs.w == rhs.w && lhs.h == rhs.h;
}

template<typename T1, typename T2>
inline bool operator!=(const size<T1> &lhs, const size<T2> &rhs)
{
    return lhs.w != rhs.w || lhs.h != rhs.h;
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size<T1> operator*(const size<T1> &in, T2 scaler)
{
    return size<T1>{in.w * (T1) scaler, in.h * (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size<T1> operator/(const size<T1> &in, T2 scaler)
{
    return size<T1>{in.w / (T1) scaler, in.h / (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size<T1> operator/(T2 scaler, const size<T1> &in)
{
    return size<T1>{(T1) scaler / in.w, (T1) scaler / in.h};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size<T1> operator*=(size<T1> &in, T2 scaler)
{
    return size<T1>{in.w *= (T1) scaler, in.h *= (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size<T1> operator/=(size<T1> &in, T2 scaler)
{
    return size<T1>{in.w /= (T1) scaler, in.h /= (T1) scaler};
}

NS_END
