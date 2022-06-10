#pragma once

#include "define.hpp"
#include "HitObject.tpp"

NS_BEGIN

    BEGIN_OBJECT_TEMPLATE(Spinner)

        float spinResistance = 0.f;
        float spinRequired = 1.f;
    END_OBJECT_TEMPLATE()

}