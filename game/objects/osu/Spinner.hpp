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

#include "OsuHitObject.hpp"
#include "SpinnerTemplate.hpp"
#include "define.hpp"

NS_BEGIN
class Spinner: public OsuHitObject<ObjectTemplateSpinner, OsuHitObjectFlags::NoApproachCircle>
{
public:
    explicit Spinner(std::shared_ptr<ObjectTemplateSpinner>, const HitObjectArguments& args);

	[[nodiscard]] HitObjectFunction getDeactivationFunction() const override;
	[[nodiscard]] HitObjectFunction getActivationFunction() const override;

protected:
    void onDraw() override;

    void onLogicUpdate(double delta) override;

    void onUpdate(double delta) override;

    void onBegin() override;

    void onPress() override;

    HitResult onFinish() override;

    void onReset() override;
private:
    ObjectSprite spinner;
    ObjectSprite spinnerCenter;
    ObjectSprite spinnerMeter;
    float RPM;
    float rotationAccum;
    unsigned int rotationsCompleted;
    float rotation;
    fvec2d lastVector;
};
}