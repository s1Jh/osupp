#pragma once

#include "define.hpp"
#include <type_traits>

NS_BEGIN

enum class ShapeType
{
    Polygon,
    Point,
    Circle,
    Rectangle,
    Line,
    Triangle,
    Line3,
    Sphere,
    Cylinder,
    Cuboid
};

template<typename Sh>
struct IsShape
{

    static const bool enable = false;
    static const ShapeType type;
};

template<class T> inline constexpr bool IsShapeV = IsShape<T>::enable;

NS_END
