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
#include "GameTask.hpp"

namespace PROJECT_NAMESPACE
{

int GameTask::operator()()
{
    if (running) {
        auto d = timing.getDelta();
        game.update(d);

        if (game.isFinished()) {
            running = false;
        }

        timing.await();
    }
	return 0;
}

void GameTask::receive(const ConfigureGameRuntime &msg)
{
    timing.setFramerate(msg.updateRate);
}

bool GameTask::receive(const StartGame &msg)
{
    if (!game.setMap(msg.map)) {
        running = false;
        return false;
    }
    game.reset();
    running = true;
    return true;
}

bool GameTask::receive(const StopGame &msg)
{
    running = false;
    return true;
}

bool GameTask::receive(const IsRunning &) const
{
    return running;
}

}
