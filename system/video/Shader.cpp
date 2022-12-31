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

#define INCLUDE_SHADER_METHOD_DEFINITIONS

#include "Shader.hpp"

#include <fstream>

#include "GL.hpp"
#include "Util.hpp"
#include "Shaders.hpp"
#include "GraphicsContext.hpp"

NS_BEGIN

namespace video
{

void Shader::fromString(const std::string &vert_src,
                        const std::string &frag_src,
                        const std::string &geom_src)
{
    vertSrc = vert_src;
	fragSrc = frag_src;
	geomSrc = geom_src;
	invalidate();
}

unsigned int Shader::CompileShader(const std::string &file, unsigned int type)
{
    int compile_state;
    char info_log[512];

    auto cbuff = file.c_str();

    auto shader = glCreateShader(type);
    CheckGLh("SHADER::LOAD_SHADER: Creation");
    glShaderSource(shader, 1, &cbuff, nullptr);
    CheckGLh("SHADER::LOAD_SHADER: Setting source");
    glCompileShader(shader);
    CheckGLh("SHADER::LOAD_SHADER: Compilation");

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_state);

    if (!compile_state) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        log::error("Shader compilation error:\n", info_log);
    }

    return shader;
}

void Shader::use() const
{ glUseProgram(getGLData()); }

void Shader::unbind()
{ glUseProgram(0); }

#define GENERATE_SHADER_SET_FUNC(TYPE, GL_POSTFIX, PTR_TYPE)                   \
  bool Shader::set(const std::string &name, const TYPE &value)                 \
      const noexcept {                                                         \
    if (!uploaded()) {                                                         \
    	return false; 														   \
	}                                                                          \
    unsigned int id = getGLData();                                             \
    int loc = glGetUniformLocation(id, name.c_str());                          \
    if (loc == -1) {                                              			   \
      	return false;                                                          \
	}                                                                          \
    glUniform##GL_POSTFIX##v(loc, 1, (const PTR_TYPE *)(&value));              \
    return true;                                                               \
  }

SHADER_SETTER_METHODS

#undef GENERATE_SHADER_SET_FUNC
#define GENERATE_SHADER_SET_FUNC(TYPE, GL_POSTFIX, PTR_TYPE)                   \
  bool Shader::set(const std::string &name, const TYPE &value)                 \
      const noexcept {                                                         \
    if (!uploaded()) {                                                         \
    	return false; 														   \
	}                                                                          \
    unsigned int id = getGLData();                                             \
    int loc = glGetUniformLocation(id, name.c_str());                          \
    if (loc == -1) {                                              			   \
      	return false;                                                          \
	}                                                                          \
    glUniform##GL_POSTFIX##v(loc, 1, GL_FALSE, (const PTR_TYPE *)(&value));    \
    return true;                                                               \
  }

SHADER_MATRIX_SETTER_METHODS

#undef GENERATE_SHADER_SET_FUNC
#define GENERATE_SHADER_SET_FUNC(TYPE, GL_POSTFIX, PTR_TYPE, CAST_TYPE)        \
  bool Shader::set(const std::string &name, const TYPE &value)                 \
      const noexcept {                                                         \
    if (!uploaded()) {                                                         \
    	return false; 														   \
	}                                                                          \
    unsigned int id = getGLData();                                             \
    int loc = glGetUniformLocation(id, name.c_str());                          \
    if (loc == -1) {                                              			   \
      	return false;                                                          \
	}                                                                          \
    auto cast = (CAST_TYPE)value;                                              \
    glUniform##GL_POSTFIX##v(loc, 1, (const PTR_TYPE *)(&cast));               \
    return true;                                                               \
  }

SHADER_INTER_CAST_SETTER_METHODS

std::optional<unsigned int> Shader::createData()
{
	unsigned int id = glCreateProgram();

	unsigned int vert = CompileShader(vertSrc, GL_VERTEX_SHADER);
	glAttachShader(id, vert);

	unsigned int frag = CompileShader(fragSrc, GL_FRAGMENT_SHADER);
	glAttachShader(id, frag);

	unsigned int geom = 0;
	if (!geomSrc.empty()) {
		geom = CompileShader(geomSrc, GL_GEOMETRY_SHADER);
		glAttachShader(id, geom);
	}

	vertSrc.clear();
	fragSrc.clear();
	geomSrc.clear();

	glLinkProgram(id);
	glDeleteShader(vert);
	glDeleteShader(frag);

	if (geom != 0)
		glDeleteShader(geom);

	int link_state;
	char info_log[512];

	glGetProgramiv(id, GL_LINK_STATUS, &link_state);
	if (!link_state) {
		glGetProgramInfoLog(id, 512, nullptr, info_log);
		log::error("Warning, an error occurred during Shader program linking:\n",
				   info_log, '\n');
		return {};
	}
	return {id};
}

void Shader::deleteData(const unsigned int &repr)
{
	glDeleteProgram(repr);
}

}

template<>
Resource<video::Shader> Create()
{
	Resource<video::Shader> r;

	r->fromString(
		std::string("#version ") + video::GL_VERSION_STR + '\n' + video::DEFAULT_VERTEX_SHADER,
		std::string("#version ") + video::GL_VERSION_STR + '\n' + video::DEFAULT_FRAGMENT_SHADER);
	return r;
}

template<>
Resource<video::Shader> Load(const std::filesystem::path &path)
{
	Resource<video::Shader> r;

	std::string frag_src, vert_src, geom_src;

	vert_src = std::string("#version ") + video::GL_VERSION_STR + '\n' + video::DEFAULT_VERTEX_SHADER;
	frag_src = std::string("#version ") + video::GL_VERSION_STR + '\n' + video::DEFAULT_FRAGMENT_SHADER;
	geom_src = "";


	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::string line;
		int bin = 0;
		while (std::getline(ifs, line)) {
			line += '\n';

			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					vert_src = "";
					bin = 0;
				}
				if (line.find("fragment") != std::string::npos) {
					frag_src = "";
					bin = 1;
				}
				if (line.find("geometry") != std::string::npos) {
					geom_src = "";
					bin = 2;
				}
				continue;
			}

			switch (bin) {
			case 0:
				vert_src += line;
				break;
			case 1:
				frag_src += line;
				break;
			case 2:
				geom_src += line;
				break;
			default:
				break;
			}
		}
	}
	ifs.close();

	r.held->fromString(vert_src, frag_src, geom_src);

	return r;
}

#undef GENERATE_SHADER_SET_FUNC
#undef INCLUDE_SHADER_METHOD_DEFINITIONS // make sure the macros don't get
// included inside of other stuff

NS_END