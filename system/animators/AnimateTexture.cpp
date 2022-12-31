/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "AnimateTexture.hpp"

#include "Math.hpp"

#include "Rect.hpp"

NS_BEGIN

namespace video
{

void AnimateTexture::applyTransform(
    Sprite &sprite,
    double delta
)
{
    frameTimer += float(delta);

    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        frameCounter++;
        frameCounter %= frameCount;
    }

    fvec2d offset = {layout == AnimationLayout::HORIZONTAL
        ? start.position.x + start.size.w * float(frameCounter)
        : 0,
        layout == AnimationLayout::VERTICAL
            ? start.position.y + start.size.h * float(frameCounter)
            : 0};
    frect newClip = Translate(start, offset);

    sprite.setClipRect(newClip);
}

AnimateTexture::AnimateTexture(
    Sprite &, unsigned int frameRate,
    unsigned int frameCount, frect firstFrame,
    AnimationLayout layout
)
    : layout(layout), start(firstFrame),
      frameTime(math::Max(1.0f / float(frameRate), 0.0f)), frameTimer(0.0f), frameCount(math::Max(frameCount, 0)),
      frameCounter(0)
{}

AnimateTexture::AnimateTexture(
    Sprite &, float frameTime,
    unsigned int frameCount, frect firstFrame,
    AnimationLayout layout
)
    : layout(layout), start(firstFrame),
      frameTime(math::Max(frameTime, 0.0f)), frameTimer(0.0f), frameCount(math::Max(frameCount, 0)),
      frameCounter(0)
{}

AnimateTexture::AnimateTexture(
    Sprite &sprite, unsigned int frameRate,
    AnimationLayout layout
)
    : layout(layout), start(UNIT_RECT<float>),
      frameTime(math::Max(1.0f / float(frameRate), 0.0f)), frameTimer(0.0f), frameCount(0), frameCounter(0)
{
    calculateRectSize(sprite.getTexture()->getSize());
}

AnimateTexture::AnimateTexture(
    Sprite &sprite, float frameTime,
    AnimationLayout layout
)
    : layout(layout), start(UNIT_RECT<float>), frameTime(math::Max(frameTime, 0.0f)),
      frameTimer(0.0f), frameCount(0), frameCounter(0)
{
    calculateRectSize(sprite.getTexture()->getSize());
}

void AnimateTexture::calculateRectSize(const fvec2d &size)
{
    if (layout == AnimationLayout::HORIZONTAL) {
        frameCount =
            int(size.x / math::Max(size.y, std::numeric_limits<float>::epsilon()));
    } else {
        frameCount =
            int(size.y / math::Max(size.x, std::numeric_limits<float>::epsilon()));
    }
    frameCount = math::Max(frameCount, 1);

    auto slice = 1.0f / float(frameCount);
    start = {{
        layout == AnimationLayout::HORIZONTAL ? slice : 1,
        layout == AnimationLayout::HORIZONTAL ? 1 : slice,
    },
        {0, 0}};
}

void AnimateTexture::firstTransform(Sprite &sprite)
{
    applyTransform(sprite, 0.0);
}

}

NS_END