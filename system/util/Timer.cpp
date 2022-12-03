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
                    timer->invokeCallback<TimerCallbacks::TIMER_DONE>( timer );
                    switch (timer->getMode())
                    {
                    case TimerMode::REPEAT:
						timer->start();
                        break;

                    case TimerMode::SINGLE:
						timer->stop();
                        break;
                    case TimerMode::MANUAL:
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
	running (false ), mode(TimerMode::REPEAT ), wasDone (false )
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
