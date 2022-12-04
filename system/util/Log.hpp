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

namespace log
{

enum class Severity
{
    DEVELOPMENT,
    DBG,
    INFO,
    WARNING,
    ERROR
};

namespace detail
{

bool BeginLogMessage(Severity sev);
void EndLogMessage();

template<typename T>
inline static void Print(T t)
{
    std::cout << t;
}

template<typename T, typename... Args>
inline static void Print(T t, Args... args)
{
    std::cout << t;
    Print(args...);
}

void Init();

}

template<typename First, typename... Args>
inline static void info(First f, Args... msgs)
{
    if (detail::BeginLogMessage(Severity::INFO)) {
        detail::Print("[INFO] ", f, msgs...);
        detail::EndLogMessage();
    }
}

template<typename First, typename... Args>
inline static void debug(First f, Args... msgs)
{
    if (detail::BeginLogMessage(Severity::DBG)) {
        detail::Print("[DEBUG] ", f, msgs...);
        detail::EndLogMessage();
    }
}

template<typename First, typename... Args>
inline static void warning(First f, Args... msgs)
{
    if (detail::BeginLogMessage(Severity::WARNING)) {
        detail::Print("[WARNING] ", f, msgs...);
        detail::EndLogMessage();
    }
}

template<typename First, typename... Args>
inline static void error(First f, Args... msgs)
{
    if (detail::BeginLogMessage(Severity::ERROR)) {
        detail::Print("[ERROR] ", f, msgs...);
        detail::EndLogMessage();
    }
}

template<typename First, typename... Args>
inline void custom(const std::string &type, First f, Args... msgs)
{
    if (detail::BeginLogMessage(Severity::INFO)) {
        detail::Print('[' + type + "] ", f, msgs...);
        detail::EndLogMessage();
    }
}

void SetLogSeverity(Severity level);

void Enable();

void Disable();

};

NS_END