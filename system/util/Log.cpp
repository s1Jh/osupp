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

NS_BEGIN

namespace detail
{

SectionEntry::~SectionEntry()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);

    ::PROJECT_NAMESPACE::log::sections.erase(
        ::PROJECT_NAMESPACE::log::sections.end() - 1);
}

SectionEntry::SectionEntry(const std::string &section)
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);

    ::PROJECT_NAMESPACE::log::sections.push_back(section);
}

} // namespace detail

std::recursive_mutex log::logMutex;

#ifdef RELEASE
bool log::enableDebug = false;
#else

bool log::enableDebug = true;

#endif

bool log::enabled = true;

std::vector<std::string> log::sections;

[[maybe_unused]] void log::setDebug(bool ns)
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enableDebug = ns;
}

void log::enable()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enabled = true;
}

void log::disable()
{
    std::scoped_lock<std::recursive_mutex> lock(::PROJECT_NAMESPACE::log::logMutex);
    enabled = false;
}

NS_END