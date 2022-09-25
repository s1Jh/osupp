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
#include <type_traits>

NS_BEGIN

enum class ShapeType
{
    POLYGON, POINT, CIRCLE, RECTANGLE, LINE, TRIANGLE, LINE3, SPHERE, CYLINDER, CUBOID
};

template<typename Sh>
struct IsShape
{

    static const bool enable = false;
    static const ShapeType type;
};

template<class T> inline constexpr bool IsShapeV = IsShape<T>::enable;

namespace detail
{
// Taken from https://stackoverflow.com/questions/44012938/how-to-tell-if-template-type-is-an-instance-of-a-template-class/44013408#44013408
template<class, template<class, class...> class>
struct IsInstanceImpl: public std::false_type
{
};

template<class...Ts, template<class, class...> class U>
struct IsInstanceImpl<U<Ts...>, U>: public std::true_type
{
};
}

template <class A, template <class, class...> class B>
concept IsInstance = requires() {
    detail::IsInstanceImpl<A, B>{};
};

NS_END
