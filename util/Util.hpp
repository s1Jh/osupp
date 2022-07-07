#pragma once

#include "define.hpp"

#include <iostream>
#include <mutex>
#include <vector>

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

#define CheckGLFW detail::CheckGLFWErrors(__FILE__, __LINE__)
#define CheckGL detail::CheckErrors(__FILE__, __LINE__)

#define CheckGLFWh(_helper) detail::CheckGLFWErrors(__FILE__, __LINE__, _helper)
#define CheckGLh(_helper) detail::CheckErrors(__FILE__, __LINE__, _helper)

#define LOG_ENTER(_void) detail::SectionEntry __log_section(__PRETTY_FUNCTION__)

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
#ifndef NDEBUG
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
