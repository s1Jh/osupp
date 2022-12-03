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

//
// Created by sijh on 12.06.22.
//

#include "Sprite.hpp"
#include "Math.hpp"
#include "Resource.hpp"
#include "Drawables.hpp"

#include <algorithm>

NS_BEGIN

namespace detail
{
void BaseAnimator::applyTransform(Sprite &, [[maybe_unused]] double delta)
{}

void BaseAnimator::finalTransform(Sprite &)
{}

void BaseAnimator::firstTransform(Sprite &)
{}

bool BaseAnimator::isDone()
{ return finished; }

void BaseAnimator::setFinished()
{ finished = true; }
} // namespace detail

Sprite::Sprite()
    : tint(WHITE), disabled(false), rotation(0.0), pivotPoint({0, 0}),
      position(UNIT_RECT<float>), clippingRect(UNIT_RECT<float>), texture(nullptr)
{}

Sprite::Sprite(const Resource<Texture> &tex)
    : tint(WHITE), disabled(false), rotation(0.0), pivotPoint({0, 0}),
      position(UNIT_RECT<float>), clippingRect(UNIT_RECT<float>), texture(nullptr)
{
    setTexture(tex);
}

void Sprite::clearAnimators()
{ animators.clear(); }

void Sprite::update(double delta)
{
    for (auto &animator: animators) {
        animator->applyTransform(*this, delta);
    }

    if (!animators.empty())
        animators.erase(
            std::remove_if(animators.begin(), animators.end(),
                           [&](const std::shared_ptr<detail::BaseAnimator> &ptr)
                           {
                               if (ptr->isDone()) {
                                   ptr->finalTransform(*this);
                                   return true;
                               }
                               return false;
                           }),
            animators.end());
}

void Sprite::setTexture(const Resource<Texture> &tex)
{
    texture = tex;
    clippingRect = UNIT_RECT<float>;
}

void Sprite::setClipRectSize(const dsize &n)
{ clippingRect.size = n; }

void Sprite::setClipRectPosition(const dvec2d &n)
{ clippingRect.position = n; }

void Sprite::setClipRect(const drect &n)
{ clippingRect = n; }

void Sprite::setPosition(dvec2d n)
{ position.position = n; }

void Sprite::setSize(dsize n)
{ position.size = n; }

void Sprite::setRotation(float n)
{ rotation = n; }

void Sprite::setScale(dvec2d n)
{
    position.size = texture->getSize() * isize(n);
}

void Sprite::setScale(double n)
{
    position.size.w *= n;
    position.size.h *= n;
}

void Sprite::enable()
{ disabled = false; }

void Sprite::disable()
{ disabled = true; }

dsize Sprite::getClipRectSize() const
{ return clippingRect.size; }

dvec2d Sprite::getClipRectPosition() const
{ return clippingRect.position; }

drect Sprite::getClipRect() const
{ return clippingRect; }

Resource<Texture> Sprite::getTexture() const
{ return texture; }

dvec2d Sprite::getPosition() const
{ return position.position; }

dsize Sprite::getSize() const
{
    if (!disabled)
        return position.size;
    else
        return {0, 0};
}

dvec2d Sprite::getScale() const
{
    if (!disabled)
        return position.size / texture->getSize();
    else
        return {0, 0};
}

float Sprite::getRotation() const
{ return rotation; }

bool Sprite::isDisabled() const
{ return disabled; }

const color &Sprite::getTint() const
{ return tint; }

void Sprite::setTint(const color &tintIn)
{ tint = tintIn; }

const fvec2d &Sprite::getPivotPoint() const
{ return pivotPoint; }

void Sprite::setPivotPoint(const fvec2d &pivotPointIn)
{
    pivotPoint = pivotPointIn;
}

void Sprite::setRect(const drect &newRect)
{
    position = newRect;
}

const drect &Sprite::getRect() const
{
    return position;
}

BEGIN_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(Sprite)
{
//    shader.load("sprite.shader");
}

void detail::RenderFunctor<Sprite>::operator()(Renderer &renderer, const Sprite &sprite, const Mat3f &transform)
{
	// FIXME: actually use the sprite shader?

	//    const auto &spriteShader = getData().shader;

    Mat3f shape =
        math::MakeScaleMatrix<float>(sprite.getSize()) *
            math::MakeRotationMatrix<float>(sprite.getRotation(), sprite.getPivotPoint()) *
            math::MakeTranslationMatrix<float>(sprite.getPosition());
    Mat3f texture = math::MakeScaleMatrix<float>(sprite.getClipRectSize()) *
        math::MakeTranslationMatrix<float>(sprite.getClipRectPosition());

//    renderer.draw(sprite.getRect(), VisualAppearance{
//        .texture = sprite.getTexture().get(),
//        .uvTransform = &texture,
//        .fillColor = sprite.getTint()
//    }, transform * shape);
}

NS_END