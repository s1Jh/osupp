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
// Created by sijh on 30.08.22.
//

#include "HumanInput.hpp"
#include "Context.hpp"
#include "Random.hpp"

NS_BEGIN

bool HumanInput::isKeyPressed(BlockMode mode) const
{
	int found = 0;

//	for (const auto code : activationKeys) {
//		// check each key to see if it is pressed
//		if (GetContext().keyboard[code].pressed) {
//			if (mode == NO_BLOCKING)
//				return true;
//
//			found++;
//		}
//	}

	return found == 1;
}

bool HumanInput::isKeyPressing(BlockMode mode) const
{
	bool found = false;

//	for (const auto code : activationKeys) {
//		// check each key to see if it is pressed
//		const auto& key = GetContext().keyboard[code];
//		if (key.pressing) {
//			if (mode == NO_BLOCKING) {
//				// if blocking is off, we just return true the moment we find a pressed key
//				return true;
//			}
//			found = true;
//		} else if (key.pressed && mode == BLOCKING) {
//			return false;
//		}
//	}

	return found;
}

bool HumanInput::isKeyReleased() const
{
//	for (const auto code : activationKeys) {
//		const auto& key = GetContext().keyboard[code];
//		if (key.pressed || key.pressing)
//			return false;
//	}
	return true;
}

fvec2d HumanInput::getCursor() const
{
//	const auto& ctx = GetContext();
//	const auto& playField = ctx.game.getPlayField();

//	fvec2d pos = ctx.mouse.position();
//	pos -= playField.position;
//	auto smaller = Min(playField.size.w, playField.size.h);
//	pos /= fvec2d{smaller, smaller};
//	return pos;

    return {};
}

void HumanInput::update()
{

}

HumanInput::HumanInput() :
	activationKeys({Key::X, Key::Z})
{}

NS_END