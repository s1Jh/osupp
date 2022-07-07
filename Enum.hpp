#pragma once

#include <cstdint>

#include "EnumOperators.hpp"
#include "define.hpp"

NS_BEGIN

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
    None = 0,
    ComboEnd = 1 << 0
};

constexpr HitObjectParams OBJECT_NO_PARAMS = static_cast<HitObjectParams>(0);

ENABLE_BITMASK_OPERATORS(HitObjectParams)

enum class SliderType
{
    Straight, Circle, Catmull, Bezier
};

enum class LoadLayers
{
    Meta = 1 << 0,
    Objects = 1 << 1,
    Event = 1 << 2,
    Media = 1 << 3
};

constexpr LoadLayers MAP_LOAD_ALL = static_cast<LoadLayers>(0xf);

ENABLE_BITMASK_OPERATORS(LoadLayers)

enum class HitResult
{
    Missed = 0, Hit50, Hit100, Hit300
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
{ // Vis | Interactive | Interacting | Render  | AR
    // circle
    Invisible,   // object is not yet rendered or updated 0    X             X 0 0
    Approaching, // the current startTime is within this object's approach window
    // 1    0             X             fade-in    1
    Ready, // object is ready to be pressed 1    1             ⮭¹            1
    // 1
    Active, // object has been pressed on and is being held 1    1             1
    // 1          0
    Inactive, // object has been pressed on, but is not being held 1    1 0 1 0
    Pickup,   // object is waiting for the final ranking to be picked up 1    0 X
    // fade-out   0
    Fading, // object is no longer active, but is still fading away from the
    // screen     1    0             X             fade-out   0
};
/**
 * 1) Transition happens on the first click.
 */

NS_END