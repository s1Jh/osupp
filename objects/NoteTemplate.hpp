#pragma once

#include "define.hpp"
#include "BaseObjectTemplate.hpp"
#include "Vec2.hpp"

namespace GAME_TITLE {

    BEGIN_OBJECT_TEMPLATE(Note)
        GLLine connectingLine;
        fvec2d position = {0.f, 0.f};
    END_OBJECT_TEMPLATE()

}