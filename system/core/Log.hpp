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

#include "ToFromString.hpp"
#include "Traits.hpp"

#include <filesystem>

namespace PROJECT_NAMESPACE::log
{

/// @brief Used for log levels.
enum class Severity
{
    DEV,    // Developer logs
    DBG,    // Debug logs
    INF,    // Information logs
    WARN,   // Warning logs
    ERR     // Error logs
};

namespace detail
{

/// @brief Initializes a log message.
/// @param sev The severity of the log message.
/// @return True if severity is high enough for the current log level.
bool BeginLogMessage(Severity sev);

/// @brief Ends a log message, must be called after BeginLogMessage returns true.
void EndLogMessage();

/// @brief Finally writes to stdout.
/// @param str String to write.
void Put(const std::string& str);

/// @brief Final expansion to variadic Print function.
/// @tparam T Any T with and implemented ToString function.
/// @param t The message.
template<typename T>
inline static void Print(T t)
{
    if constexpr (is_string<decltype(t)>::value) {
        Put(t);
    } else {
        Put(ToString<T>(t));
    }
}

/// @brief Variadic print function.
/// @tparam T Any T with and implemented ToString function.
/// @param t The message.
template<typename T, typename... Args>
inline static void Print(T t, Args... args)
{
    if constexpr (is_string<decltype(t)>::value) {
        Put(t);
    } else {
        Put(ToString<T>(t));
    }
    Put(" ");
    Print(args...);
}

/// @brief Configures standard output for logging.
/// @param logPath The path of the log file.
void Init(const std::filesystem::path& logPath = LOG_PATH);

}

class Logger {
public:
    Logger(std::string&& categoryIn, Severity severityIn);

    template<typename First, typename... Args>
    void operator()(First f, Args... msgs) const {
        if (detail::BeginLogMessage(severity)) {
            detail::Print(category, f, msgs...);
            detail::EndLogMessage();
        }
    }

    template<typename First, typename... Args>
    void operator()(Severity customSev, First f, Args... msgs) const {
        if (detail::BeginLogMessage(customSev)) {
            detail::Print(category, f, msgs...);
            detail::EndLogMessage();
        }
    }

private:
    std::string category;
    Severity severity;
};

extern Logger Info;
extern Logger Debug;
extern Logger Warning;
extern Logger Error;

//template<typename First, typename... Args>
//inline static void Info(First f, Args... msgs)
//{
//    if (detail::BeginLogMessage(Severity::INF)) {
//        detail::Print("[INFO]", f, msgs...);
//        detail::EndLogMessage();
//    }
//}
//
//template<typename First, typename... Args>
//inline static void Debug(First f, Args... msgs)
//{
//    if (detail::BeginLogMessage(Severity::DBG)) {
//        detail::Print("[DEBUG]", f, msgs...);
//        detail::EndLogMessage();
//    }
//}
//
//template<typename First, typename... Args>
//inline static void Warning(First f, Args... msgs)
//{
//    if (detail::BeginLogMessage(Severity::WARN)) {
//        detail::Print("[WARNING]", f, msgs...);
//        detail::EndLogMessage();
//    }
//}
//
//template<typename First, typename... Args>
//inline static void Error(First f, Args... msgs)
//{
//    if (detail::BeginLogMessage(Severity::ERR)) {
//        detail::Print("[ERROR]", f, msgs...);
//        detail::EndLogMessage();
//    }
//}

template<typename First, typename... Args>
inline void Custom(Severity sev, const std::string &type, First f, Args... msgs)
{
    if (detail::BeginLogMessage(sev)) {
        detail::Print('[' + type + "]", f, msgs...);
        detail::EndLogMessage();
    }
}

void SetLogSeverity(Severity level);
void SetLogFile(const std::filesystem::path& path);

void Enable();

void Disable();

}
