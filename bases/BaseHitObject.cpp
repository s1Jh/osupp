#include "BaseHitObject.hpp"

#include "Util.hpp"
#include "Math.hpp"

namespace GAME_TITLE {

    bool BaseHitObject::isFinished() const {
        return finished;
    }

    void BaseHitObject::reset() {
        state = HitObjectState::Invisible;
        finished = false;
        timeFinished = 0.0;
        this->onReset();
    }

    HitObjectState BaseHitObject::getState() const {
        return state;
    }

    double BaseHitObject::getTimeFinished() const {
        return timeFinished;
    }

    fcircle BaseHitObject::getSOF() const {
        return SOF;
    }

    BaseHitObject::BaseHitObject(BaseGameMode *session) : session(session) { reset(); }

    BaseHitObject::BaseHitObject() { reset(); }

    void BaseHitObject::transferApproaching(double time) {
        state = HitObjectState::Approaching;
        finished = false;
    }

    void BaseHitObject::transferReady(double time) {
        state = HitObjectState::Ready;
        finished = false;
    }

    void BaseHitObject::transferActive(double time) {
        state = HitObjectState::Active;
        finished = false;
    }

    void BaseHitObject::transferActiveInactive(double time) {
        state = HitObjectState::Inactive;
        finished = false;
    }

    void BaseHitObject::transferInactiveActive(double time) {
        state = HitObjectState::Active;
        finished = false;
    }

    void BaseHitObject::transferToPickup(double time) {
        state = HitObjectState::Pickup;
        finished = true;
        timeFinished = time;
    }

    void BaseHitObject::transferToFading(double time) {
        state = HitObjectState::Fading;
        finished = true;
        timeFinished = time;
    }

    void BaseHitObject::transferToInvisible(double time) {
        state = HitObjectState::Invisible;
        finished = false;
        timeFinished = 0.0;
    }

    void BaseHitObject::transferToInvisibleComplete(double time) {
        state = HitObjectState::Invisible;
        finished = true;
        timeFinished = time;
    }

    bool BaseHitObject::isApproachCircleDrawn() const {
        return
            state == HitObjectState::Approaching ||
            state == HitObjectState::Ready;
    }

    bool BaseHitObject::isFadingIn() const {
        return
            state == HitObjectState::Approaching;
    }

    bool BaseHitObject::isFadingOut() const {
        return
            state == HitObjectState::Pickup ||
            state == HitObjectState::Fading;
    }

    bool BaseHitObject::isUpdating() const {
        return
            state == HitObjectState::Active ||
            state == HitObjectState::Inactive ||
            state == HitObjectState::Ready;
    }

    bool BaseHitObject::needsApproachCircle() const {
        return true;
    }

    bool BaseHitObject::isActive() const {
        return state == HitObjectState::Active;
    }

    void BaseHitObject::onDraw(Renderer &) {}

    void BaseHitObject::onUpdate(double delta) {}

    void BaseHitObject::onBegin() {}

    void BaseHitObject::onPress() {}

    void BaseHitObject::onRaise() {}

    void BaseHitObject::onReset() {}

    HitResult BaseHitObject::onFinish() {
        return HitResult::Hit50;
    }
}