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
        auto cursor = Normalize(session.getCursorPosition() - objectTemplate->position);
        auto alpha = (float) Cross(lastVector, cursor);

        RPM = float(double(Abs(alpha) / 2.0_pi) / delta * 60.0);
        rotation += (float) alpha;

        if (Abs(rotation) > 2.0_pi) {
            rotation = std::fmod(rotation, (float) 2.0_pi);
            rotationsCompleted++;
        }

        lastVector = cursor;
    }
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
    lastVector = Normalize(session.getCursorPosition() - objectTemplate->position);
}

HitResult Spinner::onFinish()
{
    if (rotationsCompleted == 0)
        return HitResult::Missed;

    return HitResult::Hit50;
}

Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, BaseGameMode &g)
    : HitObject(std::move(t), g), RPM(0.0), rotationAccum(0.0f), rotationsCompleted(0), rotation(0.0f)
{
    const auto &skin = GetContext().activeSkin;

    spinner = skin->createObjectSprite(SPINNER_SPRITE);
    spinnerCenter = skin->createObjectSprite(SPINNER_CENTER_SPRITE);
    spinnerMeter = skin->createObjectSprite(SPINNER_METER_SPRITE);

    SOF = {objectTemplate->free ? session.getCircleSize() : 2.0f, objectTemplate->position};
}

void Spinner::onPress()
{
    lastVector = Normalize(session.getCursorPosition());
}

void Spinner::onDraw(Renderer &renderer)
{
    auto alpha = getAlpha();
    const auto &objectTransform = session.getObjectTransform();

    auto size = objectTemplate->free ? session.getCircleSize() : 0.8f;

    frect all = {{size, size}, objectTemplate->position};

    ObjectDrawInfo spinnerInfo{all, alpha, objectTransform};
    ObjectDrawInfo spinnerCenterInfo{
        all, alpha,
        (Mat3f) Transform2D{.rotate = rotation, .rotationCenter = objectTemplate->position} * objectTransform};

    renderer.draw(spinner, spinnerInfo);
    renderer.draw(spinnerCenter, spinnerCenterInfo);
}

bool Spinner::needsApproachCircle() const
{ return false; }

void Spinner::onReset()
{
    rotation = 0.0f;
    rotationAccum = 0.0f;
    rotationsCompleted = 0;
}

NS_END