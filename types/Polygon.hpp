#pragma once

#include "Line.hpp"
#include "Traits.hpp"

#include <vector>
#include "define.hpp"

NS_BEGIN

template<typename T>
struct polygon {
    template<typename Ty>
    struct _colliding_line : public line<Ty> {
        bool solid;
    };

    std::vector<_colliding_line<T>> verts;
    bool enclosed;
    bool floor_solid;
    bool ceiling_solid;
};

using fpolygon = polygon<float>;
using dpolygon = polygon<double>;
using ipolygon = polygon<int>;
using upolygon = polygon<unsigned int>;

template<typename T>
inline polygon<T> Scale(const polygon<T> &poly, float amount) {
    polygon<T> n;

    for (auto i: poly) {
        n.push_back(i * amount);
    }

    return n;
}

template<typename T>
inline polygon<T> Translate(const polygon<T> &poly, vec2d<float> amount) {
    polygon<T> n;

    for (auto i: poly) {
        n.push_back(i + amount);
    }

    return n;
}

template<typename T>
struct IsShape<polygon<T>> {
    static const bool enable = true;
    static const ShapeType type = ShapeType::Polygon;
};

NS_END
