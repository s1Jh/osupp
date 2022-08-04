#include "Timing.hpp"

#include <thread>

NS_BEGIN

Timing::Timing()
    : delta(0.0), phase(false)
{
    now = last = std::chrono::steady_clock::now();
    frameTime = 1.0 / DEFAULT_FRAMERATE;
}

void Timing::await()
{
    if (!phase) {
        int delay = int((frameTime - delta) * 1000.0);
        if (delay > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
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

double Timing::getTime()
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch())
                      .count()) / 1000.0;
}

NS_END
