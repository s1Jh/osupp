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

#include "Size3.hpp"
#include "Traits.hpp"
#include "Vec3.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct cuboid
{
    template<typename C>
    inline operator cuboid<C>()
    {
        return cuboid<C>{(::PROJECT_NAMESPACE::size3<C>) size, (vec3d<C>) position};
    }

    ::PROJECT_NAMESPACE::size3<T> size = {0, 0, 0};
    vec3d<T> position = {0, 0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const cuboid<T> &dt)
{
    os << dt.size << "@" << dt.position;

    return os;
}

template<typename T>
inline cuboid<T> Scale(cuboid<T> cuboid, float amount)
{
    return {cuboid.position * amount, cuboid.size * amount};
}

template<typename T>
inline cuboid<T> Translate(cuboid<T> cuboid, vec3d<T> amount)
{
    cuboid.position += amount;
    return cuboid;
}

using fcuboid = cuboid<float>;
using icuboid = cuboid<int>;
using ucuboid = cuboid<unsigned int>;
using dcuboid = cuboid<double>;

template<typename T>
struct IsShape<cuboid<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::Cuboid;
};

NS_END
