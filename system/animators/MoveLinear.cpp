#include "MoveLinear.hpp"

#include "Math.hpp"

NS_BEGIN

void MoveLinear::applyTransform(Sprite &spr, double delta)
{
    elapsed += float(delta);

    spr.setPosition(BiLerp(from, to, elapsed / totalTime));

    if (elapsed >= totalTime)
        setFinished();
}

MoveLinear::MoveLinear(Sprite &, const dvec2d &from, const dvec2d &to,
                       float time)
    : totalTime(time), elapsed(0.f), from(from), to(to)
{}

NS_END