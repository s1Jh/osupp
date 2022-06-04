#pragma once

#include "define.hpp"
#include "Vec2.hpp"
#include "Line.hpp"
#include <list>

namespace GAME_TITLE {

    struct SliderNode {
        fvec2d position = {0.f, 0.f};
        bool bonus = false;
    };

    struct ActiveSliderNode {
        SliderNode* info;
        float length;
        GLLine connectingLine;
    };

    using CurveT = std::list<fvec2d>;
    using SliderPathT = std::list<SliderNode>;
    using ActiveSliderPathT = std::list<ActiveSliderNode>;
}


