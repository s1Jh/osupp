#pragma once

#include "BaseGameMode.hpp"
#include "define.hpp"

NS_BEGIN

class Standard: public BaseGameMode
{
public:
    explicit Standard(Game &instance);

protected:
    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;

private:
    NotOSUObjectSprite playField;
};

NS_END
