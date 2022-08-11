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

#include "GameManager.hpp"

#include <utility>

#include "HitObjects.hpp"
#include "Math.hpp"
#include "Util.hpp"

#include "imgui.h"

#define MAKE_CASE(TYPE)                                                         \
  case HitObjectType::TYPE: {                                                   \
    auto object = std::make_shared<TYPE>(                                       \
        std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), args); \
    activeObjects.push_front(object);                                           \
    break;                                                                      \
  }

NS_BEGIN

GameManager::GameManager()
    : info(nullptr), playField(UNIT_RECT<float>), currentTime(0.0)
{
}

double GameManager::getCurrentTime() const
{ return currentTime; }

void GameManager::setCurrentTime(double newTime)
{
    currentTime = newTime;

    for (auto &obj: activeObjects) {
        obj->reset();
        obj->update(0.0);
    }

    last = activeObjects.begin();
}

void GameManager::update(double delta)
{
    currentTime += delta;

	auto& keyboard = GetContext().keyboard;
	// objects are sorted by order of appearance
	int updates = 0;
	printCounter++;
	auto start = std::chrono::high_resolution_clock::now();
	for (auto it = last;; it++) {
		if (it == activeObjects.end())
			return;

		auto &obj = *it;

		obj->update(delta);
		updates++;
		switch (obj->getState()) {
		case HitObjectState::Invisible:
			if (obj->isFinished()) {
				// This object has already been passed and is invisible.
				// We can assume it will not need to be checked anymore.
				// Therefore, we will set the object after this as the next "last"
				// object. The onUpdate loop will then start with that object,
				// ignoring all objects before, who we've assumed to already be done.
				auto nextIt = std::next(it);
				if (last != nextIt) {
					if (nextIt == activeObjects.end()) {
						// first check if nextIt is the end, if it is so
						// just set it without checking for time as that would
						// cause access violation
						last = activeObjects.end();
					}
					else if ((*last)->getEndTime() < (*nextIt)->getEndTime()) {
						last = nextIt;
					}
				}
				continue;
			}
			else {
				// The object is ahead of our field of vision, since objects are sorted
				// by order of appearance, all objects ahead of this one will also be
				// invisible. We therefore stop checking and return.

				// before that, let's take the opportunity and do some performance calculations
				rollingUPF += updates;
				auto duration = std::chrono::high_resolution_clock::now() - start;
				auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
				rollingUT += micros;

				if (printCounter >= 60) {
					averageUPF = double(rollingUPF) / 60.0;
					averageUT = double(rollingUT) / 60.0;

					printCounter = 0;
					rollingUT = 0;
					rollingUPF = 0;
				}

				return;
			}
		case HitObjectState::Ready: {
			// The object can now be interacted with, check for user inputs and then
			// update the object accordingly.
			auto func = obj->getActivationFunction();

			if (resolveFunction(func, *obj))
			{
				// We are within the object's hit window and are pressing down on the
				// right keys, start.
				obj->begin();
				return;
			}

			break;
		}
		case HitObjectState::Active: {
			// object has been activated and is currently being held, check if it's
			// been released

			auto func = obj->getDeactivationFunction();

			if (resolveFunction(func, *obj))
			{
				obj->raise();
			}
			break;
		}
		case HitObjectState::Inactive: {
			// object has been activated but is not being held, check if it's being
			// held again
			auto func = obj->getActivationFunction();

			if (resolveFunction(func, *obj))
			{
				obj->press();
			}
			break;
		}
			// The object cannot be interacted with in these states
		case HitObjectState::Pickup: {
			auto score = obj->finish();
			log::info("SCORE: ", (int) score);
			break;
		}
		case HitObjectState::Fading:
		case HitObjectState::Approaching:
		default:
			break;
		}
	}
}

void GameManager::draw(Renderer &renderer)
{
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowCollapsed(false);

	if (ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::Text("UPF: %f, (%f us)", averageUPF, averageUT);
		double UPS = 1000000.0 / averageUT;
		ImGui::SameLine();
		ImGui::Text("UPS: %f", UPS);
		ImGui::End();
	}

	// the "last" object should be the next visible object in line
	// we therefore draw every object until we hit one that is invisible
	if (last == activeObjects.end())
		return;

	// we need to reverse the order objects are drawn in, meaning that objects which will come next will be drawn last
	// this is done so that the most relevant objects are always on top

	// first find the last object that would be drawn
	auto end = std::find_if(last, activeObjects.end(), [](const std::shared_ptr<BaseHitObject> &obj)
	{
		return (obj->getState() == HitObjectState::Invisible) && !obj->isFinished();
	});

	// render everything from the object before last to the current object
	for (auto it = std::prev(end); it != std::prev(last); it--) {
		auto ptr = *it;
		ptr->draw();

//        renderer.draw(ptr->getEndPosition(), 0.1f, VisualAppearance{.fillColor = RED, .zIndex = 1.0f}, transform);
//        renderer.draw(ptr->getStartPosition(), 0.1f, VisualAppearance{.fillColor = GREEN, .zIndex = 1.0f}, transform);
//		if (it != last && it != std::prev(end)) {
//			fline connector{
//				(*it)->getEndPosition(),
//				(*std::next(it))->getStartPosition()
//			};
//			color tint = ORANGE;
//			tint.a = 0.5f;
//			renderer.draw(connector, VisualAppearance{.fillColor = tint}, transform);
//		}
//
//		color tint = PURPLE;
//		tint.a = 0.5f;
//        renderer.draw(ptr->getSOF(), VisualAppearance{.fillColor = tint}, transform);
	}

	renderer.draw(getCursorPosition(), 0.1f, VisualAppearance{.fillColor = RED}, transform);
}

void GameManager::setMap(MapInfoP map)
{
    info = std::move(map);
    activeObjects.clear();

    if (info) {
		HitObjectArguments args;
		args.mapSeed = 0;

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
				args.comboSeed++;
			args.objectSeed++;
        }
    }
}

void GameManager::reset()
{
    log::debug("Resetting");
    if (info)
        currentTime = info->getStartOffset();
    else
        currentTime = 0.0;

    for (auto &object: activeObjects) {
        object->reset();
    }

    last = activeObjects.begin();

	auto& skin = GetContext().activeSkin;

	samples.hit = skin->getSound(HIT_SOUND);
	samples.miss = skin->getSound(MISS_SOUND);
	samples.sliderBounce = skin->getSound(SLIDER_BOUNCE_SOUND);
	samples.sliderSlide = skin->getSound(SLIDER_SLIDE_SOUND);
	samples.sliderBreak = skin->getSound(SLIDER_BREAK_SOUND);
	samples.spinnerSwoosh = skin->getSound(SPINNER_SWOOSH_SOUND);
	samples.spinnerDing = skin->getSound(SPINNER_DING_SOUND);
}

const frect &GameManager::getPlayField() const
{ return playField; }

void GameManager::setPlayField(const frect &field)
{
    playField = field;
    auto smaller = Min(field.size.w, field.size.h);
    transform = MakeScaleMatrix(fvec2d(smaller, smaller)) *
        MakeTranslationMatrix(field.position);
}

fvec2d GameManager::getCursorPosition() const
{
    auto pos = GetContext().mouse.position();
    pos -= playField.position;
    auto smaller = Min(playField.size.w, playField.size.h);
    pos /= fvec2d{smaller, smaller};
    return pos;
}

MapInfoP GameManager::getMap() const
{ return info; }

const Mat3f &GameManager::getTransform() const
{ return transform; }

float GameManager::getCircleSize()
{
    return info->getCircleSize(); /* * multiplier */
}

float GameManager::getApproachTime()
{
    return info->getApproachTime(); /* * multiplier */
}

float GameManager::getFadeTime()
{
    return info->getFadeTime(); /* * multiplier */
}

float GameManager::getHitWindow()
{
    return info->getHitWindow(); /* * multiplier */
}

const SampleSet &GameManager::getSamples() const
{
	return samples;
}

bool GameManager::isFinished() const
{
	return last == activeObjects.end();
}

bool GameManager::resolveFunction(HitObjectFunction func, const BaseHitObject& object) const
{
	if (func == HitObjectFunction::NoActivation)
		return false;

	HitObjectFunction buttonRules = func & HitObjectFunction::ButtonMask;
	HitObjectFunction cursorRules = func & HitObjectFunction::CursorMask;

	bool buttonValid = true;
	bool cursorValid = true;

	auto& keyboard = GetContext().keyboard;

	auto SOF = object.getSOF();

	switch (buttonRules) {
	case HitObjectFunction::ButtonPressed:
		buttonValid = (keyboard[Key::Z].pressing && !keyboard[Key::X].pressed) ||
			          (keyboard[Key::X].pressing && !keyboard[Key::Z].pressed);
		break;
	case HitObjectFunction::ButtonHeld:
		buttonValid = (keyboard[Key::Z].pressed != keyboard[Key::X].pressed);
		break;
	case HitObjectFunction::ButtonReleased:
		buttonValid = (keyboard[Key::Z].released && keyboard[Key::X].released);
		break;
	case HitObjectFunction::ButtonPressedNoLock:
		buttonValid = (keyboard[Key::Z].pressing || keyboard[Key::X].pressing);
		break;
	case HitObjectFunction::ButtonHeldNoLock:
		buttonValid = (keyboard[Key::Z].pressed || keyboard[Key::X].pressed);
		break;
	case HitObjectFunction::ButtonIgnore:
	default:
		break;
	}

	switch (cursorRules) {
	case HitObjectFunction::CursorEnter:
		cursorValid = Distance(SOF.position, getCursorPosition()) <= SOF.radius;
		break;
	case HitObjectFunction::CursorLeave:
		cursorValid = Distance(SOF.position, getCursorPosition()) > SOF.radius;
		break;
	case HitObjectFunction::CursorIgnore:
	default:
		break;
	}

	return buttonValid && cursorValid;
}

NS_END