#pragma once

#include <iostream>

#include "Vec3.hpp"
#include "Size3.hpp"
#include "Traits.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct cuboid {
    template<typename C>
    inline operator cuboid<C>() {
        return cuboid<C>{(::GAME_TITLE::size3<C>) size, (vec3d<C>) position};
    }

    ::GAME_TITLE::size3<T> size = {0, 0, 0};
    vec3d<T> position = {0, 0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const cuboid<T> &dt) {
    os << dt.size << "@" << dt.position;

    return os;
}

template<typename T>
inline cuboid<T> Scale(cuboid<T> cuboid, float amount) {
    return
            {
                    cuboid.position * amount,
                    cuboid.size * amount
            };
}

template<typename T>
inline cuboid<T> Translate(cuboid<T> cuboid, vec3d<T> amount) {
    cuboid.position += amount;
    return cuboid;
}

using fcuboid = cuboid<float>;
using icuboid = cuboid<int>;
using ucuboid = cuboid<unsigned int>;
using dcuboid = cuboid<double>;

template<typename T>
struct IsShape<cuboid<T>> {
    static const bool enable = true;
    static const ShapeType type = ShapeType::Cuboid;
};

NS_END
