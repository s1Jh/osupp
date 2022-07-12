#pragma once

#include "define.hpp"

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>

NS_BEGIN

namespace detail
{
int CheckGLFWErrors(const std::string &file, int line,
                    const std::string &helper = "");

unsigned int CheckErrors(const std::string &file, int line,
                         const std::string &helper = "");

class SectionEntry
{
public:
    explicit SectionEntry(const std::string &section);

    ~SectionEntry();
};
} // namespace detail

unsigned int DumpGlErrors();

std::vector<std::string> GetCharacterSeparatedValues(const std::string &in, char sep);

#define CheckGLFW detail::CheckGLFWErrors(__FILE__, __LINE__)
#define CheckGL detail::CheckErrors(__FILE__, __LINE__)

#define CheckGLFWh(_helper) detail::CheckGLFWErrors(__FILE__, __LINE__, _helper)
#define CheckGLh(_helper) detail::CheckErrors(__FILE__, __LINE__, _helper)

#define LOG_ENTER(_void) detail::SectionEntry __log_section(__FUNCTION__)
#define LOG_ENTERH(_custom) detail::SectionEntry __log_section(_custom)

/**
 * String trimming functions from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring/217605#217605
 */

static inline void LTrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
}

static inline void RTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void Trim(std::string &s)
{
    LTrim(s);
    RTrim(s);
}

// trim from start (copying)
static inline std::string LTrimCopy(std::string s)
{
    LTrim(s);
    return s;
}

// trim from end (copying)
static inline std::string RTrimCopy(std::string s)
{
    RTrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string TrimCopy(std::string s)
{
    Trim(s);
    return s;
}

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
            std::lock_guard<std::recursive_mutex> lock(coutMutex);

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
    static std::recursive_mutex coutMutex;
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
