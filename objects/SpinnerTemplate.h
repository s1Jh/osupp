#pragma once

#include "define.hpp"
#include "HitObject.tpp"

namespace GAME_TITLE {

    BEGIN_OBJECT_TEMPLATE(Spinner)

        float spinResistance = 0.f;
        float spinRequired = 1.f;
    END_OBJECT_TEMPLATE()

}