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

#include "RenderTask.hpp"
#include "Video.hpp"
#include "Texture.hpp"
#include "Types.hpp"
#include "Shader.hpp"
#include "SliderTypes.hpp"

NS_BEGIN

struct ObjectDrawInfo
{
    frect destination = UNIT_RECT<float>;
    float alpha = 1.0f;
    Mat3f transform = MAT3_NO_TRANSFORM<float>;
};

class ObjectSprite
{
    friend void PROJECT_NAMESPACE::Draw<video::LambdaRender &, const ObjectSprite &, const ObjectDrawInfo &>(
        video::LambdaRender &renderer,
        const ObjectSprite &,
        const ObjectDrawInfo &
    );
public:
    ObjectSprite() = default;

    void update(double delta);

    void setTexture(const Resource<video::Texture> &texture);
    void setFPS(FPS_t fps);
    void setTotalFrames(int frames);
    void setFrameTime(float frameTime);
    void setTint(const color &tint);

    [[nodiscard]] const color &getTint() const;

    [[nodiscard]] Resource<video::Texture> getTexture() const;

protected:
    color tint{WHITE};
    Resource<video::Texture> texture{nullptr};
    video::AnimationLayout layout = video::AnimationLayout::HORIZONTAL;
    float frameTime = 0.0f;
    float frameTimer = 0.0f;
    unsigned int frameCount = 1;
    unsigned int frameCounter = 0;
};

using DrawObject = video::RenderTask<const ObjectSprite &, const ObjectDrawInfo &>;

template<>
void Draw(video::LambdaRender &renderer, const ObjectSprite &, const ObjectDrawInfo &);

NS_END
