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
#include "AutoPilot.hpp"
#include "BaseHitObject.hpp"

NS_BEGIN

bool AutoPilot::isKeyPressed(InputMapper::BlockMode) const
{
	return held;
}

bool AutoPilot::isKeyReleased() const
{
	return !held;
}

bool AutoPilot::isKeyPressing(InputMapper::BlockMode) const
{
	return held;
}

fvec2d AutoPilot::getCursor() const
{
	return position;
}

void AutoPilot::update()
{
	auto next = ctx.game.getClosestActiveObject();
	auto& objects = ctx.game.getStoredObjects();
	if (next == objects.end())
		return;

	auto thisPtr = *next;

	target = thisPtr->getSOF().position;
	held = ctx.game.getCurrentTime() >= thisPtr->getStartTime();

	auto direction = target - position;
	auto normalized = Normalize(direction);

	auto currentObjectStart = next->get()->getStartTime();
	fvec2d nextObjectPosition = {0.0f, 0.0f};
	auto previousObjectEnd = currentObjectStart - 1.0;
	BaseHitObject *from = nullptr;
	if (next != objects.begin()) {
		from = std::prev(next)->get();
		previousObjectEnd = from->getEndTime();
		nextObjectPosition = from->getSOF().position;
	}

	auto time = currentObjectStart - previousObjectEnd;
	auto distance = Distance(nextObjectPosition, target);

	float velocity;
	if (held)
		velocity = 100.f;
	else {
		velocity = Clamp(float(distance / time), minVelocity.get(), maxVelocity.get());
	}

	auto move = normalized * velocity * ctx.timing.getDelta();

	move.x = Clamp(move.x, direction.x, -direction.x);
	move.y = Clamp(move.y, direction.y, -direction.y);

	position += move;

	if (Distance(position, thisPtr->getSOF().position) >= thisPtr->getSOF().radius) {
		if (held) {
			log::debug("Lagged behind!");
			position = target;
		}
	}
}

AutoPilot::AutoPilot() :
	ctx(GetContext())
{
	minVelocity = ctx.settings.addSetting<float>(
		"setting.input.autopilot.min_speed", 5.0f,
		SettingFlags::HIDDEN | SettingFlags::WRITE_TO_FILE | SettingFlags::READONLY, 0.0f, 20.f);
	maxVelocity = ctx.settings.addSetting<float>(
		"setting.input.autopilot.max_speed", 100.0f,
		SettingFlags::HIDDEN | SettingFlags::WRITE_TO_FILE | SettingFlags::READONLY, 20.f, 100.f);
}

NS_END