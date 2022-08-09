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

#include "BaseGameMode.hpp"

#include "HitObjects.hpp"
#include "Math.hpp"
#include "Util.hpp"

#define MAKE_CASE(TYPE)                                                         \
  case HitObjectType::TYPE: {                                                   \
    auto object = std::make_shared<TYPE>(                                       \
        std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), *this); \
    activeObjects.push_front(object);                                           \
    break;                                                                      \
  }

NS_BEGIN

BaseGameMode::BaseGameMode()
    : info(nullptr), playField(UNIT_RECT<float>), currentTime(0.0)
{
    reset();
}

double BaseGameMode::getCurrentTime() const
{ return currentTime; }

void BaseGameMode::setCurrentTime(double newTime)
{
    currentTime = newTime;

    for (auto &obj: activeObjects) {
        obj->reset();
        obj->update(0.0);
    }

    last = activeObjects.begin();
}

void BaseGameMode::update(double delta)
{
    currentTime += delta;
    this->onUpdate(delta);
}

void BaseGameMode::draw(Renderer &gfx)
{ this->onDraw(gfx); }

void BaseGameMode::setMap(MapInfoP map)
{
    info = map;
    activeObjects.clear();
	musicTrack = nullptr;

    if (info) {
		int objectSeed = 0;
		int comboSeed = 0;

		musicTrack = GetContext().resources.get<SoundStream>(info->getSongPath(), info->getDirectory());

        auto templates = info->getObjectTemplates();
        for (const auto &objectTemplate: templates) {
            switch (objectTemplate->getType()) {
                MAKE_CASE(Spinner)
                MAKE_CASE(Slider)
                MAKE_CASE(Note)
                default:
                    log::warning("Corrupted map template: ", objectTemplate);
                    break;
            }

            if (bool(objectTemplate->parameters & HitObjectParams::ComboEnd))
                comboSeed++;
            objectSeed++;
        }
    }

    reset();
}

void BaseGameMode::reset()
{
    log::debug("Resetting");
    if (info)
        currentTime = info->getStartOffset();
    else
        currentTime = 0.0;

    for (auto &object: activeObjects) {
        object->reset();
    }

	auto& channel = GetContext().audio.getMusicChannel();
	channel.setSound(musicTrack, true);

    last = activeObjects.begin();

    this->onReset();
}

const frect &BaseGameMode::getPlayField() const
{ return playField; }

void BaseGameMode::setPlayField(const frect &field)
{
    playField = field;
    auto smaller = Min(field.size.w, field.size.h);
    transform = MakeScaleMatrix(fvec2d(smaller, smaller)) *
        MakeTranslationMatrix(field.position);
}

fvec2d BaseGameMode::getCursorPosition() const
{
    auto pos = GetContext().mouse.position();
    pos -= playField.position;
    auto smaller = Min(playField.size.w, playField.size.h);
    pos /= fvec2d{smaller, smaller};
    return pos;
}

MapInfoP BaseGameMode::getMap() const
{ return info; }

const Mat3f &BaseGameMode::getObjectTransform() const
{ return transform; }

float BaseGameMode::getCircleSize()
{
    return info->getCircleSize(); /* * multiplier */
}

float BaseGameMode::getApproachTime()
{
    return info->getApproachTime(); /* * multiplier */
}

float BaseGameMode::getFadeTime()
{
    return info->getFadeTime(); /* * multiplier */
}

float BaseGameMode::getHitWindow()
{
    return info->getHitWindow(); /* * multiplier */
}

void BaseGameMode::onReset()
{}

NS_END