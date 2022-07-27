#pragma once

#include "HitObject.tpp"
#include "define.hpp"

NS_BEGIN

BEGIN_OBJECT_TEMPLATE(Spinner)
    fvec2d position = {0, 0};
    bool free = false;
    float spinResistance = 0.f;
    float spinRequired = 1.f;
END_OBJECT_TEMPLATE()

NS_END