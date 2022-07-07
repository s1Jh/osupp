#include "Timing.hpp"

#include <thread>

NS_BEGIN

constexpr unsigned int DEFAULT_FRAMERATE = 60;

Timing::Timing()
    : m_dDelta(0.0)
{
    m_tNow = m_tLast = std::chrono::steady_clock::now();
    m_dFrameTime = 1.0 / DEFAULT_FRAMERATE;
}

void Timing::await()
{
    int delay = int((m_dFrameTime - m_dDelta) * 1000.0);
    if (delay > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

double Timing::getDelta()
{
    m_tNow = std::chrono::steady_clock::now();
    std::chrono::duration<double> secs = m_tNow - m_tLast;
    m_dDelta = secs.count();
    m_tLast = m_tNow;
    return m_dDelta;
}

void Timing::setFramerate(int new_fps)
{
    if (new_fps != 0)
        m_dFrameTime = 1.0 / (float) new_fps;
}

void Timing::setTime(double frame_time)
{ m_dFrameTime = frame_time; }

double Timing::getTime()
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(
        m_tNow.time_since_epoch())
                      .count()) /
        1000;
}

NS_END
