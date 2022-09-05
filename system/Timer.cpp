#include "Timer.hpp"

#include "Log.hpp"

NS_BEGIN

namespace detail
{

std::thread TimerControl::TimerThread;
std::vector<Timer*> TimerControl::ActiveTimers;
Timing TimerControl::Time;
std::mutex TimerControl::TimersMutex;
bool TimerControl::ShouldRun{true};

TimerControl::TimerControl(Timer* timer)
{
    std::scoped_lock timers_lock { TimersMutex };

    log::debug("Adding timer ", ActiveTimers.size());
    ActiveTimers.push_back(timer);
}

TimerControl::~TimerControl()
{
    ActiveTimers.erase(
        std::remove_if(ActiveTimers.begin(), ActiveTimers.end(),
        [this] (auto* timer) { return timer == this; }
    ), ActiveTimers.end());
}

void TimerControl::TimerRunner()
{
    Time.setTime( TIMER_RESOLUTION );
    while (ShouldRun)
    {
        Time.await();
        double delta = Time.getDelta();
        {
		std::scoped_lock timers_lock { TimersMutex };

        for (auto* timer : ActiveTimers)
        {
            if (timer->isRunning())
            {
                if (timer->increment(delta))
                {
                    timer->invokeCallback<TimerCallbacks::TimerDone>( timer );
                    switch (timer->getMode())
                    {
                    case TimerMode::Repeat:
						timer->start();
                        break;

                    case TimerMode::Single:
						timer->stop();
                        break;
                    case TimerMode::Manual:
					default:
                        break;
                    }
                }
            }
        }
        }
    }

	log::debug("[TIMER] Quitting timer thread");
}

}

bool StartTimerThread()
{
	detail::TimerControl::ShouldRun = true;
	detail::TimerControl::TimerThread = std::thread(detail::TimerControl::TimerRunner);
	return detail::TimerControl::TimerThread.joinable();
}

void StopTimerThread()
{
	detail::TimerControl::ShouldRun = false;
	detail::TimerControl::TimerThread.join();
}

Timer::Timer() :
	TimerControl ( this ), time (0 ), timer (0 ),
	running (false ), mode(TimerMode::Repeat ), wasDone (false )
{}

Timer::Timer(double time, TimerMode mode, bool start) :
	TimerControl ( this ), time (time ), timer (0 ),
	running (start ), mode(mode ), wasDone (false )
{}

Timer::Timer(Timer& other) :
	Callbacks(other), TimerControl ( this ), wasDone (false )
{
	time = (double)other.time;
	timer = (double)other.timer;
	running = (bool)other.running;
	mode = (TimerMode)other.mode;
}

Timer::Timer(Timer&& other) noexcept :
	Callbacks(std::move(other)), TimerControl ( this ), wasDone (false )
{
	time = (double)other.time;
	timer = (double)other.timer;
	running = (bool)other.running;
	mode = (TimerMode)other.mode;
}

Timer& Timer::operator=(const Timer& other)
{
	time = (double)other.time;
	timer = (double)other.timer;
	running = (bool)other.running;
	mode = (TimerMode)other.mode;

    return *this;
}

Timer& Timer::operator=(Timer&& other) noexcept
{
	time = (double)other.time;
	timer = (double)other.timer;
	running = (bool)other.running;
	mode = (TimerMode)other.mode;

    return *this;
}

bool Timer::isRunning()
{
    return running;
}

bool Timer::increment(double delta)
{
	timer += delta;

    if (!wasDone)
		wasDone = timer >= time;

    return wasDone;
}

void Timer::pause()
{
	running = false;
}

void Timer::resume()
{
	running = true;
}

void Timer::stop()
{
	running = false;
	timer = 0.0;
}

void Timer::start()
{
	running = true;
	timer = 0.0;
}

void Timer::reset()
{
	timer = 0.0;
}

void Timer::addTime(double timeIn)
{
	time += timeIn;
}

void Timer::setTime(double timeIn)
{
	time = timeIn;
}

void Timer::setMode(TimerMode modeIn)
{
	mode = modeIn;
}

double Timer::getCurrentTime()
{
    return timer;
}

double Timer::getRemainingTime()
{
    return time - timer;
}

double Timer::getTime()
{
    return time;
}

bool Timer::isDone()
{
    bool done = wasDone;
	wasDone = false;
    return done;
}

TimerMode Timer::getMode()
{
    return mode;
}

NS_END
