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
    activeObjects.push_back(object);                                           \
    break;                                                                      \
  }

NS_BEGIN

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

	auto active = getClosestActiveObject();

	// objects are sorted by order of appearance
	int updates = 0;
	printCounter++;
	auto start = std::chrono::high_resolution_clock::now();
	for (auto it = last;; it++) {
		if (it == activeObjects.end())
			return;

		auto &obj = *it;

		obj->update(delta);
		if (auto ptr = active.lock()) {
			if (ptr == obj)
				input->update();
		}

		updates++;
		switch (obj->getState()) {
		case HitObjectState::INVISIBLE: {
			if (!obj->isFinished()) {
				// The object is ahead of our field of vision, since objects are sorted
				// by order of appearance, all objects ahead of this one will also be
				// invisible. We therefore stop checking and return.
				goto breakout;
			}
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
		case HitObjectState::READY: {
			// The object can now be interacted with, check for user inputs and then
			// update the object accordingly.
			auto func = obj->getActivationFunction();

			if (resolveFunction(func, *obj))
			{
				// We are within the object's hit window and are pressing down on the
				// right keys, start.
				obj->begin();
				goto breakout;
			}

			break;
		}
		case HitObjectState::ACTIVE: {
			// object has been activated and is currently being held, check if it's
			// been released

			auto func = obj->getDeactivationFunction();

			if (resolveFunction(func, *obj))
			{
				log::debug("Raising");
				obj->raise();
			}
			break;
		}
		case HitObjectState::INACTIVE: {
			// object has been activated but is not being held, check if it's being
			// held again
			auto func = obj->getActivationFunction();

			if (resolveFunction(func, *obj))
			{
				log::debug("Pressing");
				obj->press();
			}
			break;
		}
			// The object cannot be interacted with in these states
		case HitObjectState::PICKUP: {
			auto score = obj->finish();
			log::info("SCORE: ", (int) score);
			break;
		}
		case HitObjectState::FADING:
		case HitObjectState::APPROACHING:
		default:
			break;
		}
	}

breakout:
	rollingUPF += updates;
	auto duration = std::chrono::high_resolution_clock::now() - start;
	auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	rollingUT += micros;

	if (printCounter >= 60) {
		averageUPF = double(rollingUPF) / 60.0;
		averageUT = double(rollingUT) / 60.0;

		history[historyInsertSpot] = averageUT;
		historyInsertSpot++;
		historyInsertSpot %= history.size();

		minUT = Min(minUT, averageUT);
		maxUT = Max(maxUT, averageUT);

		printCounter = 0;
		rollingUT = 0;
		rollingUPF = 0;
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

		ImGui::PlotLines("History (UT)", history.data(), history.size(), 0, nullptr, minUT, maxUT);

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
		return (obj->getState() == HitObjectState::INVISIBLE) && !obj->isFinished();
	});

	// render everything from the object before last to the current object

	// MSVC has a hissy fit every time we decrement a begin iterator, 
	// therefore we have to complicate things 
#ifdef WINDOWS
	if (end == activeObjects.begin())
		return;

	if (last != activeObjects.begin())
		last--;

	for (auto it = std::prev(end); it != last; it--) {
#else
	for (auto it = std::prev(end); it != std::prev(last); it--) {
#endif
		auto ptr = *it;
		ptr->draw();
	}
#ifdef WINDOWS
	if (last == activeObjects.begin()) {
		activeObjects.front()->draw();
	}
#endif
}


bool GameManager::setMap(Resource<MapInfo> map)
{
    info = std::move(map);
    activeObjects.clear();

	if (info) {
		lastLoadedObject = info->getObjectTemplates().begin();
		loadObjects(1000);
	}

	return true;
}

void GameManager::reset()
{
//    log::debug("Resetting the game state");

    if (info)
        currentTime = -info->getStartOffset();
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

Resource<MapInfo> GameManager::getMap() const
{ return info; }

const Mat3f &GameManager::getTransform() const
{ return transform; }

float GameManager::getCircleSize()
{
	if (info)
    	return info->getCircleSize() * csMultiplier;
	return 0.0f;
}

float GameManager::getApproachTime()
{
	if (info)
    	return info->getApproachTime() * arMultiplier;
	return 0.0f;
}

float GameManager::getFadeTime()
{
	if (info)
    	return info->getFadeTime() * ftMultiplier;
	return 0.0f;
}

float GameManager::getHitWindow()
{
	if (info)
    	return info->getHitWindow() * hwMultiplier;
	return 0.0f;
}

float GameManager::getHpDrain()
{
	if (info)
		return info->getHpDrain() * hpMultiplier;
	return 0.0f;
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
	if (!input)
		return false;

	if (func == HitObjectFunction::NO_ACTIVATION)
		return false;

	HitObjectFunction buttonRules = func & HitObjectFunction::BUTTON_MASK;
	HitObjectFunction cursorRules = func & HitObjectFunction::CURSOR_MASK;
	HitObjectFunction mergeRules = func & HitObjectFunction::MERGE_MASK;

	bool buttonValid = true;
	bool cursorValid = true;

	auto SOF = object.getSOF();

	switch (buttonRules) {
	case HitObjectFunction::BUTTON_PRESSED:
		buttonValid = input->isKeyPressing(InputMapper::BLOCKING);
		break;
	case HitObjectFunction::BUTTON_HELD:
		buttonValid = input->isKeyPressed(InputMapper::BLOCKING);
		break;
	case HitObjectFunction::BUTTON_RELEASED:
		buttonValid = input->isKeyReleased();
		break;
	case HitObjectFunction::BUTTON_PRESSED_NO_LOCK:
		buttonValid = input->isKeyPressing(InputMapper::NO_BLOCKING);
		break;
	case HitObjectFunction::BUTTON_HELD_NO_LOCK:
		buttonValid = input->isKeyPressed(InputMapper::NO_BLOCKING);
		break;
	case HitObjectFunction::BUTTON_IGNORE:
	default:
		break;
	}

	switch (cursorRules) {
	case HitObjectFunction::CURSOR_ENTER:
		cursorValid = Distance(SOF.position, input->getCursor()) <= SOF.radius;
		break;
	case HitObjectFunction::CURSOR_LEAVE:
		cursorValid = Distance(SOF.position, input->getCursor()) > SOF.radius;
		break;
	case HitObjectFunction::CURSOR_IGNORE:
	default:
		break;
	}

	switch (mergeRules) {
	case HitObjectFunction::AND:
		return buttonValid && cursorValid;
	case HitObjectFunction::OR:
		return buttonValid || cursorValid;
	case HitObjectFunction::XOR:
		return buttonValid + cursorValid;
	default:
		return false;
	}
}

float GameManager::getStartOffset()
{
	if (info)
		return info->getStartOffset();
	return 0.0f;
}

void GameManager::setInputMapper(std::unique_ptr<InputMapper> &&mapper)
{
	input = std::move(mapper);
}

std::weak_ptr<BaseHitObject> GameManager::getCurrentObject() const
{
	if (last != activeObjects.end()) {
		return *last;
	}
	return {};
}

std::weak_ptr<BaseHitObject> GameManager::getClosestActiveObject() const
{
	auto it = last;

	while (it != activeObjects.end()) {
		if (!(*it)->isFinished())
			return *it;
		it = std::next(it);
	}

	return {};
}

std::weak_ptr<BaseHitObject> GameManager::getNextObject() const
{
	if ((last != activeObjects.end()) && (std::next(last) != activeObjects.end())) {
		return *std::next(last);
	}
	return {};
}

const GameManager::StorageT &GameManager::getStoredObjects() const
{
	return activeObjects;
}

fvec2d GameManager::getCursorPosition() const
{
	if (input)
		return input->getCursor();
	return {0, 0};
}

GameManager::GameManager()
{
	last = activeObjects.begin();
}

unsigned int GameManager::loadObjects(unsigned int amount)
{
	HitObjectArguments args;
	args.mapSeed = 0;

	auto &templates = info->getObjectTemplates();

	if (templates.end() == lastLoadedObject)
		return 0;

	unsigned int loaded = 0;
	auto it = lastLoadedObject;
	for (it = lastLoadedObject; (it != templates.end()) && (loaded < amount); loaded++, it++) {
		const auto& objectTemplate = *it;

		switch (objectTemplate->getType()) {
		MAKE_CASE(Spinner)
		MAKE_CASE(Slider)
		MAKE_CASE(Note)
		default:
			info = nullptr;
			log::warning("Corrupted map template: ", objectTemplate);
			return false;
		}

		if (bool(objectTemplate->parameters & HitObjectParams::COMBO_END))
			args.comboSeed++;
		args.objectSeed++;
	}

	lastLoadedObject = it;

	if (lastLoadedObject != templates.end())
		lastLoadedObject++;

	log::debug("Loaded ", loaded, " objects");

	return loaded;
}

void GameManager::skipToFirst()
{
	currentTime = (*last)->getStartTime() - 3.0f;
}

void GameManager::scrobble(double amount)
{
	setCurrentTime(currentTime + amount);
}

NS_END