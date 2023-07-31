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
#include "SoundStream.hpp"
#include "Skin.hpp"
#include "InputMapper.hpp"

#include <list>

namespace PROJECT_NAMESPACE {

constexpr const char *APPROACH_CIRCLE_SPRITE = "approach_circle";

constexpr const char *PLAY_FIELD_SPRITE = "play_field";

constexpr const char *CURSOR_SPRITE = "cursor";

constexpr const char *HIT_SOUND = "hit_sound";

constexpr const char *MISS_SOUND = "miss_sound";

constexpr const char *COMBO_BREAK_SOUND = "combo_break_sound";

class BaseHitObject;

struct SampleSet
{
	Resource<SoundSample> hit;
	Resource<SoundSample> miss;
	Resource<SoundSample> comboBreak;
	Resource<SoundSample> sliderBounce;
	Resource<SoundSample> sliderSlide;
	Resource<SoundSample> sliderBreak;
	Resource<SoundSample> spinnerSwoosh;
	Resource<SoundSample> spinnerDing;
};

class GameManager
{
public:
	using StorageT = std::list<std::shared_ptr<BaseHitObject>>;

	explicit GameManager();

	virtual void update(double delta);

	virtual void draw(video::LambdaRender& gfx);

	[[nodiscard]] double getCurrentTime() const;

	void scrobble(double amount);

	void setCurrentTime(double newTime);

	[[nodiscard]] const frect &getPlayField() const;

	void setPlayField(const frect &playField);

	[[nodiscard]] Resource<MapInfo> getMap() const;

	bool setMap(Resource<MapInfo> map);

	void skipToFirst();

	void reset();

	[[nodiscard]] fvec2d getCursorPosition() const;

	[[nodiscard]] const Mat3f &getTransform() const;

	[[nodiscard]] float getCircleSize() const;

	[[nodiscard]] float getApproachTime() const;

	[[nodiscard]] float getFadeTime() const;

	[[nodiscard]] float getHitWindow() const;

	[[nodiscard]] float getHpDrain() const;

	[[nodiscard]] float getStartOffset() const;

	[[nodiscard]] const SampleSet &getSamples() const;

	[[nodiscard]] bool isFinished() const;

	void setInputMapper(std::unique_ptr<InputMapper> &&mapper);

	[[nodiscard]] std::weak_ptr<BaseHitObject> getCurrentObject() const;

	[[nodiscard]] GameManager::StorageT::const_iterator getClosestActiveObject() const;

	[[nodiscard]] std::weak_ptr<BaseHitObject> getNextObject() const;

	[[nodiscard]] const StorageT &getStoredObjects() const;

    [[nodiscard]] double getDelta() const;

    void setSkin(Resource<Skin>);

private:
	unsigned int loadObjects(unsigned int amount);
	[[nodiscard]] bool resolveFunction(HitObjectFunction func, const BaseHitObject &object) const;

    // FIXME: need to somehow pass the skin to this point
    Resource<Skin> skin;
    double delta{};
	std::unique_ptr<InputMapper> input{nullptr};
	StorageT::iterator last{};
	StorageT activeObjects{};
	Resource<MapInfo> info{nullptr};
	SampleSet samples{};
	Mat3f transform{MAT3_NO_TRANSFORM<float>};
	frect playField{UNIT_RECT<float>};
	double currentTime{0.0};

	MapInfo::StorageT::const_iterator lastLoadedObject;

	float arMultiplier{1.0f};
	float csMultiplier{1.0f};
	float hwMultiplier{1.0f};
	float hpMultiplier{1.0f};
	float ftMultiplier{1.0f};

	// Profiling
	unsigned int historyInsertSpot{0};
	std::array<float, 64> history{};
	unsigned int printCounter = 60;
	unsigned long rollingUT = 0.0;
	unsigned long rollingUPF = 0;
	double averageUPF = 1.0f;
	double averageUT = 1.0f;
	float minUT = std::numeric_limits<float>::max();
	float maxUT = 0.0f;
};

}