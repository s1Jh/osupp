#pragma once

#include "define.hpp"

#include "Sprite.hpp"
#include "Vec2.hpp"

NS_BEGIN

class MoveLinear: public detail::BaseAnimator
{
public:
    MoveLinear(Sprite &, const dvec2d &from, const dvec2d &to, float time);

    void applyTransform(Sprite &, double delta) override;

private:
    float totalTime;
    float elapsed;
    dvec2d from;
    dvec2d to;
};

NS_END