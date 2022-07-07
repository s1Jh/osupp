//
// Created by sijh on 12.06.22.
//

#include "Sprite.hpp"
#include "Math.hpp"
#include "Resources.hpp"

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
    : disabled(false), rotation(0.0), pivotPoint({0, 0}), texture(nullptr),
      position(UNIT_RECT<float>), clippingRect(UNIT_RECT<float>), tint(WHITE)
{}

Sprite::Sprite(const TextureP &tex)
    : disabled(false), rotation(0.0), pivotPoint({0, 0}), texture(nullptr),
      position(UNIT_RECT<float>), clippingRect(UNIT_RECT<float>), tint(WHITE)
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

void Sprite::setTexture(const TextureP &tex)
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

TextureP Sprite::getTexture() const
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

NS_END