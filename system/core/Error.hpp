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
    Error.hpp

    Provides error handling capabilities.
 */

#pragma once

#include "define.hpp"

#include "Errors.hpp"

#include <string>

namespace PROJECT_NAMESPACE::error
{
    /// @brief Error severity determines how the error will be handled.
    enum class Severity
    {
        NONE,
        MINOR,
        MAJOR,
        FATAL
    };

    enum class Code : unsigned int
    {
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(Name, ...) Name,
        ERROR_LIST
#undef ERROR
    };

    namespace detail
    {

        /// @brief Installs signal and exception handlers.
        /// @return True if installed successfully.
        bool InstallHandler();

        /// @brief A common end point for handling errors.
        /// @param code The error code to raise.
        /// @param name Given name for an error.
        /// @param description Given description.
        /// @param severity Given severity.
        void Receive(Code code, const std::string &name, const std::string &description, Severity severity);

    }

    /// @brief Raises an error.
    /// @param code The error code.
    /// @param additionalNotes Additional information to display.
    void Raise(Code code, const std::string &additionalNotes = "");

    /// @brief Returns the error code of the last error raised.
    /// @return The error code.
    Code GetLast();

    /// @brief Obtains the severity of a given error code.
    /// @param code The error code.
    /// @return The severity.
    Severity GetSeverity(Code code);

    /// @brief Obtains the name of a given error code.
    /// @param code The error code.
    /// @return The name.
    const std::string &GetName(Code code);

    /// @brief Obtains the description of a given error code.
    /// @param code The error code.
    /// @return The description.
    const std::string &GetDescription(Code code);

    /// @brief Obtains the
    /// @param code
    /// @return
    const std::string &GetString(Code code);
}
