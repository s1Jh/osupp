#pragma once

#include <cstdint>
#include <type_traits>
#include "define.hpp"

namespace GAME_TITLE {

    enum class ShapeType {
        Polygon, Point, Circle, Rectangle, Line, Triangle,
        Line3, Sphere, Cylinder, Cuboid
    };

    template<typename Sh>
    struct IsShape {

        static const bool enable = false;
        static const ShapeType type;
    };
    template<class T>
    inline constexpr bool IsShapeV = IsShape<T>::enable;
} // oe
