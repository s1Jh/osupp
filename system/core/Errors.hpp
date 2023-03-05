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
    Errors.hpp

    Contains a macro table of error codes, their severity and description.
 */

#pragma once

#include "config.hpp"

#ifndef USER_ERROR_LIST
#define USER_ERROR_LIST
#endif

#define ERROR_LIST                                                                                                                                     \
    ERROR(NONE, Severity::NONE, "This is not an error", "")                                                                                            \
    ERROR(UNKNOWN, Severity::MAJOR, "Unknown error", "")                                                                                               \
    USER_ERROR_LIST                                                                                                                                    \
    ERROR(FILE_NOT_FOUND, Severity::MINOR, "File could not be located", "")                                                                            \
    ERROR(LOAD_FAILED, Severity::MINOR, "File could not be read", "")                                                                                  \
    ERROR(API_FAIL, Severity::MAJOR, "An error occurred in a third party API", "")                                                                      \
    ERROR(API_FAIL_FATAL, Severity::FATAL, "A fatal error has occurred in a third party API", "")                                                                      \
    ERROR(API_LOAD_FAIL, Severity::FATAL, "Could not load third party API", "")                                                                        \
    ERROR(SIG_TERM, Severity::FATAL, "Terminate called", "The program is being terminated.")                                                           \
    ERROR(SIG_SEGV, Severity::FATAL, "Segmentation fault", "The program tried to access memory it didn't own.")                                        \
    ERROR(SIG_INT, Severity::MINOR, "Interrupt", "The run of the program was interrupted.")                                                            \
    ERROR(SIG_ILL, Severity::FATAL, "Ill-formed instruction", "The program tried to execute a ill-formed instruction, the binary could be corrupted.") \
    ERROR(SIG_ABRT, Severity::FATAL, "Abort called", "The program is being aborted.")                                                                  \
    ERROR(SIG_FPE, Severity::MAJOR, "Floating point exception", "A floating point exception has occurred.")                                            \
    ERROR(SIG_BREAK, Severity::MAJOR, "Break called", "A break had been introduced into the program.")                                                 \
    ERROR(CPP_EXCEPTION, Severity::FATAL, "", "")
