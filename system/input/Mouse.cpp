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

#define GLFW_DLL

#include <GLFW/glfw3.h>

NS_BEGIN

const ButtonState &Mouse::left() const
{ return buttons[0]; }

const ButtonState &Mouse::middle() const
{ return buttons[2]; }

const ButtonState &Mouse::right() const
{ return buttons[1]; }

fvec2d Mouse::position() const
{
    dvec2d ret;
    glfwGetCursorPos(TO_GLFW(parentViewport), &ret.x, &ret.y);

    isize size;
    glfwGetWindowSize(TO_GLFW(parentViewport), &size.w, &size.h);

    auto shorter = double(Min(size.w, size.h)) / 2.0;

    ret -= dvec2d(size / 2);
    ret /= dvec2d{shorter, shorter};
    ret.y *= -1.0f;

    return ret;
}

void Mouse::setViewport(WindowHandle *n)
{ parentViewport = n; }

void Mouse::update()
{
    for (size_t i = 0; i < 3; i++) {
        bool state = glfwGetMouseButton(TO_GLFW(parentViewport), GLFW_MOUSE_BUTTON_1 + i);

        auto &button = buttons[i];

        // onFinish previous pressing/releasing ops
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

NS_END