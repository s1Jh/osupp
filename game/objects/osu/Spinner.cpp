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

#include "Spinner.hpp"

#include "Math.hpp"

NS_BEGIN

void Spinner::onLogicUpdate(double delta)
{
    if (isActive()) {
        auto cursor = math::Normalize(ctx.game.getCursorPosition() - objectTemplate->position);
        auto alpha = (float) math::Cross(lastVector, cursor);

        RPM = float(double(math::Abs(alpha) / 2.0_pi) / delta * 60.0);
        rotation += (float) alpha;

        if (math::Abs(rotation) > 2.0_pi) {
            rotation = std::fmod(rotation, (float) 2.0_pi);
            rotationsCompleted++;
        }

        lastVector = cursor;
    }

	float x = float(ctx.timing.getTime()) * 30.f;
	const float outset = 0.25f;
	SOF.position = {std::cos(x) * outset, std::sin(x) * outset};
}

void Spinner::onUpdate(double delta)
{
    // update texture animations
    spinner.update(delta);
    spinnerCenter.update(delta);
    spinnerMeter.update(delta);
}

void Spinner::onBegin()
{
    lastVector = math::Normalize(ctx.game.getCursorPosition() - objectTemplate->position);
}

HitResult Spinner::onFinish()
{
    if (rotationsCompleted == 0)
        return HitResult::MISSED;

    return HitResult::HIT50;
}

Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, const HitObjectArguments& args)
    : OsuHitObject(std::move(t), args), RPM(0.0), rotationAccum(0.0f), rotationsCompleted(0), rotation(0.0f)
{
    const auto &skin = ctx.activeSkin;

    spinner = skin->createObjectSprite(SPINNER_SPRITE, args);
    spinnerCenter = skin->createObjectSprite(SPINNER_CENTER_SPRITE, args);
    spinnerMeter = skin->createObjectSprite(SPINNER_METER_SPRITE, args);

    SOF = {objectTemplate->free ? ctx.game.getCircleSize() : 2.0f, objectTemplate->position};
}

void Spinner::onPress()
{
    lastVector = math::Normalize(ctx.game.getCursorPosition());
}

void Spinner::onDraw()
{
    auto alpha = getAlpha();
	const auto& objectTransform = getObjectTransform();

    auto size = objectTemplate->free ? ctx.game.getCircleSize() : 0.8f;

    frect all = {{size, size}, objectTemplate->position};

    ObjectDrawInfo spinnerInfo{all, alpha, objectTransform};
    ObjectDrawInfo spinnerCenterInfo{
        all, alpha,
        (Mat3f) video::Transform2D{.rotate = rotation, .rotationCenter = objectTemplate->position} * objectTransform};

    ctx.gfx.draw(DrawObject{spinner, spinnerInfo});
	ctx.gfx.draw(DrawObject{spinnerCenter, spinnerCenterInfo});
}

void Spinner::onReset()
{
    rotation = 0.0f;
    rotationAccum = 0.0f;
    rotationsCompleted = 0;
}

HitObjectFunction Spinner::getActivationFunction() const
{
	return HitObjectFunction::BUTTON_HELD | HitObjectFunction::CURSOR_IGNORE;
}

HitObjectFunction Spinner::getDeactivationFunction() const
{
	return HitObjectFunction::BUTTON_RELEASED | HitObjectFunction::CURSOR_IGNORE;
}

NS_END