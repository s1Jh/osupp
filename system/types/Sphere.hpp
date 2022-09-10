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

#include "Traits.hpp"
#include "Vec3.hpp"

#include "define.hpp"
#include <type_traits>

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct sphere
{
    template<typename C>
    inline operator sphere<C>()
    {
        return sphere<C>{(C) radius, (vec3d<C>) position};
    }

    T radius;
    vec3d <T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const sphere<T> &dt)
{
    os << dt.radius << "@" << dt.position;

    return os;
}

template<typename T>
inline sphere<T> Scale(sphere<T> sphere, float amount)
{
    return {sphere.position, sphere.size * amount};
}

template<typename T>
inline sphere<T> Translate(sphere<T> sphere, vec2d <T> amount)
{
    sphere.position += amount;
    return sphere;
}

using fsphere = sphere<float>;
using isphere = sphere<int>;
using usphere = sphere<unsigned int>;
using dsphere = sphere<double>;

template<typename T>
struct IsShape<sphere<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::SPHERE;
};

NS_END
