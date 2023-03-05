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

#include "define.hpp"

#include "Size.hpp"
#include "Vector.hpp"
#include "ToFromString.hpp"
#include "Traits.hpp"

namespace PROJECT_NAMESPACE
{

template<typename T>
    requires std::is_arithmetic_v<T>
struct rect
{
    constexpr rect()
        : size({1, 1}), position({0, 0})
    {}

    constexpr rect(::PROJECT_NAMESPACE::size<T> _size, vec2d<T> _pos = {0, 0})
        : size(_size), position(_pos)
    {}

    template<typename C>
    inline operator rect<C>() const
    {
        return rect<C>{(::PROJECT_NAMESPACE::size<C>) size, (vec2d<C>) position};
    }

    ::PROJECT_NAMESPACE::size<T> size = {0, 0};
    vec2d<T> position = {0, 0};
};

template<typename T> constexpr rect<T> UNIT_RECT{};
template<typename T> constexpr rect<T> SCREEN_RECT{{2, 2}, {0, 0}};


template<typename T>
inline rect<T> Scale(rect<T> rect, float amount)
{
    return {rect.position * amount, rect.size * amount};
}

template<typename T>
inline rect<T> Translate(rect<T> rect, vec2d<T> amount)
{
    rect.position += amount;
    return rect;
}

using frect = rect<float>;
using irect = rect<int>;
using urect = rect<unsigned int>;
using drect = rect<double>;

template<typename T> requires
    is_specialization<T, rect>::value
std::string ToString(const T& rect)
{
    return ToString(rect.size) + '@' + ToString(rect.position);
}

}
