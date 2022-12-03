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
#include "Error.hpp"
#include "Program.hpp"

#include <csignal>
#include <exception>
#include <sstream>

#ifdef WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

NS_BEGIN

namespace error
{

struct ErrInfo
{
	Code code;
	std::string strCode;
	std::string title;
	std::string description;
	Severity severity;
};

static const ErrInfo ErrorInfoTable[] = {
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(ErrCode, Severity, Title, Desc, ...) {Code::ErrCode, #ErrCode, Title, Desc, Severity},
	ERROR_LIST
#undef ERROR
};

thread_local Code CurrentError;

void detail::Receive(Code code, const std::string &name, const std::string &description, Severity severity)
{
	if (severity == Severity::NONE) {
		return;
	}

	std::string errSeverityStr;
	switch (severity) {
	case Severity::MINOR:
		errSeverityStr = "minor";
		break;
	case Severity::MAJOR:
		errSeverityStr = "major";
		break;
	case Severity::FATAL:
		errSeverityStr = "fatal";
		break;
	default:
		break;
	}

	std::string errString = GetString(code);
	std::stringstream content;
	content << "A " << errSeverityStr << " error has occurred: " << errString << "(0x" << std::hex << (int)code << ")\n"
		<< std::dec << name << '\n' << description;

	if (severity == Severity::FATAL) {
		content << "\nThe program will now terminate.";
	}

	log::error(content.str());

	std::stringstream caption;
	caption << TOSTRING(GAME_TITLE) << " error";

	CurrentError = code;

	ErrorCallbackManager::InvokeCallback<Callback::ERROR_ANY>(code);

	switch (severity) {
	case Severity::MINOR:
		ErrorCallbackManager::InvokeCallback<Callback::ERROR_MINOR>(code);
		break;
	case Severity::MAJOR:
		ErrorCallbackManager::InvokeCallback<Callback::ERROR_MAJOR>(code);
#ifdef WINDOWS
		MessageBox(nullptr, content.str().c_str(), caption.str().c_str(),
				   MB_ICONERROR | MB_OK);
#endif
		break;
	case Severity::FATAL:
		ErrorCallbackManager::InvokeCallback<Callback::ERROR_FATAL>(code);
#ifdef WINDOWS
		MessageBox(nullptr, content.str().c_str(), caption.str().c_str(),
				   MB_ICONERROR | MB_OK);
#endif
		::PROJECT_NAMESPACE::program::Exit((int)code);
		break;
	default:
		break;
	}
}

void TerminateHandler()
{

	switch (errno) {
	case E2BIG:
		Raise(Code::C_E2BIG);
		break;
	case EACCES:
		Raise(Code::C_EACCES);
		break;
	case EADDRINUSE:
		Raise(Code::C_EADDRINUSE);
		break;
	case EADDRNOTAVAIL:
		Raise(Code::C_EADDRNOTAVAIL);
		break;
	case EAFNOSUPPORT:
		Raise(Code::C_EAFNOSUPPORT);
		break;
	case EAGAIN:
		Raise(Code::C_EAGAIN);
		break;
	case EALREADY:
		Raise(Code::C_EALREADY);
		break;
	case EBADF:
		Raise(Code::C_EBADF);
		break;
	case EBADMSG:
		Raise(Code::C_EBADMSG);
		break;
	case EBUSY:
		Raise(Code::C_EBUSY);
		break;
	case ECANCELED:
		Raise(Code::C_ECANCELED);
		break;
	case ECHILD:
		Raise(Code::C_ECHILD);
		break;
	case ECONNABORTED:
		Raise(Code::C_ECONNABORTED);
		break;
	case ECONNREFUSED:
		Raise(Code::C_ECONNREFUSED);
		break;
	case ECONNRESET:
		Raise(Code::C_ECONNRESET);
		break;
	case EDEADLK:
		Raise(Code::C_EDEADLK);
		break;
	case EDESTADDRREQ:
		Raise(Code::C_EDESTADDRREQ);
		break;
	case EDOM:
		Raise(Code::C_EDOM);
		break;
	case EEXIST:
		Raise(Code::C_EEXIST);
		break;
	case EFAULT:
		Raise(Code::C_EFAULT);
		break;
	case EFBIG:
		Raise(Code::C_EFBIG);
		break;
	case EHOSTUNREACH:
		Raise(Code::C_EHOSTUNREACH);
		break;
	case EIDRM:
		Raise(Code::C_EIDRM);
		break;
	case EILSEQ:
		Raise(Code::C_EILSEQ);
		break;
	case EINPROGRESS:
		Raise(Code::C_EINPROGRESS);
		break;
	case EINTR:
		Raise(Code::C_EINTR);
		break;
	case EINVAL:
		Raise(Code::C_EINVAL);
		break;
	case EIO:
		Raise(Code::C_EIO);
		break;
	case EISCONN:
		Raise(Code::C_EISCONN);
		break;
	case EISDIR:
		Raise(Code::C_EISDIR);
		break;
	case ELOOP:
		Raise(Code::C_ELOOP);
		break;
	case EMFILE:
		Raise(Code::C_EMFILE);
		break;
	case EMLINK:
		Raise(Code::C_EMLINK);
		break;
	case EMSGSIZE:
		Raise(Code::C_EMSGSIZE);
		break;
	case ENAMETOOLONG:
		Raise(Code::C_ENAMETOOLONG);
		break;
	case ENETDOWN:
		Raise(Code::C_ENETDOWN);
		break;
	case ENETRESET:
		Raise(Code::C_ENETRESET);
		break;
	case ENETUNREACH:
		Raise(Code::C_ENETUNREACH);
		break;
	case ENFILE:
		Raise(Code::C_ENFILE);
		break;
	case ENOBUFS:
		Raise(Code::C_ENOBUFS);
		break;
	case ENODATA:
		Raise(Code::C_ENODATA);
		break;
	case ENODEV:
		Raise(Code::C_ENODEV);
		break;
	case ENOENT:
		Raise(Code::C_ENOENT);
		break;
	case ENOEXEC:
		Raise(Code::C_ENOEXEC);
		break;
	case ENOLCK:
		Raise(Code::C_ENOLCK);
		break;
	case ENOLINK:
		Raise(Code::C_ENOLINK);
		break;
	case ENOMEM:
		Raise(Code::C_ENOMEM);
		break;
	case ENOMSG:
		Raise(Code::C_ENOMSG);
		break;
	case ENOPROTOOPT:
		Raise(Code::C_ENOPROTOOPT);
		break;
	case ENOSPC:
		Raise(Code::C_ENOSPC);
		break;
	case ENOSR:
		Raise(Code::C_ENOSR);
		break;
	case ENOSTR:
		Raise(Code::C_ENOSTR);
		break;
	case ENOSYS:
		Raise(Code::C_ENOSYS);
		break;
	case ENOTCONN:
		Raise(Code::C_ENOTCONN);
		break;
	case ENOTDIR:
		Raise(Code::C_ENOTDIR);
		break;
	case ENOTEMPTY:
		Raise(Code::C_ENOTEMPTY);
		break;
	case ENOTRECOVERABLE:
		Raise(Code::C_ENOTRECOVERABLE);
		break;
	case ENOTSOCK:
		Raise(Code::C_ENOTSOCK);
		break;
#ifndef LINUX // Causes a duplicate switch branch on linux
	case ENOTSUP:
		Raise(Code::C_ENOTSUP);
		break;
#endif
	case ENOTTY:
		Raise(Code::C_ENOTTY);
		break;
	case ENXIO:
		Raise(Code::C_ENXIO);
		break;
	case EOPNOTSUPP:
		Raise(Code::C_EOPNOTSUPP);
		break;
	case EOVERFLOW:
		Raise(Code::C_EOVERFLOW);
		break;
	case EOWNERDEAD:
		Raise(Code::C_EOWNERDEAD);
		break;
	case EPERM:
		Raise(Code::C_EPERM);
		break;
	case EPIPE:
		Raise(Code::C_EPIPE);
		break;
	case EPROTO:
		Raise(Code::C_EPROTO);
		break;
	case EPROTONOSUPPORT:
		Raise(Code::C_EPROTONOSUPPORT);
		break;
	case EPROTOTYPE:
		Raise(Code::C_EPROTOTYPE);
		break;
	case ERANGE:
		Raise(Code::C_ERANGE);
		break;
	case EROFS:
		Raise(Code::C_EROFS);
		break;
	case ESPIPE:
		Raise(Code::C_ESPIPE);
		break;
	case ESRCH:
		Raise(Code::C_ESRCH);
		break;
	case ETIME:
		Raise(Code::C_ETIME);
		break;
	case ETIMEDOUT:
		Raise(Code::C_ETIMEDOUT);
		break;
	case ETXTBSY:
		Raise(Code::C_ETXTBSY);
		break;
#ifndef LINUX // Same as EAGAIN
	case EWOULDBLOCK:
		Raise(Code::C_EWOULDBLOCK);
		break;
#endif
	case EXDEV:
		Raise(Code::C_EXDEV);
		break;
	default:
		break;
	}

	auto exception = std::current_exception();

	if (exception) {
		try {
			std::rethrow_exception(exception);
		}
		catch (const std::exception &e) {
			detail::Receive(Code::CPP_EXCEPTION, typeid(e).name(),
							e.what(), Severity::FATAL);
		}
		catch (...) {
			detail::Receive(Code::CPP_EXCEPTION, "Unknown C++ exception",
							"", Severity::FATAL);
		}
	} else {
		detail::Receive(Code::CPP_EXCEPTION, "Terminate called without an active exception",
						"", Severity::FATAL);
	}
}

void SignalHandler(int signal)
{
	switch (signal) {
	case SIGINT:
		Raise(Code::SIG_INT);
		return;
	case SIGILL:
		Raise(Code::SIG_ILL);
		return;
	case SIGFPE:
		Raise(Code::SIG_FPE);
		return;
	case SIGSEGV:
		Raise(Code::SIG_SEGV);
		return;
	case SIGTERM:
		Raise(Code::SIG_TERM);
		return;
#ifdef WINDOWS
	case SIGBREAK:
		Raise(Code::SIG_BREAK);
		return;
#endif
	case SIGABRT:
		Raise(Code::SIG_ABRT);
		return;
	default:
		Raise(Code::UNKNOWN);
		return;
	}
}

bool detail::InstallHandler()
{
	std::signal(SIGINT, SignalHandler);
	std::signal(SIGILL, SignalHandler);
	std::signal(SIGFPE, SignalHandler);
	std::signal(SIGSEGV, SignalHandler);
	std::signal(SIGTERM, SignalHandler);
#ifdef WINDOWS
	std::signal(SIGBREAK, SignalHandler);
#endif
	std::signal(SIGABRT, SignalHandler);

	std::set_terminate(TerminateHandler);

	return true;
}

void Raise(Code code, const std::string &additionalNotes)
{
	auto severity = GetSeverity(code);
	const auto &name = GetName(code);
	auto desc = GetDescription(code);

	if (!additionalNotes.empty()) {
		if (!desc.empty()) {
			desc += '\n';
		}
		desc += additionalNotes;
	}

	detail::Receive(code, name, desc, severity);
}

bool IsActive()
{
	return CurrentError != Code::NONE;
}

Code GetLast()
{
	return CurrentError;
}

Severity GetSeverity(Code code)
{
	return ErrorInfoTable[(int)code].severity;
}

const std::string &GetName(Code code)
{
	return ErrorInfoTable[(int)code].title;
}

const std::string &GetDescription(Code code)
{
	return ErrorInfoTable[(int)code].description;
}

const std::string &GetString(Code code)
{
	return ErrorInfoTable[(int)code].strCode;
}

}
NS_END

