#pragma once

#include "Curve.hpp"
#include "Line.hpp"
#include "Vec2.hpp"
#include "define.hpp"
#include <list>

NS_BEGIN

struct SliderNode: public BaseCurveNode
{
    inline SliderNode(fvec2d _position, bool _bonus)
        : position(_position), bonus(_bonus)
    {}

    [[nodiscard]] inline fvec2d getPosition() const override
    { return position; }

    fvec2d position = {0.f, 0.f};
    bool bonus = false;
};

struct ActiveSliderNode: public BaseCurveNode
{
    inline explicit ActiveSliderNode(fvec2d _position)
        : position(_position)
    {}

    [[nodiscard]] inline fvec2d getPosition() const override
    { return position; }

    fvec2d position = {0.f, 0.f};
    bool bonus = false;
    float length = 0.0;
};

using SliderPathT = std::list<SliderNode>;
using ActiveSliderPathT = std::list<ActiveSliderNode>;

NS_END