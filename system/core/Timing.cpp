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

#include "Timing.hpp"

#include <thread>

NS_BEGIN

Timing::Timing() noexcept
    : delta(0.0), phase(false)
{
    now = last = std::chrono::steady_clock::now();
    frameTime = 1.0 / DEFAULT_FRAMERATE;
}

void Timing::await()
{
    if (!phase) {
        int delay = int((frameTime - delta) * 1000000.0);
        if (delay > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(delay));
        }
        phase = true;
    }
}

double Timing::getDelta()
{
    if (phase) {
        now = Clock::now();
        std::chrono::duration<double> secs = now - last;
        delta = secs.count();
        last = now;
        phase = false;
    }
    return delta;
}

void Timing::setFramerate(int newFps)
{
    if (newFps != 0)
        frameTime = 1.0 / (float) newFps;
}

void Timing::setTime(double newFrameTime)
{ frameTime = newFrameTime; }

double Timing::getTime() const
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch())
                      .count()) / 1000.0;
}

NS_END
