#pragma once

#include "define.hpp"
#include "Circle.hpp"
#include "Enum.hpp"
#include "Renderer.hpp"

namespace GAME_TITLE {
    class BaseGameMode;
/**
 * The following class encompasses all logic surrounding hit objects.
 *
 * Each object holds a startTime floating point value which represents in seconds
 * the startTime at which the object should be hit for a perfect score.
 *
 * Upon completion, the startTime value changes to the startTime at which the object has been completed.
 *
 * The object holds a bool finished variable that gets set to true the moment
 * an object is considered to be done. This can occur either when a player successfully
 * hits the object, or misses it entirely.
 *
 * A update() method is called each frame when the object is in a active state.
 *
 * When a player first interacts with a hit object, the begin() method is called.
 * This will transfer the object into the Active state.
 * This also sets the startTime at which the player has began holding down on an object.
 *
 * When the player raises their hit button, or when the player's cursor leaves
 * the object's circle of influence, the raise() method is called.
 *
 * Once the object enters the fading state, the finish() method will be called.
 * This method will return how well the object has been hit.
 **/
    class BaseHitObject {
    public:
        BaseHitObject();

        virtual void update(double delta) = 0;

        virtual void begin() = 0;

        virtual void press() = 0;

        virtual void raise() = 0;

        virtual void draw(Renderer&) = 0;

        [[nodiscard]] virtual double getStartTime() const = 0;

        [[nodiscard]] virtual double getEndTime() const = 0;

        [[nodiscard]] virtual HitResult finish() = 0;

        [[nodiscard]] fcircle getSOF() const;

        [[nodiscard]] bool isFinished() const;

        [[nodiscard]] HitObjectState getState() const;

        [[nodiscard]] double getTimeFinished() const;

        void reset();

    protected:
        explicit BaseHitObject(BaseGameMode *session);

        void transferToInvisible(double time);
        void transferApproaching(double time);
        void transferReady(double time);
        void transferActive(double time);
        void transferActiveInactive(double time);
        void transferInactiveActive(double time);
        void transferToPickup(double time);
        void transferToFading(double time);
        void transferToInvisibleComplete(double time);

        [[nodiscard]] bool isApproachCircleDrawn() const;
        [[nodiscard]] bool isFadingIn() const;
        [[nodiscard]] bool isFadingOut() const;
        [[nodiscard]] bool isUpdating() const;
        [[nodiscard]] bool isActive() const;
        [[nodiscard]] virtual float getAlpha() const = 0;

        virtual void onDraw(Renderer&);
        virtual void onUpdate(double delta);
        virtual void onBegin();
        virtual void onPress();
        virtual void onRaise();
        virtual void onReset();
        [[nodiscard]] virtual bool needsApproachCircle() const;
        [[nodiscard]] virtual HitResult onFinish();

        fcircle SOF;
        bool finished;
        double timeFinished;
        const BaseGameMode *session;
    private:
        HitObjectState state;
    };
}