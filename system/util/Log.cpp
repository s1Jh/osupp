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

#include "Log.hpp"

#include <thread>
#include <mutex>

NS_BEGIN

namespace log
{

static std::recursive_mutex LogMutex;
#ifdef RELEASE
static Severity MinimumSeverity = Severity::INF;
#else
static Severity MinimumSeverity = Severity::DEV;
#endif
static bool Enabled = true;

namespace detail
{

bool BeginLogMessage(Severity msgSeverity)
{
    if (msgSeverity < MinimumSeverity) {
        return false;
    }

    LogMutex.lock();
    std::cout << '[' << std::this_thread::get_id() << "] ";
    return true;
}

void Init()
{
    std::cin.tie(nullptr);
    std::cout.sync_with_stdio(false);
}

void EndLogMessage()
{
    std::cout << '\n';
    std::cout.flush();
    LogMutex.unlock();
}

} // namespace detail

void SetLogSeverity(Severity level)
{
    std::scoped_lock<std::recursive_mutex> lock(LogMutex);
    MinimumSeverity = level;
}

void Enable()
{
    std::scoped_lock<std::recursive_mutex> lock(LogMutex);
    Enabled = true;
}

void Disable()
{
    std::scoped_lock<std::recursive_mutex> lock(LogMutex);
    Enabled = false;
}

}

NS_END