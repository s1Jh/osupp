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
#pragma once

#include "define.hpp"

#include <string>

namespace PROJECT_NAMESPACE
{

namespace time
{

typedef unsigned int FPS;
typedef double Time;

constexpr Time DEFAULT_FRAME_TIME = 0.016;

constexpr double YEARS_SECONDS_RATIO = 3600 * 24 * 365;
constexpr double WEEKS_SECONDS_RATIO = 3600 * 24 * 7;
constexpr double DAYS_SECONDS_RATIO = 3600 * 24;
constexpr double HOURS_SECONDS_RATIO = 3600;
constexpr double MINUTES_SECONDS_RATIO = 60;
constexpr double MILLISECONDS_SECONDS_RATIO = 1e-3;
constexpr double MICROSECONDS_SECONDS_RATIO = 1e-6;
constexpr double NANOSECONDS_SECONDS_RATIO = 1e-9;
constexpr double PICOSECONDS_SECONDS_RATIO = 1e-12;

constexpr double SECONDS_YEARS_RATIO = 1 / YEARS_SECONDS_RATIO;
constexpr double SECONDS_WEEKS_RATIO = 1 / WEEKS_SECONDS_RATIO;
constexpr double SECONDS_DAYS_RATIO = 1 / DAYS_SECONDS_RATIO;
constexpr double SECONDS_HOURS_RATIO = 1 / HOURS_SECONDS_RATIO;
constexpr double SECONDS_MINUTES_RATIO = 1 / MINUTES_SECONDS_RATIO;
constexpr double SECONDS_MILLISECONDS_RATIO = 1 / MILLISECONDS_SECONDS_RATIO;
constexpr double SECONDS_MICROSECONDS_RATIO = 1 / MICROSECONDS_SECONDS_RATIO;
constexpr double SECONDS_NANOSECONDS_RATIO = 1 / NANOSECONDS_SECONDS_RATIO;
constexpr double SECONDS_PICOSECONDS_RATIO = 1 / PICOSECONDS_SECONDS_RATIO;

constexpr Time operator ""_years(long double val)
{ return static_cast<Time>(val) * YEARS_SECONDS_RATIO; }
constexpr Time operator ""_weeks(long double val)
{ return static_cast<Time>(val) * WEEKS_SECONDS_RATIO; }
constexpr Time operator ""_days(long double val)
{ return static_cast<Time>(val) * DAYS_SECONDS_RATIO; }
constexpr Time operator ""_h(long double val)
{ return static_cast<Time>(val) * HOURS_SECONDS_RATIO; }
constexpr Time operator ""_min(long double val)
{ return static_cast<Time>(val) * MINUTES_SECONDS_RATIO; }
constexpr Time operator ""_s(long double val)
{ return static_cast<Time>(val); }
constexpr Time operator ""_ms(long double val)
{ return static_cast<Time>(val) * MILLISECONDS_SECONDS_RATIO; }
constexpr Time operator ""_us(long double val)
{ return static_cast<Time>(val) * MICROSECONDS_SECONDS_RATIO; }
constexpr Time operator ""_ns(long double val)
{ return static_cast<Time>(val) * NANOSECONDS_SECONDS_RATIO; }
constexpr Time operator ""_ps(long double val)
{ return static_cast<Time>(val) * PICOSECONDS_SECONDS_RATIO; }

struct TimeComponents
{
	unsigned int years{0};
	unsigned int months{0};
	unsigned int weeks{0};
	unsigned int days{0};

	unsigned int hours{0};
	unsigned int minutes{0};
	Time seconds{0};
};

Time SinceStart();
Time Local();
Time UTC(int offset = 0);

TimeComponents Split(Time t);
Time Merge(const TimeComponents& t);
std::string Format(Time t, const std::string& format);
std::string Format(const TimeComponents& t, const std::string& format);

void SetFrameTime(Time t);
void SetFPS(FPS fps);

Time GetDelta();
Time Await();

}

constexpr time::FPS operator "" _fps(unsigned long long int val)
{ return static_cast<unsigned int>(val); }

}
