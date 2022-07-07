#include "DisableAfter.hpp"

NS_BEGIN

void DisableAfter::applyTransform(Sprite &, double delta)
{
    timer += delta;

    if (timer >= disableTime)
        setFinished();
}

DisableAfter::DisableAfter(Sprite &, float time)
    : disableTime(time), timer(0.f)
{}

void DisableAfter::finalTransform(Sprite &spr)
{ spr.disable(); }

NS_END