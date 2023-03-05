//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#include "Time.hpp"

#include "date/tz.h"

#include <thread>
#include <chrono>
#include <cmath>

namespace chr = std::chrono;

namespace PROJECT_NAMESPACE
{

namespace time
{

using HRClock = chr::high_resolution_clock;
using SClock = chr::steady_clock;

thread_local SClock::time_point Last = SClock::now();
thread_local Time Delta = 0.0;
thread_local Time FrameTime = DEFAULT_FRAME_TIME;

static chr::high_resolution_clock::time_point StartTime = chr::high_resolution_clock::now();

Time SinceStart()
{
	return chr::duration<Time>(chr::high_resolution_clock::now() - StartTime).count();
}

Time Local()
{
	auto now = chr::system_clock::now();
	auto zn = date::make_zoned(date::current_zone(), now);

	return chr::duration_cast<chr::seconds>(zn.get_local_time().time_since_epoch()).count();
}

Time UTC(int offset)
{
	//return chr::duration<Time>(chr::utc_clock::now().time_since_epoch()).count();
	return 0;
}

std::string Format(const TimeComponents &t, const std::string &format)
{
	std::string ret = format;

	auto Replace = [&](const std::string &mark, std::string repl, char pad = 0)
	{
		size_t start_pos = 0;

		if (pad != 0) {
			while (repl.size() < mark.size()) {
				repl.insert(repl.begin(), pad);
			}
		}

		while ((start_pos = ret.find(mark)) != std::string::npos) {
			ret.replace(start_pos, mark.length(), repl);
		}
	};

	std::string AMPM = t.hours > 12 ? "am" : "pm";
	unsigned int hours12 = t.hours % 12;

	unsigned int year2 = t.years % 100;
	unsigned int monthIndex = t.months % 12;

	std::string weekDay;

	Replace("yyyy", std::to_string(t.years));
	Replace("YY", std::to_string(year2));
	Replace("MM", std::to_string(t.months));
//	Replace("MS", shortMonth);
//	Replace("ML", longMonth);
	Replace("dd", std::to_string(t.days));
	Replace("WD", weekDay);
	Replace("HH", std::to_string(t.hours), '0');
	Replace("hh", std::to_string(hours12));
	Replace("ap", AMPM);
	Replace("mm", std::to_string(t.minutes), '0');
	Replace("ss", std::to_string((int)std::floor(t.seconds)), '0');
	Replace("SS", std::to_string(t.seconds), '0');

	return ret;
}

std::string Format(Time t, const std::string &format)
{
	auto components = Split(t);
	return Format(components, format);
}

void SetFrameTime(Time t)
{
	FrameTime = t;
}

void SetFPS(FPS fps)
{
	if (fps != 0)
		FrameTime = 1.0 / (Time)fps;
}

Time GetDelta()
{
	return Delta;
}

Time Await()
{
	Time slept = 0.0;

	auto now = SClock::now();
	std::chrono::duration<double> secs = now - Last;
	Delta = secs.count();
	Last = now;

	slept = FrameTime - Delta;
	auto delay = long(slept * SECONDS_MILLISECONDS_RATIO);
	if (delay > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
	return slept;
}

TimeComponents Split(Time t)
{
	TimeComponents c;
	auto seconds = (unsigned)t;
	c.years = seconds / unsigned(YEARS_SECONDS_RATIO);
	seconds -= c.years * unsigned(YEARS_SECONDS_RATIO);

	c.weeks = seconds / unsigned(WEEKS_SECONDS_RATIO);
	seconds -= c.weeks * unsigned(WEEKS_SECONDS_RATIO);

	c.days = seconds / unsigned(DAYS_SECONDS_RATIO);
	seconds -= c.days * unsigned(DAYS_SECONDS_RATIO);

	c.hours = seconds / unsigned(HOURS_SECONDS_RATIO);
	seconds -= c.hours * unsigned(HOURS_SECONDS_RATIO);

	c.minutes = seconds / unsigned(MINUTES_SECONDS_RATIO);
	seconds -= c.minutes * unsigned(MINUTES_SECONDS_RATIO);

	c.seconds = ((t - std::floor(t)) + (Time)seconds);

	return c;
}

}

}
