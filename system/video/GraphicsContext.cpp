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
#include "GraphicsContext.hpp"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <cstring>

#include "GL.hpp"
#include "Log.hpp"
#include "Error.hpp"

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

NS_BEGIN

namespace video
{

namespace detail
{

void OnGLFWError(int code, const char* description) {
    log::custom("GLFW", description, " (", code, ")");
}

bool InitPlatformVideo()
{
    log::info("Initializing GLFW");
    glfwSetErrorCallback(OnGLFWError);

    if (glfwInit() != GLFW_TRUE) {
        error::Raise(error::Code::GLFW_ERROR, "Could not initialize GLFW");
        return false;
    }

    log::info("GLFW version: ", glfwGetVersionString());

    log::info("Loading OpenGL functions");
#if defined(LINUX)
    void *libGL = dlopen("libGL.so", RTLD_LAZY);
    if (!libGL) {
        error::Raise(error::Code::GL_NOT_LOADED, "Unable to open libGL.so");
    }

#define GLEXT(ret, name, ...)                                              \
	gl##name = (name##proc *) dlsym(libGL, "gl" #name);                    \
	if (!gl##name) {                                                       \
		error::Raise(error::Code::GL_NOT_LOADED, "Function gl" #name " couldn't be loaded"); \
    	return false;                                                                       \
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
		error::Raise(error::Code::GL_NOT_LOADED, "Unable to open opengl32.dll");
	}
	auto *wglGetProcAddress = (wglGetProcAddressproc *)GetProcAddress(dll, "wglGetProcAddress");

#define GLEXT(ret, name, ...) \
    gl##name = (name##proc *)wglGetProcAddress("gl" #name); \
    if (!gl##name) { \
        gl##name = (name##proc*)GetProcAddress(dll, "gl" #name); \
        if (!gl##name) {         \
            error::Raise(error::Code::GL_NOT_LOADED, "Function gl" #name " couldn't be loaded"); \
        	return false;        \
		}   \
    }

	GL_FUNC_LIST_SHARED
	GL_FUNC_LIST_WIN32

#undef GLEXT

#else
#error "OpenGL loading for this platform is not implemented yet."
#endif

//    log::info("OpenGL version: ", glGetString(GL_VERSION));
//    log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
//    log::info("Renderer: ", glGetString(GL_RENDERER));
//    log::info("Vendor: ", glGetString(GL_VENDOR));

    return true;
}

}

bool IsExtensionSupported(const char *extList, const char *extension)
{
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0') {
		return false;
	}

	/* It takes a bit of care to be fool-proof about parsing the
		OpenGL extensions string. Don't be fooled by sub-strings,
		etc. */
	for (start = extList;;) {
		where = strstr(start, extension);

		if (!where) {
			break;
		}

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ') {
			if (*terminator == ' ' || *terminator == '\0') {
				return true;
			}
		}

		start = terminator;
	}

	return false;
}

}

NS_END
