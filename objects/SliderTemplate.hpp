#pragma once

#include "define.hpp"
#include "BaseObjectTemplate.hpp"
#include "SliderTypes.hpp"
#include "Vec2.hpp"

NS_BEGIN

    BEGIN_OBJECT_TEMPLATE(Slider)
        SliderPathT path;
        CurveType sliderType = CurveType::Straight;
        unsigned int repeats = 0;
    END_OBJECT_TEMPLATE()

}