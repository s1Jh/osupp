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
#include "Log.hpp"

#include "Math.hpp"

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

namespace PROJECT_NAMESPACE::error
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

	log::Error(content.str());

	std::stringstream caption;
	caption << TOSTRING(GAME_TITLE) << " error";

	CurrentError = code;

	switch (severity) {
	case Severity::MINOR:
		break;
	case Severity::MAJOR:
#ifdef WINDOWS
		MessageBox(nullptr, content.str().c_str(), caption.str().c_str(),
				   MB_ICONERROR | MB_OK);
#endif
		break;
	case Severity::FATAL:
#ifdef WINDOWS
		MessageBox(nullptr, content.str().c_str(), caption.str().c_str(),
				   MB_ICONERROR | MB_OK);
#endif
		core::Exit(static_cast<unsigned int>(code));
		break;
	default:
		break;
	}
}

void TerminateHandler()
{
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
