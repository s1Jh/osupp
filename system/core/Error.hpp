//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#pragma once

#include "define.hpp"

#include "CallbackManager.hpp"

#include <string>

NS_BEGIN

namespace error {

enum class Severity {
    NONE, MINOR, MAJOR, FATAL
};

#ifndef USER_ERROR_LIST
#define USER_ERROR_LIST
#endif

#define ERROR_LIST \
    ERROR(NONE, Severity::NONE, "This is not an error", "") \
    ERROR(UNKNOWN, Severity::MAJOR, "Unknown error", "")    \
    ERROR(GL_NOT_LOADED, Severity::FATAL, "Unable to load OpenGL", "") \
    ERROR(GLFW_ERROR, Severity::FATAL, "A GLFW error occured", "")     \
    ERROR(WINDOW_NOT_OPEN, Severity::FATAL, "Unable to open a window", "") \
    USER_ERROR_LIST \
    ERROR(SIG_TERM, Severity::FATAL, "Terminate called", "The program is being terminated.") \
    ERROR(SIG_SEGV, Severity::FATAL, "Segmentation fault", "The program tried to access memory it didn't own.") \
    ERROR(SIG_INT, Severity::MINOR, "Interrupt", "The run of the program was interrupted.") \
    ERROR(SIG_ILL, Severity::FATAL, "Ill-formed instruction", "The program tried to execute a ill-formed instruction, the binary could be corrupted.") \
    ERROR(SIG_ABRT, Severity::FATAL, "Abort called", "The program is being aborted.") \
    ERROR(SIG_FPE, Severity::MAJOR, "Floating point exception", "A floating point exception has occurred.")      \
    ERROR(SIG_BREAK, Severity::MAJOR, "Break called", "A break had been introduced into the program.")      \
    ERROR( C_E2BIG, Severity::MINOR , "Argument list too long", "" ) \
    ERROR( C_EACCES, Severity::MINOR , "Permission denied", "" ) \
    ERROR( C_EADDRINUSE, Severity::MINOR , "Address in use", "" ) \
    ERROR( C_EADDRNOTAVAIL, Severity::MINOR , "Address not available", "" ) \
    ERROR( C_EAFNOSUPPORT, Severity::MINOR , "Address family not supported", "" ) \
    ERROR( C_EAGAIN, Severity::FATAL , "Resource unavailable, try again", "" ) \
    ERROR( C_EALREADY, Severity::MINOR , "Connection already in progress", "" ) \
    ERROR( C_EBADF, Severity::MINOR , "Bad file descriptor", "" ) \
    ERROR( C_EBADMSG, Severity::MINOR , "Bad message", "" ) \
    ERROR( C_EBUSY, Severity::MINOR , "Device or resource busy", "" ) \
    ERROR( C_ECANCELED, Severity::MINOR , "Operation canceled", "" ) \
    ERROR( C_ECHILD, Severity::MINOR , "No child processes", "" ) \
    ERROR( C_ECONNABORTED, Severity::MINOR , "Connection aborted", "" ) \
    ERROR( C_ECONNREFUSED, Severity::MINOR , "Connection refused", "" ) \
    ERROR( C_ECONNRESET, Severity::MINOR , "Connection reset", "" ) \
    ERROR( C_EDEADLK, Severity::MINOR , "Resource deadlock would occur", "" ) \
    ERROR( C_EDESTADDRREQ, Severity::MINOR , "Destination address required", "" ) \
    ERROR( C_EDOM, Severity::MINOR, "Mathematics argument out of domain of function", "" ) \
    ERROR( C_EEXIST, Severity::MINOR , "File exists", "" ) \
    ERROR( C_EFAULT, Severity::MINOR , "Bad address", "" ) \
    ERROR( C_EFBIG, Severity::MINOR , "File too large", "" ) \
    ERROR( C_EHOSTUNREACH, Severity::MINOR , "Host is unreachable", "" ) \
    ERROR( C_EIDRM, Severity::MINOR , "Identifier removed", "" ) \
    ERROR( C_EILSEQ, Severity::MINOR , "Illegal byte sequence", "" ) \
    ERROR( C_EINPROGRESS, Severity::MINOR , "Operation in progress", "" ) \
    ERROR( C_EINTR, Severity::MINOR , "Interrupted function", "" ) \
    ERROR( C_EINVAL, Severity::MINOR , "Invalid argument", "" ) \
    ERROR( C_EIO, Severity::MINOR , "I/O error", "" ) \
    ERROR( C_EISCONN, Severity::MINOR , "Socket is connected", "" ) \
    ERROR( C_EISDIR, Severity::MINOR , "Is a directory", "" ) \
    ERROR( C_ELOOP, Severity::MINOR , "Too many levels of symbolic links", "" ) \
    ERROR( C_EMFILE, Severity::MINOR , "File descriptor value too large", "" ) \
    ERROR( C_EMLINK, Severity::MINOR , "Too many links", "" ) \
    ERROR( C_EMSGSIZE, Severity::MINOR , "Message too large", "" ) \
    ERROR( C_ENAMETOOLONG, Severity::MINOR , "Filename too long", "" ) \
    ERROR( C_ENETDOWN, Severity::MINOR , "Network is down", "" ) \
    ERROR( C_ENETRESET, Severity::MINOR , "Connection aborted by network", "" ) \
    ERROR( C_ENETUNREACH, Severity::MINOR , "Network unreachable", "" ) \
    ERROR( C_ENFILE, Severity::MINOR , "Too many files open in system", "" ) \
    ERROR( C_ENOBUFS, Severity::MINOR , "No buffer space available", "" ) \
    ERROR( C_ENODATA, Severity::MINOR , "No message is available on the STREAM head read queue", "" ) \
    ERROR( C_ENODEV, Severity::MINOR , "No such device", "" ) \
    ERROR( C_ENOENT, Severity::MINOR , "No such file or directory", "" ) \
    ERROR( C_ENOEXEC, Severity::MINOR , "Executable file format error", "" ) \
    ERROR( C_ENOLCK, Severity::MINOR , "No locks available", "" ) \
    ERROR( C_ENOLINK, Severity::MINOR , "Link has been severed", "" ) \
    ERROR( C_ENOMEM, Severity::MINOR , "Not enough space", "" ) \
    ERROR( C_ENOMSG, Severity::MINOR , "No message of the desired type", "" ) \
    ERROR( C_ENOPROTOOPT, Severity::MINOR , "Protocol not available", "" ) \
    ERROR( C_ENOSPC, Severity::MINOR , "No space left on device", "" ) \
    ERROR( C_ENOSR, Severity::MINOR , "No STREAM resources", "" ) \
    ERROR( C_ENOSTR, Severity::MINOR , "Not a STREAM", "" ) \
    ERROR( C_ENOSYS, Severity::MINOR , "Function not supported", "" ) \
    ERROR( C_ENOTCONN, Severity::MINOR , "The socket is not connected", "" ) \
    ERROR( C_ENOTDIR, Severity::MINOR , "Not a directory", "" ) \
    ERROR( C_ENOTEMPTY, Severity::MINOR , "Directory not empty", "" ) \
    ERROR( C_ENOTRECOVERABLE, Severity::MINOR , "State not recoverable", "" ) \
    ERROR( C_ENOTSOCK, Severity::MINOR , "Not a socket", "" ) \
    ERROR( C_ENOTSUP, Severity::MINOR , "Not supported", "" ) \
    ERROR( C_ENOTTY, Severity::MINOR , "Inappropriate I/O control operation", "" ) \
    ERROR( C_ENXIO, Severity::MINOR , "No such device or address", "" ) \
    ERROR( C_EOPNOTSUPP, Severity::MINOR , "Operation not supported on socket", "" ) \
    ERROR( C_EOVERFLOW, Severity::MINOR , "Value too large to be stored in data type", "" ) \
    ERROR( C_EOWNERDEAD, Severity::MINOR , "Previous owner died", "" ) \
    ERROR( C_EPERM, Severity::MINOR , "Operation not permitted", "" ) \
    ERROR( C_EPIPE, Severity::MINOR , "Broken pipe", "" ) \
    ERROR( C_EPROTO, Severity::MINOR , "Protocol error", "" ) \
    ERROR( C_EPROTONOSUPPORT, Severity::MINOR , "Protocol not supported", "" ) \
    ERROR( C_EPROTOTYPE, Severity::MINOR , "Protocol wrong type for socket", "" ) \
    ERROR( C_ERANGE, Severity::MINOR, "Result too large", "" ) \
    ERROR( C_EROFS, Severity::MINOR , "Read-only file system", "" ) \
    ERROR( C_ESPIPE, Severity::MINOR , "Invalid seek", "" ) \
    ERROR( C_ESRCH, Severity::MINOR , "No such process", "" ) \
    ERROR( C_ETIME, Severity::MINOR , "Stream ioctl( ) timeout", "" ) \
    ERROR( C_ETIMEDOUT, Severity::MINOR , "Connection timed out", "" ) \
    ERROR( C_ETXTBSY, Severity::MINOR , "Text file busy", "" ) \
    ERROR( C_EWOULDBLOCK, Severity::MINOR , "Operation would block", "" ) \
    ERROR( C_EXDEV, Severity::MINOR , "Cross-device link", "" ) \
    ERROR( CPP_EXCEPTION, Severity::FATAL , "", "" )

enum class Code {
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(Name, ...) Name,
    ERROR_LIST
#undef ERROR
};

namespace detail {

    bool InstallHandler();
    void Receive(Code code, const std::string& name, const std::string& description, Severity severity);

}

void Raise(Code code, const std::string& additionalNotes = "");
bool IsActive();
Code GetLast();

Severity GetSeverity(Code code);
const std::string& GetName(Code code);
const std::string& GetDescription(Code code);
const std::string& GetString(Code code);

enum class Callback {
    ERROR_MINOR,
    ERROR_MAJOR,
    ERROR_FATAL,
    ERROR_ANY
};

class ErrorCallbackManager : public ::PROJECT_NAMESPACE::detail::StaticCallbacks<Callback> {
    friend void detail::Receive(Code code, const std::string& name,
                                     const std::string& description, Severity severity);
};

}

MAKE_CALLBACK(error::Callback, error::Callback::ERROR_ANY, error::Code);
MAKE_CALLBACK(error::Callback, error::Callback::ERROR_MAJOR, error::Code);
MAKE_CALLBACK(error::Callback, error::Callback::ERROR_MINOR, error::Code);
MAKE_CALLBACK(error::Callback, error::Callback::ERROR_FATAL, error::Code);

NS_END
