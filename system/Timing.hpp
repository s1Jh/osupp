#pragma once

#include "../define.hpp"
#include <chrono>

NS_BEGIN

class Timing
{
public:
    Timing();

    void setFramerate(int new_fps);

    void setTime(double frame_time);

    void await();

    double getDelta();

    double getTime();

private:
    std::chrono::time_point<std::chrono::steady_clock> m_tNow;
    std::chrono::time_point<std::chrono::steady_clock> m_tLast;
    double m_dDelta;
    double m_dFrameTime;
};

NS_END
