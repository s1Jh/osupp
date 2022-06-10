#pragma once

#include <iostream>

#include "Vec2.hpp"
#include "Size.hpp"
#include "Traits.hpp"
#include "define.hpp"

NS_BEGIN

template<typename T>
struct rect {
    rect() = default;

    rect(::GAME_TITLE::size<T> _size, vec2d<T> _pos = {0.f, 0.f}) :
            size(_size), position(_pos) {}

    template<typename C>
    inline operator rect<C>() const {
        return rect<C>{(::GAME_TITLE::size<C>) size, (vec2d<C>) position};
    }

    ::GAME_TITLE::size<T> size = {0, 0};
    vec2d<T> position = {0, 0};
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const rect<T> &dt) {
    os << dt.size << "@" << dt.position;

    return os;
}

template<typename T>
inline rect<T> Scale(rect<T> rect, float amount) {
    return
            {
                    rect.position * amount,
                    rect.size * amount
            };
}

template<typename T>
inline rect<T> Translate(rect<T> rect, vec2d<T> amount) {
    rect.position += amount;
    return rect;
}

using frect = rect<float>;
using irect = rect<int>;
using urect = rect<unsigned int>;
using drect = rect<double>;

template<typename T>
struct IsShape<rect<T>> {
    static const bool enable = true;
    static const ShapeType type = ShapeType::Rectangle;
};

NS_END
