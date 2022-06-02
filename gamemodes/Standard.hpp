#pragma once

#include "define.hpp"
#include "BaseGameMode.hpp"
#include "StandardResources.hpp"

namespace GAME_TITLE {

    class Standard : public BaseGameMode {
    protected:
        void onUpdate(double delta) override;

        void onDraw(Renderer &renderer) override;
    };

}

