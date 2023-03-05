// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 04.02.23.
//
#pragma once

#include "define.hpp"

#include "EnumOperators.hpp"

#include "Mouse.hpp"
#include "Keyboard.hpp"

#include <vector>
#include <type_traits>
#include <string>
#include <functional>

namespace PROJECT_NAMESPACE {

namespace gui
{

namespace detail
{

class IContainsElements;
class IContainsModals;
class IElement;
struct IPrototype;
template<typename T> requires std::is_base_of_v<IElement, T>
struct Prototype;

}

class Registry;
class Root;
class State;
class Collections;
class Modal;

typedef std::vector<std::string> sioArguments;

struct IO
{
    input::Mouse mouse;
    input::Keyboard keyboard;
};

struct sioContext
{
};

typedef std::function<bool(sioContext &, const sioArguments &)> sioFunc;

enum class Phase
{
    CLOSED,             // is ready to be shown, but isn't visible
    OPENING = 0b100,    // is in the opening animation
    OPEN = 0b01,        // is displaying itself
    CLOSING = 0b110,    // is in the closing animation

    IS_ANIMATING = 0b100
};

}

ENABLE_BITMASK_OPERATORS(gui::Phase)

}
