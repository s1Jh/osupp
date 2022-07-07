#include "AnimateTexture.hpp"

#include "Math.hpp"

#include "Rect.hpp"

NS_BEGIN

void AnimateTexture::applyTransform(org::sijh::NotOSU::Sprite &sprite,
                                    double delta)
{
    frameTimer += float(delta);

    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        frameCounter++;
        frameCounter %= frameCount;
    }

    fvec2d offset = {layout == AnimationLayout::Horizontal
                     ? start.position.x + start.size.w * float(frameCounter)
                     : 0,
                     layout == AnimationLayout::Vertical
                     ? start.position.y + start.size.h * float(frameCounter)
                     : 0};
    frect newClip = Translate(start, offset);

    sprite.setClipRect(newClip);
}

AnimateTexture::AnimateTexture(Sprite &, unsigned int frameRate,
                               unsigned int frameCount, frect firstFrame,
                               AnimationLayout layout)
    : layout(layout), start(firstFrame),
      frameTime(Max(1.0f / float(frameRate), 0.0f)), frameTimer(0.0f), frameCount(Max(frameCount, 0)),
      frameCounter(0)
{}

AnimateTexture::AnimateTexture(Sprite &, float frameTime,
                               unsigned int frameCount, frect firstFrame,
                               AnimationLayout layout)
    : layout(layout), start(firstFrame),
      frameTime(Max(frameTime, 0.0f)), frameTimer(0.0f), frameCount(Max(frameCount, 0)),
      frameCounter(0)
{}

AnimateTexture::AnimateTexture(Sprite &sprite, unsigned int frameRate,
                               AnimationLayout layout)
    : layout(layout), start(UNIT_RECT<float>),
      frameTime(Max(1.0f / float(frameRate), 0.0f)), frameTimer(0.0f), frameCount(0), frameCounter(0)
{
    calculateRectSize(sprite.getTexture()->getSize());
}

AnimateTexture::AnimateTexture(Sprite &sprite, float frameTime,
                               AnimationLayout layout)
    : layout(layout), start(UNIT_RECT<float>), frameTime(Max(frameTime, 0.0f)),
      frameTimer(0.0f), frameCount(0), frameCounter(0)
{
    calculateRectSize(sprite.getTexture()->getSize());
}

void AnimateTexture::calculateRectSize(const fvec2d &size)
{
    if (layout == AnimationLayout::Horizontal) {
        frameCount =
            int(size.x / Max(size.y, std::numeric_limits<float>::epsilon()));
    }
    else {
        frameCount =
            int(size.y / Max(size.x, std::numeric_limits<float>::epsilon()));
    }
    frameCount = Max(frameCount, 1);

    auto slice = 1.0f / float(frameCount);
    start = {{
                 layout == AnimationLayout::Horizontal ? slice : 1,
                 layout == AnimationLayout::Horizontal ? 1 : slice,
             },
             {0, 0}};
}

void AnimateTexture::firstTransform(Sprite &sprite)
{
    applyTransform(sprite, 0.0);
}

NS_END