#pragma once

#include "define.hpp"
#include "BaseGameMode.hpp"
#include "StandardResources.hpp"

NS_BEGIN

class Standard : public BaseGameMode {
protected:
    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;
};

NS_END

