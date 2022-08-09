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

#include "HitObject.tpp"
#include "SliderTemplate.hpp"
#include "define.hpp"

NS_BEGIN

constexpr double SLIDER_DIRECTION_EPSILON = 0.01;

constexpr double SLIDER_ANGLE_OPT_THRESHOLD = 0.05;

constexpr double SLIDER_DISTANCE_OPT_THRESHOLD = 0.1;

constexpr unsigned int SLIDER_STEPS_PER_CURVE_UNIT = 10;

class Slider: public HitObject<ObjectTemplateSlider>
{
public:
    explicit Slider(std::shared_ptr<ObjectTemplateSlider>, BaseGameMode &);

    [[nodiscard]] fvec2d getStartPosition() const override;

    [[nodiscard]] fvec2d getEndPosition() const override;

protected:
    [[nodiscard]] fvec2d findDirection(double t);

    [[nodiscard]] fvec2d findNormal(double t);

    void onReset() override;

    enum class TravelDirection
    {
        Forward, Backward
    };

    void onUpdate(double delta) override;

    void onLogicUpdate(double delta) override;

    void onBegin() override;

    HitResult onFinish() override;

    void onRaise() override;

    void onDraw(Renderer &renderer) override;

    void onPress() override;

private:
    ShaderP bodyShader;
    Mesh body;

    ObjectSprite bodyTexture;
	ObjectSprite ballRing;
    ObjectSprite ball;
    ObjectSprite head;
    ObjectSprite headRepeat;
    ObjectSprite tail;
    ObjectSprite tailRepeat;
    ObjectSprite hitPoint;

    std::list<fvec2d> meshOutline;
    std::vector<fvec2d> meshSpine;
    std::vector<fvec2d> loops;
    std::vector<fcircle> circleMask;

    // the interpolated path
    SliderPathT interpolatedPath;
    Curve<SliderPathT::iterator> curve;

    // cached variables
    float endBumperAngle;
    float startBumperAngle;

    // state
    TravelDirection currentDirection;
    bool active;
    bool broken;
    bool started;
    unsigned int repeatsLeft;
    double progression;
    double curvePosition;
};

NS_END
