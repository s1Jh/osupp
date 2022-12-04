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

#include "define.hpp"

#define USER_CONTEXT_INCLUDES
#include "config.hpp"

#include "StateHandler.hpp"
#include "Renderer.dpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Timing.hpp"
#include "Locale.hpp"
#include "Settings.hpp"
#include "AudioDevice.hpp"
#include "Files.hpp"

NS_BEGIN

struct Context
{
	AudioDevice audio;
    StateHandler state;
    Renderer gfx;
    Keyboard keyboard;
    Mouse mouse;
    Timing timing;
    Settings settings;
    files::MultiDirectorySearch paths;

#define CONTEXT_FIELD(Type, Name) Type Name;
    USER_CONTEXT_FIELDS
#undef CONTEXT_FIELD
};

Context &GetContext();

NS_END