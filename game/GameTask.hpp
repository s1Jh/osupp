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
// Created by sijh on 05.03.23.
//
#pragma once

#include "define.hpp"

#include "GameManager.hpp"
#include "Timing.hpp"
#include "NamedTask.hpp"
#include "MapInfo.hpp"
#include "Resource.hpp"

namespace PROJECT_NAMESPACE
{

struct ConfigureGameRuntime
{
    using ResultType = void;

    Resource<Skin> skin;
    unsigned int updateRate;
};

struct StartGame
{
    using ResultType = bool;

    Resource<MapInfo> map;
};

struct StopGame
{
    using ResultType = bool;
};

struct IsRunning
{
    using ResultType = bool;
};


struct GameTask
{
    int operator() ();

    void receive(const ConfigureGameRuntime& msg);
    bool receive(const StartGame& msg);
    bool receive(const StopGame& msg);
    bool receive(const IsRunning& msg) const;

    GameManager game;
    Timing timing;
    bool running{false};
};

MAKE_NAMED_TASK(GAME, "game", GameTask, int);

}
