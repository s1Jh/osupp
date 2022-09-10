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

#include "Circle.hpp"
#include "Color.hpp"
#include "Cuboid.hpp"
#include "Cylinder.hpp"
#include "EnumOperators.hpp"
#include "Line.hpp"
#include "Line3.hpp"
#include "Matrix.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Size.hpp"
#include "Size3.hpp"
#include "Sphere.hpp"
#include "Traits.hpp"
#include "Triangle.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

NS_BEGIN

typedef unsigned int FPS_t;

constexpr FPS_t operator ""_fps(unsigned long long int val)
{ return val; }

enum class AnimationLayout: bool
{
    HORIZONTAL, VERTICAL
};

enum BlendMode
{
    NONE, MULTIPLY, ADD, SUBTRACT
};

namespace detail
{

// Taken from
// https://stackoverflow.com/questions/34672441/stdis-base-of-for-template-classes/34672753#34672753
// Credit to Jarod42 / rmawatson
template<template<typename...> class C, typename... Ts>
std::true_type is_base_of_template_impl(const C<Ts...> *);
template<template<typename...> class C>
std::false_type is_base_of_template_impl(...);
template<typename T, template<typename...> class C>
using is_base_of_template =
decltype(is_base_of_template_impl<C>(std::declval<T *>()));

} // namespace detail

NS_END