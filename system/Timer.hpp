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
