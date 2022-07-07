#pragma once

#include "Sprite.hpp"

NS_BEGIN

class DisableAfter: public detail::BaseAnimator
{
public:
    explicit DisableAfter(Sprite &, float time);

    void finalTransform(Sprite &) override;

    void applyTransform(Sprite &, double delta) override;

private:
    float disableTime;
    float timer;
};

NS_END