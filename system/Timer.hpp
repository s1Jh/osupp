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

#include <chrono>
#include <vector>
#include <thread>

#include "CallbackManager.hpp"
#include "Timing.hpp"

NS_BEGIN

enum class TimerCallbacks
{
    TIMER_DONE
};

enum class TimerMode
{
    SINGLE, REPEAT, MANUAL
};

class Timer;

bool StartTimerThread();
void StopTimerThread();

namespace detail
{

constexpr double TIMER_RESOLUTION = 0.0001;

class TimerControl
{
	friend bool ::PROJECT_NAMESPACE::StartTimerThread();
	friend void ::PROJECT_NAMESPACE::StopTimerThread();
protected:
    explicit TimerControl(Timer* timer);
    ~TimerControl();

    static void TimerRunner();

    static Timing Time;
    static std::thread TimerThread;
    static std::vector<Timer*> ActiveTimers;
	static std::mutex TimersMutex;
	static bool ShouldRun;
};

}

class Timer : public detail::Callbacks<TimerCallbacks>, public detail::TimerControl
{
    friend class TimerControl;
public:
    Timer();
    explicit Timer(double time, TimerMode mode = TimerMode::REPEAT, bool start = false);

    Timer(Timer&);
    Timer(Timer&&) noexcept;

    Timer& operator=(const Timer&);
    Timer& operator=(Timer&&) noexcept;

    void pause();
    void resume();

    void stop();
    void start();

    void reset();
    void addTime(double time);
    void setTime(double time);
    void setMode(TimerMode mode);

    bool isRunning();
    double getCurrentTime();
    double getRemainingTime();
    double getTime();
    bool isDone();
    TimerMode getMode();

protected:
    bool increment(double delta);

//    std::recursive_mutex timerMutex;
    std::atomic<double> time;
	std::atomic<double> timer;
	std::atomic<bool> running;
	std::atomic<TimerMode> mode;
	std::atomic<bool> wasDone;
};

MAKE_CALLBACK(TimerCallbacks, TimerCallbacks::TIMER_DONE, Timer*)

NS_END
