#pragma once

#include "define.hpp"

#include "Sprite.hpp"

#include "Types.hpp"

NS_BEGIN

class AnimateTexture: public detail::BaseAnimator
{
public:
    AnimateTexture(Sprite &, FPS_t frameRate,
                   AnimationLayout layout = AnimationLayout::Horizontal);
    AnimateTexture(Sprite &, float frameTime,
                   AnimationLayout layout = AnimationLayout::Horizontal);

    AnimateTexture(Sprite &, FPS_t frameRate, unsigned int frameCount,
                   frect firstFrame,
                   AnimationLayout layout = AnimationLayout::Horizontal);

    AnimateTexture(Sprite &, float frameTime, unsigned int frameCount,
                   frect firstFrame,
                   AnimationLayout layout = AnimationLayout::Horizontal);

    void applyTransform(Sprite &sprite, double delta) override;

    void firstTransform(Sprite &sprite) override;

private:
    void calculateRectSize(const fvec2d &size);

    Mat3f UVTransform;

    AnimationLayout layout;
    frect start;
    float frameTime;
    float frameTimer;
    unsigned int frameCount;
    unsigned int frameCounter;
};

NS_END