#pragma once

#include "Vec3.hpp"
#include "Traits.hpp"

#include <type_traits>
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct sphere {
    template<typename C>
    inline operator sphere<C>() {
        return sphere<C>{(C) radius, (vec3d<C>) position};
    }

    T radius;
    vec3d <T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const sphere<T> &dt) {
    os << dt.radius << "@" << dt.position;

    return os;
}

template<typename T>
inline sphere<T> Scale(sphere<T> sphere, float amount) {
    return
            {
                    sphere.position,
                    sphere.size * amount
            };
}

template<typename T>
inline sphere<T> Translate(sphere<T> sphere, vec2d <T> amount) {
    sphere.position += amount;
    return sphere;
}

using fsphere = sphere<float>;
using isphere = sphere<int>;
using usphere = sphere<unsigned int>;
using dsphere = sphere<double>;

template<typename T>
struct IsShape<sphere<T>> {
    static const bool enable = true;
    static const ShapeType type = ShapeType::Sphere;
};

NS_END
