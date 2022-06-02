#pragma once

#include "define.hpp"
#include "BaseObjectTemplate.hpp"
#include "SliderTypes.h"
#include "Vec2.hpp"

namespace GAME_TITLE {

    BEGIN_OBJECT_TEMPLATE(Slider)

        SliderPathT path;
        SliderType sliderType = SliderType::Straight;
    END_OBJECT_TEMPLATE()

}