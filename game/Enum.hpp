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

#include <cstdint>

#include "EnumOperators.hpp"
#include "define.hpp"

namespace PROJECT_NAMESPACE {

/*====================================================================================================================*/
/*  Enumerations */
/*--------------------------------------------------------------------------------------------------------------------*/
enum class HitObjectType: uint8_t
{
    None,
    Note,
    Slider,
    Spinner,
};

enum class HitObjectParams: uint8_t
{
    NONE = 0,
    COMBO_END = 1 << 0
};

constexpr HitObjectParams OBJECT_NO_PARAMS = static_cast<HitObjectParams>(0);

ENABLE_BITMASK_OPERATORS(HitObjectParams)

enum class SliderType
{
    STRAIGHT, CIRCLE, CATMULL, BEZIER
};

enum class LoadLayers
{
    META = 1 << 0,
    OBJECTS = 1 << 1,
    EVENT = 1 << 2,
    MEDIA = 1 << 3
};

constexpr LoadLayers MAP_LOAD_ALL = static_cast<LoadLayers>(0xf);

ENABLE_BITMASK_OPERATORS(LoadLayers)

enum class HitResult
{
    MISSED = 0, HIT50, HIT100, HIT300
};

/**
 * Each object will circle through these states in the following order:
 *                                             Inactive -⟍
 *                                               𝛿 ↕      ↓
 *   Invisible -α-> Approaching -β-> Ready -γ-> Active ---𝜀-> Fading -𝜁-> Pickup
 * -η-> Invisible (finished) ⟍_________________________↗ States: Invisible =
 * Object is not currently visible on the screen and thus cannot be interacted
 * with. Overridden onLogicUpdate method is not called during this state.
 *   Approaching = Object became visible, but cannot yet be interacted with.
 *   Ready = Object is visible and can be interacted with.
 *   Active = Object is being interacted with and points are being gained.
 *   Inactive = Object has been interacted with once, but is not being held
 * right now. Fading = Object is fading away. Pickup = Invisible = Object is
 * again invisible, it has been passed.
 *
 * Transitions:
 *   α - [TIMED] currentTime >= startTime - approachTime.
 *   β - [TIMED] currentTime >= startTime - hitWindow.
 *   γ - [PLAYER] Player clicking down for the first time (initial hit), begin()
 * called. 𝛿 - [PLAYER] Player letting go / pressing down on the object,
 * press(), raise() called respectively. 𝜀 - [TIMED] currentTime >= endTime. 𝜁 -
 * [UPDATE] currentTime >= endTime + hitWindow (fading time). η - [TIMED]
 * finish() called to get the object rank.
 */
enum class HitObjectState
{
    INVISIBLE,   // object is not yet rendered or updated 0    X             X 0 0
    APPROACHING, // the current startTime is within this object's approach window
    // 1    0             X             fade-in    1
    READY, // object is ready to be pressed 1    1             ⮭¹            1
    // 1
    ACTIVE, // object has been pressed on and is being held 1    1             1
    // 1          0
    INACTIVE, // object has been pressed on, but is not being held 1    1 0 1 0
    PICKUP,   // object is waiting for the final ranking to be picked up 1    0 X
    // fade-out   0
    FADING, // object is no longer active, but is still fading away from the
    // screen     1    0             X             fade-out   0
};
/**
 * 1) Transition happens on the first click.
 */

enum class HitObjectFunction : uint8_t
{
	BUTTON_MASK = 0b0111,		// 3 bits - 3
	BUTTON_IGNORE = 0,			// buttons will not contribute to decision-making
	BUTTON_PRESSED = 1,			// activated when a button is pressed down
	BUTTON_HELD = 2,			// activated if a button is already held down
	BUTTON_RELEASED = 3,		// activated when a button is released
	BUTTON_LOCK_MASK = 0b100,
	BUTTON_PRESSED_NO_LOCK = 4,	// ditto, no locking
	BUTTON_HELD_NO_LOCK = 5,	//

	CURSOR_MASK = 0b11000,		// 2 bits - 5
	CURSOR_IGNORE = 0 << 3,		// cursor position does not contribute to decision-making
	CURSOR_LEAVE = 1 << 3,		// true when cursor is not in the object's SOF
	CURSOR_ENTER = 2 << 3,		// true when the cursor is inside the object's SOF

	MERGE_MASK = 0b1100000,		// 2 bits - 7
	AND = 0 << 5,				// button and cursor values will be AND-ed together
	OR = 1 << 5,				// button and cursor values will be OR-ed together
	XOR = 2 << 5,				// button and cursor values will be XOR-ed together

	NO_ACTIVATION = 0xff		// will always resolve to false
};

ENABLE_BITMASK_OPERATORS(HitObjectFunction)

}