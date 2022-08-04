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
    Horizontal, Vertical
};

enum BlendMode
{
    None, Multiply, Add, Subtract
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