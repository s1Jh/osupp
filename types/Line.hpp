#pragma once

#include "Traits.hpp"
#include "Util.hpp"
#include "Vec2.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct line
{
    template<typename C>
    inline operator line<C>() const
    {
        return line<C>{(vec2d<C>) A, (vec2d<C>) B};
    }

    vec2d<T> A, B;
};

using fline = line<float>;
using iline = line<int>;
using uline = line<unsigned int>;
using dline = line<double>;

template<typename T>
struct IsShape<line<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::Line;
};

NS_END
