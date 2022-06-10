#pragma once

#include "define.hpp"
#include "Vec2.hpp"
#include "Line.hpp"
#include "Curve.hpp"
#include <list>

NS_BEGIN

    struct SliderNode : public BaseCurveNode {
        inline SliderNode(fvec2d _position, bool _bonus) : position(_position), bonus(_bonus) {}

        [[nodiscard]] inline fvec2d getPosition() const override { return position; }

        fvec2d position = {0.f, 0.f};
        bool bonus = false;
    };

    struct ActiveSliderNode : public BaseCurveNode {
        inline explicit ActiveSliderNode(fvec2d _position) : position(_position) {}

        [[nodiscard]] inline fvec2d getPosition() const override { return position; }

        fvec2d position = {0.f, 0.f};
        bool bonus = false;
        float length = 0.0;
        GLLine connectingLine;
    };

    using SliderPathT = std::list<SliderNode>;
    using ActiveSliderPathT = std::list<ActiveSliderNode>;
}


