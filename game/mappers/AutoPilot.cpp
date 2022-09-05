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

//
// Created by sijh on 04.09.22.
//

#include "AutoPilot.hpp"
#include "BaseHitObject.hpp"

#include "imgui.h"

NS_BEGIN

bool AutoPilot::isKeyPressed(InputMapper::BlockMode mode) const
{
	return held;
}

bool AutoPilot::isKeyReleased() const
{
	return !held;
}

bool AutoPilot::isKeyPressing(InputMapper::BlockMode mode) const
{
	return held != lastHeld;
}

fvec2d AutoPilot::getCursor() const
{
	return position;
}

void AutoPilot::update()
{
	lastHeld = held;

	if (ImGui::Begin("Autopilot control")) {

		ImGui::SliderFloat("Velocity", &velocity, 0.0f, 50.f);

		ImGui::Checkbox("Held", &held);

		ImGui::End();
	}

	auto thisPtr = ctx.game.getCurrentObject().lock();

	if (!thisPtr)
		return;

	if (thisPtr->isFinished()) {
		if (auto nextPtr = ctx.game.getNextObject().lock()) {
			thisPtr = nextPtr;
			target = nextPtr->getSOF().position;
		}
	} else {
		target = thisPtr->getSOF().position;
	}

	held = ctx.game.getCurrentTime() >= thisPtr->getStartTime();
	bool inObject = Distance(position, thisPtr->getSOF().position) < thisPtr->getSOF().radius;

	if (held && !inObject) {
		log::debug("Lagged behind!");
		position = target;
	}

	auto direction = target - position;
	auto normalized = Normalize(direction);

	auto move = normalized * velocity * ctx.timing.getDelta();

	move.x = Clamp(move.x, direction.x, -direction.x);
	move.y = Clamp(move.y, direction.y, -direction.y);

	position += move;
}

AutoPilot::AutoPilot() :
	ctx(GetContext())
{
	previous = ctx.game.getCurrentObject();
}

NS_END