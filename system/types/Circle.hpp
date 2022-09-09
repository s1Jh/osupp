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

#include "Rect.hpp"
#include "Traits.hpp"
#include "Vec2.hpp"

#include "define.hpp"
#include <type_traits>

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct circle
{
    circle() = default;

    circle(T _rad, vec2d <T> _pos = {0.f, 0.f})
        : radius(_rad), position(_pos)
    {}

    template<typename C>
    inline operator circle<C>() const
    {
        return circle<C>{(C) radius, (vec2d<C>) position};
    }

    template<typename C>
    inline operator rect<C>() const
    {
        return rect<C>{{(C) radius, (C) radius}, (vec2d<C>) position};
    }

    T radius;
    vec2d <T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const circle<T> &dt)
{
    os << dt.radius << "@" << dt.position;

    return os;
}

template<typename T, typename C>
requires std::is_arithmetic_v<C>
inline circle<T> Scale(circle<T> circle, C amount)
{
    return {circle.position, circle.size * amount};
}

template<typename T>
inline circle<T> Translate(circle<T> circle, vec2d <T> amount)
{
    circle.position += amount;
    return circle;
}

using fcircle = circle<float>;
using icircle = circle<int>;
using ucircle = circle<unsigned int>;
using dcircle = circle<double>;

template<typename T>
struct IsShape<circle<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::CIRCLE;
};

NS_END