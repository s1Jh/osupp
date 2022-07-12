#include "BaseHitObject.hpp"

#include "BaseGameMode.hpp"
#include "Math.hpp"
#include "Util.hpp"

NS_BEGIN

bool BaseHitObject::isFinished() const
{ return finished; }

void BaseHitObject::reset()
{
    state = HitObjectState::Invisible;
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

BaseHitObject::BaseHitObject(BaseGameMode &session)
    : finished(false), timeFinished(0.0), session(session), state(HitObjectState::Invisible)
{
    reset();
}

void BaseHitObject::transferApproaching()
{
    state = HitObjectState::Approaching;
    finished = false;
}

void BaseHitObject::transferReady()
{
    state = HitObjectState::Ready;
    finished = false;
}

void BaseHitObject::transferActive()
{
    state = HitObjectState::Active;
    finished = false;
}

void BaseHitObject::transferActiveInactive()
{
    state = HitObjectState::Inactive;
    finished = false;
}

void BaseHitObject::transferInactiveActive()
{
    state = HitObjectState::Active;
    finished = false;
}

void BaseHitObject::transferToPickup()
{
    state = HitObjectState::Pickup;
    finished = true;
    timeFinished = session.getCurrentTime();
}

void BaseHitObject::transferToFading()
{
    state = HitObjectState::Fading;
    finished = true;
}

void BaseHitObject::transferToInvisible()
{
    state = HitObjectState::Invisible;
    finished = false;
    timeFinished = 0.0;
}

void BaseHitObject::transferToInvisibleComplete()
{
    state = HitObjectState::Invisible;
    finished = true;
    timeFinished = session.getCurrentTime();
}

bool BaseHitObject::isApproachCircleDrawn() const
{
    return state == HitObjectState::Approaching || state == HitObjectState::Ready;
}

bool BaseHitObject::isFadingIn() const
{
    return state == HitObjectState::Approaching;
}

bool BaseHitObject::isFadingOut() const
{
    return state == HitObjectState::Pickup || state == HitObjectState::Fading;
}

bool BaseHitObject::isUpdating() const
{
    return state == HitObjectState::Active || state == HitObjectState::Inactive ||
        state == HitObjectState::Ready;
}

bool BaseHitObject::needsApproachCircle() const
{ return true; }

bool BaseHitObject::isActive() const
{ return state == HitObjectState::Active; }

void BaseHitObject::onDraw(Renderer &)
{}

void BaseHitObject::onLogicUpdate(double delta)
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
{ return HitResult::Missed; }

void BaseHitObject::onUpdate(double delta)
{}

fvec2d BaseHitObject::getStartPosition() const
{ return {0, 0}; }

fvec2d BaseHitObject::getEndPosition() const
{ return {0, 0}; }

NS_END