#pragma once

#include "Vec2.hpp"
#include "Traits.hpp"

#include <type_traits>
#include "define.hpp"

NS_BEGIN

template<typename T> requires std::is_arithmetic_v<T>
struct circle {
    circle() = default;

    circle(T _rad, vec2d <T> _pos = {0.f, 0.f}) :
            radius(_rad), position(_pos) {}

    template<typename C>
    inline operator circle<C>() {
        return circle<C>{(C) radius, (vec2d<C>) position};
    }

    T radius;
    vec2d <T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const circle<T> &dt) {
    os << dt.radius << "@" << dt.position;

    return os;
}

template<typename T, typename C>
requires std::is_arithmetic_v<C>
inline circle<T> Scale(circle<T> circle, C amount) {
    return
            {
                    circle.position,
                    circle.size * amount
            };
}

template<typename T>
inline circle<T> Translate(circle<T> circle, vec2d <T> amount) {
    circle.position += amount;
    return circle;
}

using fcircle = circle<float>;
using icircle = circle<int>;
using ucircle = circle<unsigned int>;
using dcircle = circle<double>;

template<typename T>
struct IsShape<circle<T>> {
    static const bool enable = true;
    static const ShapeType type = ShapeType::Circle;
};

NS_END