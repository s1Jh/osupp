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

#include "Keyboard.hpp"
#include "MapInfo.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "SoundStream.hpp"
#include "Skin.hpp"
#include "InputMapper.hpp"

#include <list>

NS_BEGIN

class BaseHitObject;

struct SampleSet {
	SoundSampleP hit;
	SoundSampleP miss;
	SoundSampleP sliderBounce;
	SoundSampleP sliderSlide;
	SoundSampleP sliderBreak;
	SoundSampleP spinnerSwoosh;
	SoundSampleP spinnerDing;
};

class GameManager
{
public:
    using StorageT = std::list<std::shared_ptr<BaseHitObject>>;

	explicit GameManager();

    virtual void update(double delta);

    virtual void draw(Renderer &);

    [[nodiscard]] double getCurrentTime() const;

    void setCurrentTime(double newTime);

    [[nodiscard]] const frect &getPlayField() const;

    void setPlayField(const frect &playField);

    [[nodiscard]] MapInfoP getMap() const;

    bool setMap(MapInfoP map);

    void reset();

	bool start();

	void stop();

	[[nodiscard]] fvec2d getCursorPosition() const;

    [[nodiscard]] const Mat3f &getTransform() const;

    [[nodiscard]] float getCircleSize();

    [[nodiscard]] float getApproachTime();

    [[nodiscard]] float getFadeTime();

    [[nodiscard]] float getHitWindow();

	[[nodiscard]] float getHpDrain();

	[[nodiscard]] float getStartOffset();

	[[nodiscard]] const SampleSet &getSamples() const;

	[[nodiscard]] bool isFinished() const;

	void setInputMapper(std::unique_ptr<InputMapper>&& mapper);

	[[nodiscard]] std::weak_ptr<BaseHitObject> getCurrentObject() const;

	[[nodiscard]] std::weak_ptr<BaseHitObject> getNextObject() const;

	[[nodiscard]] const StorageT& getStoredObjects() const;

private:
	[[nodiscard]] bool resolveFunction(HitObjectFunction func, const BaseHitObject& object) const;

	ObjectSprite cursor;

	std::unique_ptr<InputMapper> input{nullptr};
	StorageT::iterator last{};
	StorageT activeObjects{};
	MapInfoP info{nullptr};
	SampleSet samples{};
	bool simulationRunning{false};
    Mat3f transform{MAT3_NO_TRANSFORM<float>};
    frect playField{UNIT_RECT<float>};
    double currentTime{0.0};


	float arMultiplier{1.0f};
	float csMultiplier{1.0f};
	float hwMultiplier{1.0f};
	float hpMultiplier{1.0f};
	float ftMultiplier{1.0f};

	unsigned int historyInsertSpot{0};
	std::array<float, 64> history;
	unsigned int printCounter = 60;
	unsigned long rollingUT = 0.0;
	unsigned long rollingUPF = 0;
	double averageUPF = 1.0f;
	double averageUT = 1.0f;
	float minUT = std::numeric_limits<float>::max();
	float maxUT = 0.0f;
};

NS_END