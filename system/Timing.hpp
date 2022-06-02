#pragma once

#include <chrono>
#include "../define.hpp"

namespace GAME_TITLE {

    class Timing {
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

} // oe
