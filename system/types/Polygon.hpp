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

#include "Line.hpp"
#include "Traits.hpp"

#include "define.hpp"
#include <vector>

NS_BEGIN

template<typename T>
struct polygon
{
    template<typename Ty>
    struct _colliding_line: public line<Ty>
    {
        bool solid;
    };

    std::vector<_colliding_line<T>> verts;
    bool enclosed;
    bool floor_solid;
    bool ceiling_solid;
};

using fpolygon = polygon<float>;
using dpolygon = polygon<double>;
using ipolygon = polygon<int>;
using upolygon = polygon<unsigned int>;

template<typename T>
inline polygon<T> Scale(const polygon<T> &poly, float amount)
{
    polygon<T> n;

    for (auto i: poly) {
        n.push_back(i * amount);
    }

    return n;
}

template<typename T>
inline polygon<T> Translate(const polygon<T> &poly, vec2d<float> amount)
{
    polygon<T> n;

    for (auto i: poly) {
        n.push_back(i + amount);
    }

    return n;
}

template<typename T>
struct IsShape<polygon<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::Polygon;
};

NS_END
