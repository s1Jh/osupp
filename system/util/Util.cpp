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

#include "Util.hpp"

#include "GL.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <AL/al.h>
#include <algorithm>

namespace PROJECT_NAMESPACE {
namespace detail
{

int CheckGLFWErrors(const std::string &file, int line,
                    const std::string &helper)
{
    return 0;
}

unsigned int CheckGLErrors(const std::string &file, int line,
                         const std::string &helper)
{
    GLenum error = glGetError();
    const char *err_str;
    if (error != 0) {
        switch (error) {
            case GL_INVALID_ENUM:
                err_str = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                err_str = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                err_str = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                err_str = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                err_str = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                err_str = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                err_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                err_str = "!INVALID ERROR!";
                break;
        }

        log::Error("@(", helper, ") Near line: ", line, " In File: ", file, ": ",
                   err_str);
    }
    return error;
}

unsigned int CheckALErrors(const std::string& file, int line, const std::string& helper)
{
	ALenum error = alGetError();
	std::string err_str;
	if(error != AL_NO_ERROR)
	{
		switch(error)
		{
		case AL_INVALID_NAME:
			err_str = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
			break;
		case AL_INVALID_ENUM:
			err_str = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
			break;
		case AL_INVALID_VALUE:
			err_str = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case AL_INVALID_OPERATION:
			err_str = "AL_INVALID_OPERATION: the requested operation is not valid";
			break;
		case AL_OUT_OF_MEMORY:
			err_str = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
			break;
		default:
			err_str = "UNKNOWN AL ERROR: " + std::to_string(error);
		}
		log::Error("@(", helper, ") Near line: ", line, " In File: ", file, ": ",
				   err_str);
	}
	return error;
}
} // namespace detail

unsigned int DumpGlErrors()
{ return glGetError(); }

std::vector<std::string> GetCharacterSeparatedValues(const std::string &in, char sep)
{
    std::vector<std::string> results;

    std::stringstream ss(in);
    std::string str;
    while (getline(ss, str, sep)) {
        results.push_back(str);
    }

    return results;
}

}
