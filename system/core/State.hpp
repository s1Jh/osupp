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

NS_BEGIN

class Renderer;

#define ALL_STATES \
    USER_STATE(NONE, = 0x100) \
    USER_STATES \
    USER_STATE(EXIT, = 0xffffffff)

enum class GameState: unsigned int
{
#define USER_STATE(Name, ...) Name __VA_ARGS__,
    ALL_STATES
#undef USER_STATE
};

namespace detail
{

class BaseState
{
public:
    virtual int update(double);
    virtual int draw();
    virtual int exit();
    virtual int init(GameState);

    virtual ~BaseState() = default;
};
}

template<GameState Name>
class State
{
};

#define DECLARE_STATE(StateName) \
    template<> class State<StateName> : public detail::BaseState

NS_END