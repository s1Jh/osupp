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