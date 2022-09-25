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

#include "BaseHitObject.hpp"

#include "GameManager.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Context.hpp"

NS_BEGIN

bool BaseHitObject::isFinished() const
{ return finished; }

void BaseHitObject::reset()
{
    state = HitObjectState::INVISIBLE;
    finished = false;
    timeFinished = 0.0;
    this->onReset();
}

HitObjectState BaseHitObject::getState() const
{ return state; }

double BaseHitObject::getTimeFinished() const
{ return timeFinished; }

fcircle BaseHitObject::getSOF() const
{ return SOF; }

BaseHitObject::BaseHitObject()
    : finished(false), timeStarted(0.0), timeFinished(0.0),
	  state(HitObjectState::INVISIBLE)
{
    reset();
}

void BaseHitObject::transferApproaching()
{
    state = HitObjectState::APPROACHING;
    finished = false;
}

void BaseHitObject::transferReady()
{
    state = HitObjectState::READY;
    finished = false;
}

void BaseHitObject::transferActive()
{
    timeStarted = ctx.game.getCurrentTime();
    state = HitObjectState::ACTIVE;
    finished = false;
}

void BaseHitObject::transferActiveInactive()
{
    state = HitObjectState::INACTIVE;
    finished = false;
}

void BaseHitObject::transferInactiveActive()
{
    state = HitObjectState::ACTIVE;
    finished = false;
}

void BaseHitObject::transferToPickup()
{
    state = HitObjectState::PICKUP;
    finished = true;
    timeFinished = ctx.game.getCurrentTime();
}

void BaseHitObject::transferToFading()
{
    state = HitObjectState::FADING;
    finished = true;
}

void BaseHitObject::transferToInvisible()
{
    state = HitObjectState::INVISIBLE;
    finished = false;
    timeFinished = 0.0;
}

void BaseHitObject::transferToInvisibleComplete()
{
    state = HitObjectState::INVISIBLE;
    finished = true;
    timeFinished = ctx.game.getCurrentTime();
}

bool BaseHitObject::isApproachCircleDrawn() const
{
    return (state == HitObjectState::APPROACHING || state == HitObjectState::READY);
}

bool BaseHitObject::isFadingIn() const
{
    return state == HitObjectState::APPROACHING;
}

bool BaseHitObject::isFadingOut() const
{
    return state == HitObjectState::PICKUP || state == HitObjectState::FADING;
}

bool BaseHitObject::isUpdating() const
{
    return state == HitObjectState::ACTIVE || state == HitObjectState::INACTIVE ||
        state == HitObjectState::READY;
}

bool BaseHitObject::isActive() const
{ return state == HitObjectState::ACTIVE; }

void BaseHitObject::onDraw()
{}

void BaseHitObject::onLogicUpdate(double)
{}

void BaseHitObject::onBegin()
{}

void BaseHitObject::onPress()
{}

void BaseHitObject::onRaise()
{}

void BaseHitObject::onReset()
{}

HitResult BaseHitObject::onFinish()
{ return HitResult::MISSED; }

void BaseHitObject::onUpdate(double)
{}

fvec2d BaseHitObject::getStartPosition() const
{ return {0, 0}; }

fvec2d BaseHitObject::getEndPosition() const
{ return this->getStartPosition(); }

double BaseHitObject::getTimeStarted() const
{
    return timeStarted;
}

HitObjectFunction BaseHitObject::getActivationFunction() const
{
	return HitObjectFunction::NO_ACTIVATION;
}

HitObjectFunction BaseHitObject::getDeactivationFunction() const
{
	return HitObjectFunction::CURSOR_LEAVE | HitObjectFunction::OR | HitObjectFunction::BUTTON_RELEASED;
}


/**
 * Each object will circle through these states in the following order:
 *                                             Inactive -⟍
 *                                               𝛿 ↕      ↓
 *   Invisible -α-> Approaching -β-> Ready ⟍-γ-> Active -𝜀-> Pickup -𝜁-> Fading -η-> Invisible (finished)
 *                                          ⟍______________________↗
 *
 * States: Invisible = Object
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
void BaseHitObject::update(double delta)
{
	objectTransform = this->calculateObjectTransform() * ctx.game.getTransform();

	// gather transfer times
	auto currentTime = ctx.game.getCurrentTime();

	auto alphaTime = getStartTime() - ctx.game.getApproachTime();
	auto betaTime = getStartTime() - ctx.game.getHitWindow();
	auto epsilonTime = getEndTime() + ctx.game.getHitWindow();
	auto etaTime = timeFinished + ctx.game.getFadeTime();

	// update the object
	this->onUpdate(delta);

	// perform all timed state transfers
	if (state == HitObjectState::INVISIBLE && !finished) {
		// check if we've gone past the closer edge of the approach window,
		// α-transition
		if (currentTime >= alphaTime) {
			transferApproaching();
		}
	}
	if (state == HitObjectState::APPROACHING) {
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
	if (state == HitObjectState::FADING) {
		// 𝜁-transition
		if (currentTime >= etaTime) {
			transferToInvisibleComplete();
		}
	}
}

void BaseHitObject::begin()
{
	if (getState() == HitObjectState::READY) {
		transferActive();
		this->onBegin();
	}
}

HitResult BaseHitObject::finish()
{
	// η-transition
	if (getState() == HitObjectState::PICKUP) {
		transferToFading();
		return this->onFinish();
	}
	log::error(this, " tried invalid state change (pickup->finish)");
	return HitResult::MISSED;
}

void BaseHitObject::raise()
{
	if (getState() == HitObjectState::ACTIVE) {
		transferActiveInactive();
		this->onRaise();
	}
}
void BaseHitObject::press()
{
	if (getState() == HitObjectState::INACTIVE) {
		transferInactiveActive();
		this->onPress();
	}
}

void BaseHitObject::draw()
{
	if (getState() != HitObjectState::INVISIBLE)
		this->onDraw();
}

float BaseHitObject::getAlpha() const
{
	if (isFadingIn()) {
		auto x = float(ctx.game.getCurrentTime() - this->getStartTime());
		auto a = x > 0 ? ctx.game.getHitWindow() : ctx.game.getApproachTime();
		return Lerp(0.0f, 1.0f, QuadRR(x, a));
	}
	else if (isFadingOut()) {
		auto x = float(ctx.game.getCurrentTime() - this->getTimeFinished());
		auto a = ctx.game.getFadeTime();
		return Lerp(0.0f, 1.0f, LinearUD(x, a));
	}
	else
		return 1.0f;
}

const Mat3f &BaseHitObject::getObjectTransform() const
{
	return objectTransform;
}

NS_END