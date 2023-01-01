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
#include "Slider.hpp"

#include "SliderTrail.hpp"

#include <utility>

NS_BEGIN

void Slider::onUpdate(double delta)
{
    // update texture animations
    bodyTexture.update(delta);
    ball.update(delta);
    head.update(delta);
    headRepeat.update(delta);
    tail.update(delta);
    tailRepeat.update(delta);
    hitPoint.update(delta);
}

void Slider::onLogicUpdate(double delta)
{
    double timeLength = getEndTime() - getStartTime();
    double timeRunning = ctx.game.getCurrentTime() - startPoint;

    timeRunning = math::Clamp(timeRunning, 0.0, timeLength);

    progression = timeRunning / timeLength;

    const auto &repeats = objectTemplate->repeats;
    const auto currentRepeat = (unsigned int) (timeRunning * repeats / timeLength);
    repeatsLeft = repeats - currentRepeat;

    TravelDirection nextDirection = currentRepeat % 2 == 1 ? TravelDirection::Backward : TravelDirection::Forward;
    if (currentDirection != nextDirection && repeatsLeft > 0) {
        ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().sliderBounce.ref());
        currentDirection = nextDirection;
    }

    curvePosition = std::fmod(progression * repeats, 1.0 + std::numeric_limits<float>::epsilon());

    if (currentDirection == TravelDirection::Backward) {
        curvePosition = 1 - curvePosition;
    }

    if (repeatsLeft == 0) [[unlikely]] {
        transferToPickup();
        SOF.position = getEndPosition();
    } else {
        SOF.position = curve.get<math::CurveType::STRAIGHT>(curvePosition);
    }

    const float maxSOFSizeMultiplier = 2.0;
    const float SOFGrowthFactor = 6.0f;
    auto maxSOFSize = (float) ctx.game.getCircleSize() * maxSOFSizeMultiplier;

    auto growthChunk = float(maxSOFSize * SOFGrowthFactor * delta);

    if (isActive()) {
        SOF.radius = maxSOFSize;
        visualRingSize += growthChunk;
    } else {
        SOF.radius = ctx.game.getCircleSize();
        visualRingSize -= growthChunk;
    }

    visualRingSize = math::Clamp(
        visualRingSize, ctx.game.getCircleSize(),
        ctx.game.getCircleSize() * maxSOFSizeMultiplier
    );
}

void Slider::onBegin()
{
    startPoint = math::Min(ctx.game.getCurrentTime(), getStartTime());

    ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().hit.ref());
    started = true;
}

HitResult Slider::onFinish()
{
    if (!started) {
        ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().miss.ref());
        return HitResult::MISSED;
    }
    if (broken) {
        return HitResult::HIT100;
    }

    return HitResult::HIT300;
}

void Slider::onRaise()
{
    ctx.audio.getSFXChannel().playSound(ctx.game.getSamples().sliderBreak.ref());
    broken = true;
}

void Slider::onPress()
{
    started = true;
}

Slider::Slider(std::shared_ptr<ObjectTemplateSlider> templateIn, const HitObjectArguments &args)
    : OsuHitObject(std::move(templateIn), args), currentDirection(TravelDirection::Forward),
      broken(false), started(false), progression(0.0), curvePosition(0.0)
{
    const auto &skin = ctx.activeSkin;

    startPoint = getStartTime();

    bodyShader = skin->getShader(SLIDER_SHADER);
    bodyTexture = skin->createObjectSprite(SLIDER_BODY_SPRITE, args);
    ball = skin->createObjectSprite(SLIDER_BALL_SPRITE, args);
    ballRing = skin->createObjectSprite(SLIDER_BALL_RING_SPRITE, args);
    head = skin->createObjectSprite(SLIDER_HEAD_SPRITE, args);
    headRepeat = skin->createObjectSprite(SLIDER_HEAD_REPEAT_SPRITE, args);
    tail = skin->createObjectSprite(SLIDER_TAIL_SPRITE, args);
    tailRepeat = skin->createObjectSprite(SLIDER_TAIL_REPEAT_SPRITE, args);
    hitPoint = skin->createObjectSprite(SLIDER_HIT_POINT_SPRITE, args);

    /*============================================================================================================*/
    // Initialize the variables to their defaults.
    auto &path = objectTemplate->path;
    SOF = {ctx.game.getCircleSize(),
        {path.front().position}};
    repeatsLeft = objectTemplate->repeats;

    /*============================================================================================================*/
    // Create curve out of the template points for interpolating.
    math::Curve<SliderPathT::iterator> templateCurve(path.begin(), path.end());

    // Prepare some misc variables.
    interpolatedPath.clear();
    auto lastIt = interpolatedPath.end();
    auto middleIt = interpolatedPath.end();
    auto steps =
        math::Max((unsigned int) (SLIDER_STEPS_PER_CURVE_UNIT * templateCurve.getLength()), 2);

    // Interpolate over n * length steps.
    for (unsigned int i = 0; i <= steps; i++) {
        const auto t = double(i) / double(steps);

        fvec2d thisPosition = templateCurve.get(objectTemplate->sliderType, t);

        // meshSpine.push_back(thisPosition);

        // Optimizations
        if (i >= 2) {
            middleIt = std::prev(interpolatedPath.end(), 1);
            lastIt = std::prev(interpolatedPath.end(), 2);
            auto lastPosition = lastIt->position;
            auto midPosition = middleIt->position;
            if (lastIt != interpolatedPath.end()) {
                // Optimization #1: If a triplet of sequential points has a small enough
                // angle, remove the middle point of this triplet.

                auto vec1 = math::Normalize(lastPosition - midPosition);
                auto vec2 = math::Normalize(thisPosition - midPosition);

                auto angle = math::Abs(math::Cross(vec1, vec2));
                if (angle <= SLIDER_ANGLE_OPT_THRESHOLD) {
                    interpolatedPath.erase(middleIt);
                }
            }

            // Optimization #2: If the new point is too close to an existing point,
            // don't add it.
//            if (i < steps)
//                if (Distance(thisPosition, midPosition) <= SLIDER_DISTANCE_OPT_THRESHOLD && i != steps) {
//                    continue;
//                }
        }
        interpolatedPath.push_back({thisPosition, false});
    }

    /*============================================================================================================*/
    // Finish setup by setting up our interpolated curve and constant variables.
    curve.setIteratorRange(interpolatedPath.begin(), interpolatedPath.end());

    auto startBumperDirection = findNormal(0.0);
    startBumperAngle = std::atan2(startBumperDirection.y, startBumperDirection.x);
    auto endBumperDirection = findNormal(1.0);
    // Needs to be flipped, add π.
    endBumperAngle = std::atan2(endBumperDirection.y, endBumperDirection.x) + math::fPI;
}

void Slider::onDraw()
{
    /*============================================================================================================*/
    // Prepare some constant variables.
    const auto &objectTransform = getObjectTransform();
    const auto circleSize = ctx.game.getCircleSize();
    const auto alpha = getAlpha();

    /*============================================================================================================*/
    // Draw the curve body.

    if (bodyTexture.getTexture() && bodyShader) {
        float end = 1.0f, start = 0.0f;
        bool useTexture = true;

        // TODO: customization of fade in/out animations
        if (isFadingIn()) {
            useTexture = false;
            auto hw = ctx.game.getHitWindow();
            auto snakeInPeriod = float(ctx.game.getApproachTime() - hw);
            auto snakeInEnd = float(getStartTime() - hw);
            end = 1.0f + float((ctx.game.getCurrentTime() - snakeInEnd) / snakeInPeriod);
            end = math::SmoothStep(end);
        }

        if (repeatsLeft <= 1 && objectTemplate->repeats != 1) {
            useTexture = false;
            end = (float) curvePosition;
        }

        // TODO: customization of slider trail oppacity
        SliderTrailDrawInfo sliderInfo = {
            .start = start,
            .end = end,
            .useTexture = useTexture,
            .thickness = circleSize,
            .bakedTexture = nullptr,
            .shader = bodyShader,
            .alpha = alpha,
            .sprite = &bodyTexture,
            .transform = objectTransform
        };
        ctx.gfx.draw(DrawSliderTrail{curve, sliderInfo});
    }
    /*============================================================================================================*/
    // Draw curve decorations such as bonus points.

    ObjectDrawInfo hitPointInfo = {SOF, alpha, objectTransform};
    for (const auto &node : interpolatedPath) {
        if (node.bonus) {
            hitPointInfo.destination.position = node.position;
            ctx.gfx.draw(DrawObject{hitPoint, hitPointInfo});
        }
    }

    /*============================================================================================================*/
    // Draw the start decorations.
    const auto &startBumperPosition = interpolatedPath.front().position;

    ObjectDrawInfo headInfo = {
        {{circleSize, circleSize}, startBumperPosition},
        alpha,
        (Mat3f) video::Transform2D{
            .rotate = startBumperAngle,
            .rotationCenter = startBumperPosition
        } * objectTransform
    };

    ctx.gfx.draw(DrawObject{head, headInfo});

    if (repeatsLeft >= 3) {
        ctx.gfx.draw(DrawObject{headRepeat, headInfo});
    }

    /*============================================================================================================*/
    // Draw the end decorations.
    const auto &endBumperPosition = interpolatedPath.back().position;

    ObjectDrawInfo tailInfo = {
        {{circleSize, circleSize}, endBumperPosition},
        alpha,
        (Mat3f) video::Transform2D{.rotate = endBumperAngle,
            .rotationCenter = endBumperPosition} *
            objectTransform};

    ctx.gfx.draw(DrawObject{tail, tailInfo});

    if (repeatsLeft >= 2) {
        ctx.gfx.draw(DrawObject{tailRepeat, tailInfo});
    }

    /*============================================================================================================*/
    // Draw the approach circle underneath the ball which is our starting target
    drawApproachCircle();

    /*============================================================================================================*/
    // Draw the ball moving across the slider.
    const auto ballDirection = findNormal(curvePosition);
    const auto ballAngle = std::atan2(ballDirection.y, ballDirection.x);

    ObjectDrawInfo ballInfo = {
        {{circleSize, circleSize}, SOF.position}, alpha,
        (Mat3f) video::Transform2D{.rotate = ballAngle, .rotationCenter = SOF.position} *
            objectTransform};

    ctx.gfx.draw(DrawObject{ball, ballInfo});

    ObjectDrawInfo ringInfo = {
        {{visualRingSize, visualRingSize}, SOF.position}, alpha,
        (Mat3f) video::Transform2D{.rotate = ballAngle, .rotationCenter = SOF.position} *
            objectTransform};
    if (isActive()) {
        ctx.gfx.draw(DrawObject{ballRing, ringInfo});
    }
}

void Slider::onReset()
{
    currentDirection = TravelDirection::Forward;
    started = false;
    broken = false;
    curvePosition = 0.0;
    progression = 0.0;
    repeatsLeft = objectTemplate->repeats;
    SOF.position = getStartPosition();
}

fvec2d Slider::findDirection(double t)
{
    // Clamp the parameter to <0; t-ε>.
    // This is done so that if t=1, calling curve.get(t+ε) would yield the same
    // result as calling curve.get(t). because the value of t is clamped in the
    // calculation.
    t = math::Clamp(t, 0.0, 1.0 - SLIDER_DIRECTION_EPSILON);

    // We find the direction vector by pointing a vector from the requested
    // parametric position t and another position equal to t + ε, with ε being a
    // small positive offset. We normalize this vector and return.
    auto targetPosition = curve.get<math::CurveType::STRAIGHT>(t);
    auto offsetPosition = curve.get<math::CurveType::STRAIGHT>(t + SLIDER_DIRECTION_EPSILON);
    return math::Normalize(offsetPosition - targetPosition);
}

fvec2d Slider::findNormal(double t)
{ return math::Normal(findDirection(t)); }

fvec2d Slider::getStartPosition() const
{
    return interpolatedPath.front().position;
}

fvec2d Slider::getEndPosition() const
{
    if (objectTemplate->repeats % 2 == 1) {
        return interpolatedPath.back().position;
    } else {
        return interpolatedPath.front().position;
    }
}
HitObjectFunction Slider::getActivationFunction() const
{
    if (getState() == HitObjectState::READY && ctx.game.getCurrentTime() < getStartTime()) {
        return HitObjectFunction::CURSOR_ENTER | HitObjectFunction::BUTTON_PRESSED;
    } else {
        return HitObjectFunction::CURSOR_ENTER | HitObjectFunction::BUTTON_HELD;
    }
}
}