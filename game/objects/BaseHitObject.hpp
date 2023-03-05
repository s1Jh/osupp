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

#include "Circle.hpp"
#include "Enum.hpp"
#include "Context.hpp"

namespace PROJECT_NAMESPACE {

/**
 * The following class encompasses all logic surrounding hit objects.
 *
 * Each object holds a startTime floating point value which represents in
 *seconds the startTime at which the object should be hit for a perfect score.
 *
 * Upon completion, the startTime value changes to the startTime at which the
 *object has been completed.
 *
 * The object holds a bool finished variable that gets set to true the moment
 * an object is considered to be done. This can occur either when a player
 *successfully hits the object, or misses it entirely.
 *
 * A update method is called each frame when the object is in a active state.
 *
 * When a player first interacts with a hit object, the begin() method is
 *called. This will transfer the object into the Active state. This also sets
 *the startTime at which the player has began holding down on an object.
 *
 * When the player raises their hit button, or when the player's cursor leaves
 * the object's circle of influence, the raise() method is called.
 *
 * Once the object enters the fading state, the finish() method will be called.
 * This method will return how well the object has been hit.
 **/

class GameManager;

class BaseHitObject
{
    friend class GameManager;
public:
    void create(Resource<Skin>&);

    void update();

    void begin();

    void press();

    void raise();

    void draw(video::LambdaRender&);

	[[nodiscard]] HitResult finish();

	[[nodiscard]] fcircle getSOF() const;

	[[nodiscard]] bool isFinished() const;

	[[nodiscard]] HitObjectState getState() const;

	[[nodiscard]] double getTimeFinished() const;

	[[nodiscard]] double getTimeStarted() const;

	void reset();

	// implemented by HitObject
    [[nodiscard]] virtual double getStartTime() const = 0;

	// implemented by HitObject
    [[nodiscard]] virtual double getEndTime() const = 0;

    [[nodiscard]] virtual fvec2d getStartPosition() const;

    [[nodiscard]] virtual fvec2d getEndPosition() const;

	[[nodiscard]] virtual HitObjectFunction getActivationFunction() const;

	[[nodiscard]] virtual HitObjectFunction getDeactivationFunction() const;

protected:
    explicit BaseHitObject();

    void transferToInvisible();

    void transferApproaching();

    void transferReady();

    void transferActive();

    void transferActiveInactive();

    void transferInactiveActive();

    void transferToPickup();

    void transferToFading();

    void transferToInvisibleComplete();

    [[nodiscard]] bool isApproachCircleDrawn() const;

    [[nodiscard]] bool isFadingIn() const;

    [[nodiscard]] bool isFadingOut() const;

    [[nodiscard]] bool isUpdating() const;

    [[nodiscard]] bool isActive() const;

    [[nodiscard]] float getAlpha() const;

    virtual void onDraw(video::LambdaRender& gfx);

    virtual void onUpdate();

    virtual void onLogicUpdate();

    virtual void onBegin();

    virtual void onPress();

    virtual void onRaise();

    virtual void onReset();

    virtual void onCreate(Resource<Skin>&);

    [[nodiscard]] virtual HitResult onFinish();

	// implemented by HitObject derivatives for different gamemodes
	[[nodiscard]] virtual Mat3f calculateObjectTransform() const = 0;

	[[nodiscard]] const Mat3f& getObjectTransform() const;

	fcircle SOF;

    GameManager &getGame();
    const GameManager &getGame() const;

private:
    GameManager* game;
	bool finished;
	double timeStarted;
	double timeFinished;
	Mat3f objectTransform;
    HitObjectState state;
};

}