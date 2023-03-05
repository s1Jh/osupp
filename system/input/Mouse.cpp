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

#include "Mouse.hpp"

#include "Math.hpp"

#include <SDL2/SDL_mouse.h>

#include "Context.hpp"

namespace PROJECT_NAMESPACE::input {

const ButtonState &Mouse::left() const
{ return buttons[0]; }

const ButtonState &Mouse::middle() const
{ return buttons[1]; }

const ButtonState &Mouse::right() const
{ return buttons[2]; }


fvec2d Mouse::position() const
{
    ivec2d ret;
    SDL_GetMouseState(&ret[0], &ret[1]);
    auto* window = SDL_GetMouseFocus();
    isize size;
    SDL_GetWindowSize(window, &size.w, &size.h);

    auto shorter = double(math::Min(size.w, size.h)) / 2.0;

    ret -= dvec2d(size / 2);
    ret /= dvec2d{shorter, shorter};
    ret[1] *= -1.0f;

    return ret;
}

void Mouse::update()
{
    ivec2d ret;
    auto buttonStates = SDL_GetMouseState(&ret[0], &ret[1]);

    for (size_t i = 0; i < 3; i++) {

        bool state = SDL_BUTTON(i+1) & buttonStates;

        auto &button = buttons[i];

        // finish previous pressing/releasing ops
        if (button.releasing) {
            button.releasing = false;
            button.released = true;
        }
        if (button.pressing) {
            button.pressing = false;
            button.pressed = true;
        }

        // queue the new ones
        if (state && button.released) {
            button.released = false;
            button.pressing = true;
        }
        if (!state && button.pressed) {
            button.pressed = false;
            button.releasing = true;
        }
    }
}

}