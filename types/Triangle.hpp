#pragma once

#include "Vec2.hpp"
#include "Traits.hpp"

#include <type_traits>
#include "define.hpp"

namespace GAME_TITLE {

    template<typename T> requires std::is_arithmetic_v<T>
    struct triangle {
        vec2d <T> vrt[3];
    };

    using itriangle = triangle<int>;
    using utriangle = triangle<unsigned int>;
    using ftriangle = triangle<float>;
    using dtriangle = triangle<double>;

    template<typename T>
    struct IsShape<triangle<T>> {
        static const bool enable = true;
        static const ShapeType type = ShapeType::Triangle;
    };

} // oe
