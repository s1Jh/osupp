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

#include <iostream>
#include <mutex>
#include <vector>

NS_BEGIN

namespace detail
{

class SectionEntry
{
public:
    explicit SectionEntry(const std::string &section);

    ~SectionEntry();
};
} // namespace detail


#define LOG_ENTER(_void) detail::SectionEntry __log_section(__FUNCTION__)
#define LOG_ENTERH(_custom) detail::SectionEntry __log_section(_custom)

class log
{
    friend class detail::SectionEntry;

public:
    template<typename First, typename... Args>
    inline static void info(First f, Args... msgs)
    {
        custom("INFO", f, msgs...);
    }

    template<typename First, typename... Args>
    inline static void debug(First f, Args... msgs)
    {
#if(RELEASE != 1 || MINREL != 1)
        if (enableDebug) {
            custom("DEBUG", f, msgs...);
        }
#endif
    }

    template<typename First, typename... Args>
    inline static void warning(First f, Args... msgs)
    {
        custom("WARNING", f, msgs...);
    }

    template<typename First, typename... Args>
    inline static void error(First f, Args... msgs)
    {
        custom("ERROR", f, msgs...);
    }

    template<typename First, typename... Args>
    inline static void custom(const std::string &type, First f, Args... msgs)
    {
        if (enabled) {
            std::lock_guard<std::recursive_mutex> lock(logMutex);

            for (const auto &section: sections) {
                std::cout << '[' << section << "] ";
            }

            std::cout << '[' << type << "] ";
            print(f, msgs...);
        }
    }

    static void setDebug(bool ns);

    static void enable();

    static void disable();

private:
    static std::vector<std::string> sections;
    static std::recursive_mutex logMutex;
    static bool enableDebug;
    static bool enabled;

    template<typename T>
    inline static void print(T t)
    {
        std::cout << t << std::endl;
    }

    template<typename T, typename... Args>
    inline static void print(T t, Args... args)
    {
        std::cout << t;
        print(args...);
    }
};

NS_END