#pragma once

#include "define.hpp"
#include <chrono>

NS_BEGIN

constexpr unsigned int DEFAULT_FRAMERATE = 60;

class Timing
{
public:
    Timing();

    void setFramerate(int newFps);

    void setTime(double newFrameTime);

    void await();

    double getDelta();

    double getTime();

private:
    using Clock = std::chrono::steady_clock;
    std::chrono::time_point<Clock> now;
    std::chrono::time_point<Clock> last;
    double delta;
    bool phase;
    double frameTime;
};

NS_END
