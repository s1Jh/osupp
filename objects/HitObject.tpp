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

#include "AnimateTexture.hpp"
#include "BaseGameMode.hpp"
#include "BaseHitObject.hpp"
#include "BaseObjectTemplate.hpp"
#include "Enum.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Vec2.hpp"
#include "Context.hpp"

#include <memory>
#include <string>

NS_BEGIN
template<typename TemplateT> requires IsTemplateV<TemplateT>
class HitObject: public BaseHitObject
{
public:
    explicit HitObject(std::shared_ptr<TemplateT>, BaseGameMode &);

    [[nodiscard]] double getStartTime() const override;

    [[nodiscard]] double getEndTime() const override;

    void update(double delta) final;

    void draw(Renderer &renderer) final;

    void begin() final;

    void raise() final;

    HitResult finish() final;

    void press() final;

protected:
    void onReset() override;

    [[nodiscard]] float getAlpha() const override;

    ObjectSprite approachCircle;
    std::shared_ptr<TemplateT> objectTemplate;
};

template<typename TemplateT>
requires IsTemplateV<TemplateT>
HitObject<TemplateT>::HitObject(std::shared_ptr<TemplateT> trackedTemplate,
                                BaseGameMode &activeSession)
    : BaseHitObject(activeSession), objectTemplate(trackedTemplate)
{

    approachCircle =
        GetContext().activeSkin->createObjectSprite(APPROACH_CIRCLE_SPRITE);
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
double HitObject<TemplateT>::getStartTime() const
{
    return objectTemplate->startTime;
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
double HitObject<TemplateT>::getEndTime() const
{
    return objectTemplate->endTime;
}

/**
 * Each object will circle through these states in the following order:
 *                                             Inactive -⟍
 *                                               𝛿 ↕      ↓
 *   Invisible -α-> Approaching -β-> Ready ⟍-γ-> Active -𝜀-> Pickup -𝜁-> Fading
 * -η-> Invisible (finished) ⟍______________________↗ States: Invisible = Object
 * is not currently visible on the screen and thus cannot be interacted with.
 *               Overridden onLogicUpdate method is not called during this
 * state. Approaching = Object became visible, but cannot yet be interacted
 * with. Ready = Object is visible and can be interacted with. Active = Object
 * is being interacted with and points are being gained. Inactive = Object has
 * been interacted with once, but is not being held right now. Fading = Object
 * is fading away. Pickup = Invisible = Object is again invisible, it has been
 * passed.
 *
 * Transitions:
 *   α - [TIMED] currentTime >= startTime - approachTime.
 *   β - [TIMED] currentTime >= startTime - hitWindow.
 *   γ - [PLAYER] Player clicking down for the first time (initial hit), begin()
 * called. 𝛿 - [PLAYER] Player letting go / pressing down on the object,
 * press(), raise() called respectively. 𝜀 - [TIMED] currentTime >= endTime. 𝜁 -
 * [UPDATE] currentTime >= endTime + hitWindow (fading time). η - [TIMED]
 * finish() called to get the object rank.
 */
template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::update(double delta)
{
    // update the visuals
    approachCircle.update(delta);

    // gather transfer times
    auto state = getState();
    auto currentTime = session.getCurrentTime();

    auto alphaTime = getStartTime() - session.getApproachTime();
    auto betaTime = getStartTime() - session.getHitWindow();
    auto epsilonTime = getEndTime() + session.getHitWindow();
    auto etaTime = timeFinished + session.getFadeTime();

    // update the object
    this->onUpdate(delta);

    // perform all timed state transfers
    if (state == HitObjectState::Invisible && !finished) {
        // check if we've gone past the closer edge of the approach window,
        // α-transition
        if (currentTime >= alphaTime) {
            transferApproaching();
        }
    }
    if (state == HitObjectState::Approaching) {
        // β-transition
        if (currentTime >= betaTime) {
            // We're now in the hit window, we are ready to be hit
            transferReady();
        }
    }
    if (isUpdating()) {
        // update the object's functions
        this->onLogicUpdate(delta);

        // 𝜀-transition
        if (currentTime >= epsilonTime) {
            // We've gone past the object's hit window and are still in the ready
            // state (ie. not hit), consider this object missed.
            transferToPickup();
        }
    }
    if (state == HitObjectState::Fading) {
        // 𝜁-transition
        if (currentTime >= etaTime) {
            transferToInvisibleComplete();
        }
    }
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::begin()
{
    if (getState() == HitObjectState::Ready) {
        transferActive();
        this->onBegin();
    }
}

template<typename TemplateT>
requires IsTemplateV<TemplateT> HitResult HitObject<TemplateT>::finish()
{
    // η-transition
    if (getState() == HitObjectState::Pickup) {
        transferToFading();
        return this->onFinish();
    }
    log::error(this, " tried invalid state change (pickup->finish)");
    return HitResult::Missed;
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::raise()
{
    if (getState() == HitObjectState::Active) {
        transferActiveInactive();
        this->onRaise();
    }
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::press()
{
    if (getState() == HitObjectState::Inactive) {
        transferInactiveActive();
        this->onPress();
    }
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::draw(Renderer &renderer)
{
    const auto &objectTransform = session.getObjectTransform();

    if (isApproachCircleDrawn() && this->needsApproachCircle()) {

        auto scale = 4.0f;  // how big will the circle be at -ar
        float offset = 1.0; // how big the circle will be at 0

        float slope = (scale - offset) / session.getApproachTime();

        float x = session.getCurrentTime() - getStartTime();
        float y = x * -slope + offset;

        float acSize = session.getCircleSize() * y;
        acSize = Max(acSize, 0.0);

        renderer.draw(approachCircle,
                      ObjectDrawInfo{{{acSize, acSize}, SOF.position}, getAlpha(), objectTransform});
    }

    this->onDraw(renderer);
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
float HitObject<TemplateT>::getAlpha() const
{
    if (isFadingIn()) {
        float x = session.getCurrentTime() - getStartTime();
        float a = x > 0 ? session.getHitWindow()
                        : session.getApproachTime();
        return Lerp(0.0f, 1.0f, QuadRR(x, a));
    }
    else if (isFadingOut()) {
        float x = session.getCurrentTime() - getTimeFinished();
        float a = session.getFadeTime();
        return Lerp(0.0f, 1.0f, QuadRR(x, a));
    }
    else
        return 1.0f;
}

template<typename TemplateT>
requires IsTemplateV<TemplateT>
void HitObject<TemplateT>::onReset()
{
    auto &pool = GetContext().resources;
    approachCircle.setTexture(pool.get<Texture>(APPROACH_CIRCLE_SPRITE));
}
NS_END