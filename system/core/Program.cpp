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

#include "Program.hpp"

#include "Log.hpp"
#include "State.hpp"
#include "Error.hpp"
#include "Tasks.hpp"

#include <cstdlib>

namespace PROJECT_NAMESPACE::core
{

/// Serves as the real entry point to the program, gets called from main.
void EntryPoint()
{
    // First initialize all the APIs we rely on.
    error::detail::InstallHandler();
    tasks::Start();
    log::detail::Init();
    log::Custom(log::Severity::INF, "TOAST", "Hello world!");

    // SDL2, OpenGL and FreeType get initiated by the video module during static initialization.

    // Keep creating states and running them as long as they return RESTART, otherwise quit.
    StateHandler::StateReturn status = StateHandler::StateReturn::RESTART;

    while (status == StateHandler::StateReturn::RESTART) {
        StateHandler state;
        status = state.enter();
    }

    // We're done here...
    Exit(0);
}

void Exit(unsigned int code)
{
    log::Custom(log::Severity::INF, "TOAST", "Goodbye world!");
    // Here we should perform all library cleanup, as this is a single choke for all exit calls.

    std::exit(code);
}

}

#if defined(WINDOWS) && defined(RELEASE)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#else
int main()
#endif
{
    PROJECT_NAMESPACE::core::EntryPoint();
}
