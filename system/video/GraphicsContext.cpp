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

#include "Math.hpp"
#include "GL.hpp"
#include "Log.hpp"
#include "Util.hpp"
#include "Error.hpp"
#include "NamedTask.hpp"
#include "Tasks.hpp"


#include "backends/imgui_impl_glfw.h"

NS_BEGIN

namespace video
{

static WindowHandle *GLContextHandle = nullptr;
static bool IsInit = false;


namespace detail
{

bool InitContext()
{
	if (IsInit) {
        return true;
    }

    IsInit = true;
    log::info("Initializing GLFW");
    glfwSetErrorCallback(OnGLFWError);

    if (glfwInit() != GLFW_TRUE) {
        error::Raise(error::Code::GLFW_ERROR, "Could not initialize GLFW");
        return false;
    }

    log::info("GLFW version: ", glfwGetVersionString());

    GLContextHandle = CreateWindowHandle();
    glfwMakeContextCurrent(TO_GLFW(GLContextHandle));

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

    log::info("OpenGL version: ", glGetString(GL_VERSION));
    log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
    log::info("Renderer: ", glGetString(GL_RENDERER));
    log::info("Vendor: ", glGetString(GL_VENDOR));

    log::debug("Generating standard meshes");

    bool success = true;

    GenericMeshes.rectMask.setAttributeDescriptors({
                                                       AttributeType::VEC2 // position
                                                   });
    GenericMeshes.rectMask.insertVertices({{1.f, 1.f},
                                              {1.f, -1.f},
                                              {-1.f, -1.f},
                                              {-1.f, 1.f}});
    GenericMeshes.rectMask.insertIndices({0, 1, 2, 0, 3, 2});

    // Create geometry for rectangular meshes
    GenericMeshes.rect.setAttributeDescriptors({
                                                   AttributeType::VEC2, // position
                                                   AttributeType::VEC2  // uv
                                               });
    GenericMeshes.rect.insertVertices({{1.f, 1.f, 1.f, 1.f},
                                          {1.f, -1.f, 1.f, 0.f},
                                          {-1.f, -1.f, 0.f, 0.f},
                                          {-1.f, 1.f, 0.f, 1.f}});
    GenericMeshes.rect.insertIndices({0, 1, 2, 0, 3, 2});

    const unsigned int resolution = 32;

    double circle_rotation = 2.0_pi / (resolution - 1);
    fvec2d circle_vec = {1.f, 0.f};

    GenericMeshes.circle.setAttributeDescriptors({
                                                     AttributeType::VEC2, // position
                                                     AttributeType::VEC2  // uv
                                                 });
    GenericMeshes.circle.insertVertex({0.0, 0.0, 0.5f, 0.5f});

    for (unsigned int i = 0; i < resolution; i++) {
        fvec2d vec = {circle_vec.x, circle_vec.y};
        fvec2d uv = (vec + 1.0f) / 2.0f;
        GenericMeshes.circle.insertVertex({vec.x, vec.y, uv.x, uv.y});
        GenericMeshes.circle.insertIndice(i);
        circle_vec = math::Rotate(circle_vec, circle_rotation);
    }
    GenericMeshes.circle.insertIndice(1);

    if (!success)
        log::error("Failed to generate static geometry");

    return success;
}

void DestroyContext()
{
	if (IsInit) {
		log::info("Closing GLFW");
        FreeWindowHandle(GLContextHandle);
        GLContextHandle = nullptr;
		glfwTerminate();
		IsInit = false;
	}
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
