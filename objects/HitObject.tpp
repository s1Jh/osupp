#pragma once

#include "define.hpp"
#include "Vec2.hpp"
#include "Enum.hpp"
#include "BaseObjectTemplate.hpp"
#include "Util.hpp"
#include "BaseGameMode.hpp"
#include "BaseHitObject.hpp"
#include "Math.hpp"
#include "StandardResources.hpp"

#include <string>
#include <memory>

namespace GAME_TITLE {
    template<typename TemplateT> requires IsTemplateV<TemplateT>
    class HitObject : public BaseHitObject {
    public:
        explicit HitObject(std::shared_ptr<TemplateT>, BaseGameMode *);

        [[nodiscard]] double getStartTime() const override;

        [[nodiscard]] double getEndTime() const override;

        void update(double delta) override;

        void draw(Renderer &renderer) override;

        void begin() override;

        void raise() override;

        HitResult finish() override;

        void press() override;

    protected:
        [[nodiscard]] float getAlpha() const override;

        std::shared_ptr<TemplateT> objectTemplate;
    };

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    HitObject<TemplateT>::HitObject(std::shared_ptr<TemplateT> trackedTemplate, BaseGameMode *activeSession) :
            BaseHitObject(activeSession), objectTemplate(trackedTemplate) {
        session = activeSession;
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    double HitObject<TemplateT>::getStartTime() const {
        return objectTemplate->startTime;
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    double HitObject<TemplateT>::getEndTime() const {
        return objectTemplate->endTime;
    }

    /**
     * Each object will circle through these states in the following order:
     *                                             Inactive -⟍
     *                                               𝛿 ↕      ↓
     *   Invisible -α-> Approaching -β-> Ready ⟍-γ-> Active -𝜀-> Pickup -𝜁-> Fading -η-> Invisible (finished)
     *                                           ⟍______________________↗
     * States:
     *   Invisible = Object is not currently visible on the screen and thus cannot be interacted with.
     *               Overridden onUpdate method is not called during this state.
     *   Approaching = Object became visible, but cannot yet be interacted with.
     *   Ready = Object is visible and can be interacted with.
     *   Active = Object is being interacted with and points are being gained.
     *   Inactive = Object has been interacted with once, but is not being held right now.
     *   Fading = Object is fading away.
     *   Pickup =
     *   Invisible = Object is again invisible, it has been passed.
     *
     * Transitions:
     *   α - [TIMED] currentTime >= startTime - approachTime.
     *   β - [TIMED] currentTime >= startTime - hitWindow.
     *   γ - [PLAYER] Player clicking down for the first time (initial hit), begin() called.
     *   𝛿 - [PLAYER] Player letting go / pressing down on the object, press(), raise() called respectively.
     *   𝜀 - [TIMED] currentTime >= endTime.
     *   𝜁 - [UPDATE] currentTime >= endTime + hitWindow (fading time).
     *   η - [TIMED] finish() called to get the object rank.
     */
    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    void HitObject<TemplateT>::update(double delta) {
        auto state = getState();
        auto currentTime = session->getCurrentTime();

        auto alphaTime = getStartTime() - session->getMap()->getApproachTime();
        auto betaTime = getStartTime() - session->getMap()->getHitWindow();
        auto epsilonTime = getEndTime() + session->getMap()->getHitWindow();
        auto etaTime = timeFinished + session->getMap()->getFadeTime();

        if (state == HitObjectState::Invisible && !finished) {
            // check if we've gone past the closer edge of the approach window, α-transition
            if (session->getCurrentTime() >= alphaTime) {
                transferApproaching(currentTime);
            }
        }
        if (state == HitObjectState::Approaching) {
            // β-transition
            if (session->getCurrentTime() >= betaTime) {
                // We're now in the hit window, we are ready to be hit
                transferReady(currentTime);
            }
        }
        if (isUpdating()) {
            // update the object's functions
            this->onUpdate(delta);

            // 𝜀-transition
            if (session->getCurrentTime() >= epsilonTime) {
                // We've gone past the object's hit window and are still in the ready state (ie. not hit),
                // consider this object missed.
                transferToPickup(currentTime);
            }
        }
        if (state == HitObjectState::Fading) {
            // 𝜁-transition
            if (session->getCurrentTime() >= etaTime) {
                transferToInvisibleComplete(currentTime);
            }
        }
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    void HitObject<TemplateT>::begin() {
        if (getState() == HitObjectState::Ready) {
            transferActive(session->getCurrentTime());
            this->onBegin();
        }
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    HitResult HitObject<TemplateT>::finish() {
        // η-transition
        if (getState() == HitObjectState::Pickup) {
            transferToFading(session->getCurrentTime());
            return this->onFinish();
        }
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    void HitObject<TemplateT>::raise() {
        if (getState() == HitObjectState::Active) {
            transferActiveInactive(session->getCurrentTime());
            this->onRaise();
        }
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    void HitObject<TemplateT>::press() {
        if (getState() == HitObjectState::Inactive) {
            transferInactiveActive(session->getCurrentTime());
            this->onPress();
        }
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    void HitObject<TemplateT>::draw(Renderer &renderer) {
        const auto objectTransform = session->getObjectTransform();

        if (isApproachCircleDrawn() && this->needsApproachCircle()) {
            auto acCircleScale = 1.5f;
            float x = session->getCurrentTime() - getStartTime();
            float a = x > 0 ? session->getMap()->getHitWindow() : session->getMap()->getApproachTime();
            float acSize = Lerp(session->getMap()->getCircleSize() * acCircleScale, 0, LinearES(x, a));
            acSize = Max(acSize, 0.0);
            renderer.drawRect(
                    { { acSize, acSize }, SOF.position },
                    {
                        .texture = &StandardResources::ApproachCircle,
                        .blendMode = BlendMode::Multiply,
                        .fillColor = WHITE
                    }, objectTransform);

        }

        this->onDraw(renderer);

        color col = WHITE;
        auto alpha = getAlpha();
        auto circleSize = SOF.radius;

        col.a = alpha;
        renderer.drawRect(
                { { circleSize, circleSize }, SOF.position },
                {
                        .texture = &StandardResources::NoteBase,
                        .blendMode = BlendMode::Multiply,
                         .fillColor = col
                }, objectTransform);

        switch (getState()) {
            case HitObjectState::Approaching: col = MAGENTA; break;
            case HitObjectState::Ready: col = GREEN; break;
            case HitObjectState::Active: col = LIME; break;
            case HitObjectState::Inactive: col = RED; break;
            case HitObjectState::Pickup:
            case HitObjectState::Fading: col = GRAY; break;
            case HitObjectState::Invisible: col = PERSIAN_BLUE; break;
        }
        col.a = alpha;
        renderer.drawRect(
                { { circleSize, circleSize }, SOF.position },
                {
                        .texture = &StandardResources::NoteOverlay,
                        .blendMode = BlendMode::Multiply,
                        .fillColor = col
                }, objectTransform);
    }

    template<typename TemplateT>
    requires IsTemplateV<TemplateT>
    float HitObject<TemplateT>::getAlpha() const {
        if (isFadingIn()) {
            float x = session->getCurrentTime() - getStartTime();
            float a = x > 0 ? session->getMap()->getHitWindow() : session->getMap()->getApproachTime();
            return Lerp(0.0f, 1.0f, QuadRR(x, a));
        } else if (isFadingOut()) {
            float x = session->getCurrentTime() - getTimeFinished();
            float a = session->getMap()->getFadeTime();
            return Lerp(0.0f, 1.0f, QuadRR(x, a));
        } else
            return 1.0f;
    }
}