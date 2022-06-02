#pragma once

#include "Vec3.hpp"
#include "Traits.hpp"

#include <type_traits>
#include "define.hpp"

namespace GAME_TITLE {

    template<typename T> requires std::is_arithmetic_v<T>
    struct line3 {
        template<typename C>
        inline operator line3<C>() {
            return line3<C>{(vec3d<C>) A, (vec3d<C>) B};
        }

        vec3d <T> A, B;
    };

    using fline3 = line3<float>;
    using iline3 = line3<int>;
    using uline3 = line3<unsigned int>;
    using dline3 = line3<double>;


    template<typename T>
    struct IsShape<line3<T>> {
        static const bool enable = true;
        static const ShapeType type = ShapeType::Line3;
    };

}
