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

	auto currentObjectEnd = next->get()->getEndTime();
	auto nextObjectPosition = target;
	auto nextObjectStart = currentObjectEnd + 1.0;
	BaseHitObject *to = nullptr;
	if (next != std::prev(objects.end())) {
		to = std::next(next)->get();
		nextObjectStart = to->getStartTime();
		nextObjectPosition = to->getSOF().position;
	}

	auto time = nextObjectStart - currentObjectEnd;
	auto distance = Distance(nextObjectPosition, target);

	float velocity;
	if (held)
		velocity = 100.f;
	else
		velocity = distance / time;

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

	/*auto currentObjectEnd = next->get()->getEndTime();
	auto nextObjectStart = currentObjectEnd + 1.0;
	BaseHitObject *to = nullptr;
	if (next != std::prev(objects.end())) {
		to = std::next(next)->get();
		nextObjectStart = std::next(next)->get()->getStartTime();
	}

	auto delta = nextObjectStart - currentObjectEnd;

	auto direction = target - position;

	acceleration = (direction * accMult) / delta;

	velocity *= drag;
	velocity += acceleration * ctx.timing.getDelta();
	position += velocity * ctx.timing.getDelta();

	if (Distance(position, thisPtr->getSOF().position) >= thisPtr->getSOF().radius) {
		if (held) {
			log::debug("Lagged behind!");
			position = target;
			velocity = {0.0f, 0.0f};
		}
	}

	const auto& transform = ctx.game.getTransform();
	static float displayScale = 1.0f;*/

//	ctx.gfx.draw(fline{position, target}, VisualAppearance{.fillColor = WHITE}, transform);
//	ctx.gfx.draw(fline{position, position + acceleration * displayScale}, VisualAppearance{.fillColor = GREEN}, transform);
//	ctx.gfx.draw(fline{position, position + velocity * displayScale}, VisualAppearance{.fillColor = RED}, transform);

	if (ImGui::Begin("Autopilot control")) {
		ImGui::Text("Tracking: 0x%lx", thisPtr.get());
		ImGui::Text("Next: 0x%lx", to);
		ImGui::Text("Time to next: %f", time);
		ImGui::Text("Distance to next: %f", distance);
		ImGui::Text("Velocity: %f", velocity);
		ImGui::Text("Held: %i", held);

		/*ImGui::Text("Next: 0x%xl", to);
		ImGui::Text("Delay to next: %f", delta);
		ImGui::Text("Direction: %f %f", direction.x, direction.y);
		ImGui::Text("Target: %f %f", target.x, target.y);
		ImGui::Text("Position: %f %f", position.x, position.y);
		ImGui::Text("Velocity: %f %f", velocity.x, velocity.y);
		ImGui::Text("Acceleration: %f %f", acceleration.x, acceleration.y);
		ImGui::SliderFloat("Acceleration multiplier", &accMult, 0.0f, 100.0f);
		ImGui::SliderFloat("Drag", &drag, 0.0f, 1.0f);
		ImGui::SliderFloat("Scale", &displayScale, 0.0f, 1.0f);*/

		ImGui::End();
	}
}

AutoPilot::AutoPilot() :
	ctx(GetContext())
{
}

NS_END