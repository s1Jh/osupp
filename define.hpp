/*
 Copyright (c) 2023 sijh

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
    define.hpp

    This file contains all macros and compile time constants such as the game's title, build info etc.
 */

#pragma once
/*====================================================================================================================*/
/*  Build information & macros */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "config.hpp"

#ifndef GAME_TITLE
#   error("No game title defined")
#endif

#ifndef PROJECT_NAMESPACE
#   define PROJECT_NAMESPACE GAME_TITLE
#endif

/**
 *  199711L -> (until C++11)
 *  201103L -> (C++11)
 *  201402L -> (C++14)
 *  201703L -> (C++17)
 *  202002L -> (C++20)
 */

#if __cplusplus < 202002L
#   error("C++ version too old")
#endif

#ifndef INITIAL_STATE
#define INITIAL_STATE Exit
#endif

// Redefine the __FUNCSIG__ macro to __PRETTY_FUNCTION__ if applicable.
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// Simple macro to determine the size of a C style array.
#define ARRAY_SIZE(ARR) sizeof(ARR) / sizeof(ARR[0])

// Convert macro to a string.
#define _STRINGIFY(x) #x
#define TOSTRING(x) _STRINGIFY(x)

// Creates a compile time assertion that only occurs upon macro expansion.
#define WRAP_CONSTEXPR_ASSERTION(_Msg) \
    []<bool flag = false>()            \
    {                                  \
        static_assert(flag, _Msg);     \
    }                                  \
    ()

namespace PROJECT_NAMESPACE // keep default constexprs in the project namespace
{

#ifdef RELEASE
    constexpr const char *BUILD_TYPE = "Release";
#elif DEBUG

    constexpr const char *BUILD_TYPE = "Debug";

#elif RELDEB
    constexpr const char *BUILD_TYPE = "ReleaseWithDebInfo";
#elif MINREL
    constexpr const char *BUILD_TYPE = "MinSizeRel";
#elif
#error("No build type defined")
#endif

#if defined(__x86_64__) || defined(_M_X64)
constexpr const char* ARCH = "x86_64";
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
constexpr const char* ARCH = "x86_32";
#elif defined(__ARM_ARCH_2__)
constexpr const char* ARCH = "ARM2";
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
constexpr const char* ARCH = "ARM3";
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
constexpr const char* ARCH = "ARM4T";
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
constexpr const char* ARCH = "ARM5"
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
constexpr const char* ARCH = "ARM6T2";
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
constexpr const char* ARCH = "ARM6";
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
constexpr const char* ARCH = "ARM7";
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
constexpr const char* ARCH = "ARM7A";
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
constexpr const char* ARCH = "ARM7R";
#elif defined(__ARM_ARCH_7M__)
constexpr const char* ARCH = "ARM7M";
#elif defined(__ARM_ARCH_7S__)
constexpr const char* ARCH = "ARM7S";
#elif defined(__aarch64__) || defined(_M_ARM64)
constexpr const char* ARCH = "ARM64";
#elif defined(mips) || defined(__mips__) || defined(__mips)
constexpr const char* ARCH = "MIPS";
#elif defined(__sh__)
constexpr const char* ARCH = "SUPERH";
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
constexpr const char* ARCH = "POWERPC";
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
constexpr const char* ARCH = "POWERPC64";
#elif defined(__sparc__) || defined(__sparc)
constexpr const char* ARCH = "SPARC";
#elif defined(__m68k__)
constexpr const char* ARCH = "M68K";
#else
constexpr const char* ARCH = "UNKNOWN";
#endif

#ifdef WINDOWS
    constexpr const char *PLATFORM = "Windows";
#elif LINUX

    constexpr const char *PLATFORM = "Linux";

#else
#error("No platform defined")
#endif

#ifndef CONFIG_FILE
    constexpr const char *CONFIG_PATH = "settings.sdf";
#else

    constexpr const char *CONFIG_PATH = TOSTRING(CONFIG_FILE);

#endif

#ifndef LOG_FILE
    constexpr const char *LOG_PATH = "log.txt";
#else
    constexpr const char *LOG_PATH = TOSTRING(LOG_FILE);
#endif

    constexpr const char *BUILD_DATE = __DATE__;

    constexpr const char *BUILD_TIME = __TIME__;

    constexpr int VERSION_MAJOR = 0;

    constexpr int VERSION_MINOR = 10;

    constexpr int VERSION_PATCH = 0;

    constexpr const char *GAME_NAME = TOSTRING(GAME_TITLE);

}
/*====================================================================================================================*/
