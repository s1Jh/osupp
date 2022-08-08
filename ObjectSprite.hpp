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

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Types.hpp"

NS_BEGIN

struct ObjectDrawInfo
{
    frect destination = UNIT_RECT<float>;
    float alpha = 1.0f;
    const Mat3f &transform = MAT3_NO_TRANSFORM<float>;
};

class ObjectSprite
{
public:
    BEFRIEND_RENDER_FUNCTOR(ObjectSprite)

    ObjectSprite() = default;

    void update(double delta);

    void setTexture(const TextureP &texture);
    void setFPS(FPS_t fps);
    void setFrameTime(float frameTime);
    void setTint(const color &tint);

    [[nodiscard]] TextureP getTexture() const;

private:
    color tint;
    TextureP texture;
    AnimationLayout layout = AnimationLayout::Horizontal;
    float frameTime = 0.0f;
    float frameTimer = 0.0f;
    unsigned int frameCount = 1;
    unsigned int frameCounter = 0;
};

BEGIN_RENDER_FUNCTOR_DECL(ObjectSprite, const ObjectDrawInfo&)
END_RENDER_FUNCTOR_DECL()

NS_END
