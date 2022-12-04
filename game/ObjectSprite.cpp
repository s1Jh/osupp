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

#include "ObjectSprite.hpp"

#include "Math.hpp"
#include "Renderer.dpp"

NS_BEGIN

void ObjectSprite::update(double delta)
{
    frameTimer += float(delta);

    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        frameCounter++;
        frameCounter %= frameCount;
    }
}

void ObjectSprite::setTexture(const Resource<Texture> &textureIn)
{
    if (textureIn) {
        texture = textureIn;
        frameTimer = 0;
        frameCounter = 0;

        auto longerSide = (float) math::Max(texture->getHeight(), texture->getWidth());
        auto shorterSide = (float) math::Min(texture->getHeight(), texture->getWidth());
        layout = texture->getWidth() >= texture->getHeight()
                 ? AnimationLayout::HORIZONTAL
                 : AnimationLayout::VERTICAL;
        frameCount = math::Max(int(longerSide / shorterSide), 1);

        // advance the animation by one frame upon construction
        // to ensure a proper clip is set up before the texture is used
        update(frameTime);
    }
}

void ObjectSprite::setFPS(FPS_t fps)
{ frameTime = 1.0f / float(fps); }

void ObjectSprite::setFrameTime(float frameTimeIn)
{
    frameTime = frameTimeIn;
}

void ObjectSprite::setTotalFrames(int framesIn)
{
	if (framesIn > 0)
		frameCount = framesIn;
}

void ObjectSprite::setTint(const color &tintIn)
{ tint = tintIn; }

const color &ObjectSprite::getTint() const
{
	return tint;
}

Resource<Texture> ObjectSprite::getTexture() const
{
    return texture;
}

void detail::RenderFunctor<ObjectSprite>::operator()(Renderer &renderer,
                                                     const ObjectSprite &object,
                                                     const ObjectDrawInfo &info)
{
    if (!object.texture)
        return;

    auto slice = 1.0f / float(object.frameCount);
    auto offset = slice * float(object.frameCounter);

    frect clip = {{
                      object.layout == AnimationLayout::HORIZONTAL ? slice : 1.0f,
                      object.layout == AnimationLayout::HORIZONTAL ? 1.0f : slice,
                  },
                  {
                      object.layout == AnimationLayout::HORIZONTAL ? offset : 0.0f,
                      object.layout == AnimationLayout::HORIZONTAL ? 0.0f : offset,
                  }};

    object.texture->setClipArea(clip);

    color tint = object.tint;
    tint.a = info.alpha;

    renderer.draw(
        info.destination,
        VisualAppearance{.texture = object.texture.get(), .fillColor = tint},
        info.transform
    );

    object.texture->setClipArea(UNIT_RECT<float>);
}

BEGIN_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(ObjectSprite)
{

}

NS_END