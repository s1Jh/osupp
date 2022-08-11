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

#include <mutex>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "Log.hpp"
#include "Util.hpp"

NS_BEGIN

namespace detail {

std::mutex CallMutex;
static int GraphicsUsers = 0;

void OnGLFWResize(WindowHandle *window, int width, int height)
{
	glfwMakeContextCurrent(TO_GLFW(window));
	glViewport(0, 0, width, height);
}

void OnGLFWError(int code, const char *msg)
{
	log::error("GLFW: [", code, "] ", msg);
}

WindowHandle* EnsureGraphicalContext() {
	std::scoped_lock<std::mutex> lock(CallMutex);
	WindowHandle *handle;
	if (GraphicsUsers <= 0) {
		// initiate a graphical context
		log::info("Creating a graphics context");
		glfwSetErrorCallback(OnGLFWError);

		if (glfwInit() != GLFW_TRUE) {
			log::error("Failed to initialize GLFW");
			CheckGLFW;
			return nullptr;
		}
		log::info("GLFW version: ", glfwGetVersionString());

#ifdef APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // DEBUG
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

		handle = FROM_GLFW(glfwCreateWindow(1, 1, TOSTRING(GAME_TITLE), nullptr, nullptr));

		glfwSetWindowSizeCallback(TO_GLFW(handle), reinterpret_cast<GLFWwindowsizefun>(OnGLFWResize));
		glfwMakeContextCurrent(TO_GLFW(handle));

		if (glewInit() != GLEW_OK) {
			log::error("Failed to load GLEW");
			glfwDestroyWindow(TO_GLFW(handle));
			return nullptr;
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
#endif // DEBUG

		log::info("OpenGL version: ", glGetString(GL_VERSION));
		log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
		log::info("Renderer: ", glGetString(GL_RENDERER));
		log::info("Vendor: ", glGetString(GL_VENDOR));

		glfwSwapInterval(0);
		glfwSetInputMode(TO_GLFW(handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		CheckGLFW;
	}

	GraphicsUsers++;
	return handle;
}

bool FreeGraphicalContext(WindowHandle* handle) {
	std::scoped_lock<std::mutex> lock(CallMutex);
	glfwDestroyWindow(TO_GLFW(handle));
	GraphicsUsers--;
	if (GraphicsUsers <= 0) {
		log::info("Closing graphics context");
		glfwTerminate();
	}
	return true;
}

}

NS_END
