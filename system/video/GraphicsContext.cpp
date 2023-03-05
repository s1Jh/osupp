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
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "GL.hpp"
#include "Log.hpp"
#include "Error.hpp"
#include "StrUtil.hpp"

#define GLEXT(ret, name, ...) \
name##proc * gl##name;

#ifdef WIN32
GL_FUNC_LIST_WIN32
#endif

#ifdef LINUX

GL_FUNC_LIST_LINUX

#endif

GL_FUNC_LIST_SHARED

#undef GLEXT

namespace PROJECT_NAMESPACE::video {

class GraphicsApi
{
public:
	GraphicsApi() noexcept;
    ~GraphicsApi();
private:
    log::Logger logger{"GFX", log::Severity::INF};
};

[[maybe_unused]] static GraphicsApi Api;

GraphicsApi::GraphicsApi() noexcept
{
	logger("Initializing SDL2");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		error::Raise(error::Code::API_LOAD_FAIL, "Could not initialize SDL2");
	}

	SDL_version ver;
	SDL_GetVersion(&ver);
	logger("SDL2 version:", Concatenate(ver.major, ".", ver.minor, ".", ver.patch));

	logger("Loading OpenGL functions");
#if defined(LINUX)
	void *libGL = dlopen("libGL.so", RTLD_LAZY);
	if (!libGL) {
		error::Raise(error::Code::API_LOAD_FAIL, "Unable to open libGL.so");
	}

#define GLEXT(ret, name, ...)                                              \
	gl##name = (name##proc *) dlsym(libGL, "gl" #name);                    \
	if (!gl##name) {                                                       \
		error::Raise(error::Code::API_LOAD_FAIL, "Function gl" #name " couldn't be loaded"); \
	}

	GL_FUNC_LIST_SHARED
	GL_FUNC_LIST_LINUX

#undef GLEXT

#elif defined(WINDOWS)
	HMODULE dll = LoadLibraryA("opengl32.dll");
	typedef PROC WINAPI
		wglGetProcAddressproc(LPCSTR
							  lpszProc);
	if (!dll) {
		error::Raise(error::Code::API_LOAD_FAIL, "Unable to open opengl32.dll");
	}
	auto *wglGetProcAddress = (wglGetProcAddressproc *)GetProcAddress(dll, "wglGetProcAddress");

#define GLEXT(ret, name, ...) \
    gl##name = (name##proc *)wglGetProcAddress("gl" #name); \
    if (!gl##name) { \
        gl##name = (name##proc*)GetProcAddress(dll, "gl" #name); \
        if (!gl##name) {         \
            error::Raise(error::Code::API_LOAD_FAIL, "Function gl" #name " couldn't be loaded"); \
		}   \
    }

	GL_FUNC_LIST_SHARED
	GL_FUNC_LIST_WIN32

#undef GLEXT

#else
#error "OpenGL loading for this platform is not implemented yet."
#endif
}
GraphicsApi::~GraphicsApi()
{
    logger("Closing SDL2");
    SDL_Quit();
}

}
