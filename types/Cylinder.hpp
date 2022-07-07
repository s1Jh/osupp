#pragma once

#include "Traits.hpp"
#include "Vec3.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct cylinder
{
    template<typename C>
    inline operator cylinder<C>()
    {
        return cylinder<C>{(C) radius, (C) height, (vec3d<C>) position};
    }

    T radius;
    T height;
    vec3d <T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const cylinder<T> &dt)
{
    os << dt.radius << "x" << dt.height << "@" << dt.position;

    return os;
}

template<typename T>
inline cylinder<T> Translate(cylinder<T> cylinder, vec3d <T> amount)
{
    cylinder.position += amount;
    return cylinder;
}

using fcylinder = cylinder<float>;
using icylinder = cylinder<int>;
using ucylinder = cylinder<unsigned int>;
using dcylinder = cylinder<double>;

template<typename T>
struct IsShape<cylinder<T>>
{
    static const bool enable = true;
    static const ShapeType type = ShapeType::Cylinder;
};
}
