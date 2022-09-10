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
#include <type_traits>

#include "Vec3.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct size3
{
    size3() = default;

    size3(T _w, T _h, T _l)
        : w(_w), h(_h), l(_l)
    {}

    template<typename C>
    size3(vec3d<C> v)
        : w((C) v.x), h((C) v.y), l((C) v.z)
    {}

    template<typename C>
    operator size3<C>() const
    {
        return size3<C>{(C) w, (C) h, (C) l};
    }

    template<typename C>
    explicit operator vec3d<C>() const
    {
        return vec3d<C>{(C) w, (C) h, (C) l};
    }

    T w, h, l;
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const size3<T> &dt)
{
    os << dt.w << 'x' << dt.h << 'x' << dt.l;

    return os;
}

using isize3 = size3<int>;
using usize3 = size3<unsigned int>;
using fsize3 = size3<float>;
using dsize3 = size3<double>;

template<typename T1, typename T2>
inline size3<T1> operator*(const size3<T1> &lhs, const size3<T2> &rhs)
{
    return size3<T1>{lhs.w * rhs.w, lhs.h * rhs.h, lhs.l * rhs.l};
}

template<typename T1, typename T2>
inline size3<T1> operator/(const size3<T1> &lhs, const size3<T2> &rhs)
{
    return size3<T1>{lhs.w / rhs.w, lhs.h / rhs.h, lhs.l / rhs.l};
}

template<typename T1, typename T2>
inline size3<T1> operator*=(size3<T1> &lhs, const size3<T2> &rhs)
{
    return size3<T1>{lhs.w *= rhs.w, lhs.h *= rhs.h, lhs.l *= rhs.l};
}

template<typename T1, typename T2>
inline size3<T1> operator/=(size3<T1> &lhs, const size3<T2> &rhs)
{
    return size3<T1>{lhs.w /= rhs.w, lhs.h /= rhs.h, lhs.l /= rhs.l};
}

template<typename T1, typename T2>
inline bool operator==(const size3<T1> &lhs, const size3<T1> &rhs)
{
    return lhs.w == rhs.w && lhs.h == rhs.h && lhs.l == rhs.l;
}

template<typename T1, typename T2>
inline bool operator!=(const size3<T1> &lhs, const size3<T1> &rhs)
{
    return lhs.w != rhs.w || lhs.h != rhs.h || lhs.l != rhs.l;
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size3<T1> operator*(const size3<T1> &in, T2 scaler)
{
    return size3<T1>{in.w * (T1) scaler, in.h * (T1) scaler, in.l * (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size3<T1> operator/(const size3<T1> &in, T2 scaler)
{
    return size3<T1>{in.w / (T1) scaler, in.h / (T1) scaler, in.l / (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size3<T1> operator*=(size3<T1> &in, T2 scaler)
{
    return size3<T1>{in.w *= (T1) scaler, in.h *= (T1) scaler, in.l *= (T1) scaler};
}

template<typename T1, typename T2>
requires std::is_arithmetic_v<T2>
inline size3<T1> operator/=(size3<T1> &in, T2 scaler)
{
    return size3<T1>{in.w /= (T1) scaler, in.h /= (T1) scaler, in.l /= (T1) scaler};
}

NS_END
