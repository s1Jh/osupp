#pragma once

#include "BaseGameMode.hpp"
#include "define.hpp"


NS_BEGIN

class Standard: public BaseGameMode
{
protected:
    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;
    void onReset() override;
private:
    unsigned long rollingUT = 0.0;
    unsigned long rollingUPF = 0;
    double averageUPF = 1.0f;
    double averageUT = 1.0f;

    unsigned int printCounter = 60;
    ObjectSprite playField;
};

NS_END
