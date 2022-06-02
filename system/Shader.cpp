#include "Shader.hpp"

#include <fstream>
#include "Util.hpp"
#include "define.hpp"

namespace GAME_TITLE {

    Shader::Shader(const std::string &path) {
        load(path);
    }

    bool Shader::load(const std::string &path) {
        log::info("Loading shader ", path);
        std::string frag_src, vert_src;

        vert_src = std::string("#version ") + GL_VERSION_STR + '\n' + GL_DEFAULT_VERTEX_SHADER;
        frag_src = std::string("#version ") + GL_VERSION_STR + '\n' + GL_DEFAULT_FRAGMENT_SHADER;

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
                    continue;
                }

                if (bin == 0)
                    vert_src += line;
                else
                    frag_src += line;
            }
        }
        ifs.close();
        return fromString(vert_src, frag_src);
    }

    bool Shader::fromString(const std::string &vert_src, const std::string &frag_src) {
        log::info("Creating shader");
        unsigned int vert = compileShader(vert_src, GL_VERTEX_SHADER);
        unsigned int frag = compileShader(frag_src, GL_FRAGMENT_SHADER);

        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);
        glDeleteShader(vert);
        glDeleteShader(frag);

        int link_state;
        char info_log[512];

        glGetProgramiv(id, GL_LINK_STATUS, &link_state);
        if (!link_state) {
            glGetProgramInfoLog(id, 512, nullptr, info_log);
            log::warning("Warning, an error occurred during Shader program linking:\n", info_log, '\n');
            return false;
        }
        return true;
    }

    unsigned int Shader::compileShader(const std::string &file, unsigned int type) {
        int compile_state;
        char info_log[512];

        auto cbuff = file.c_str();

        auto shader = glCreateShader(type);
        glShaderSource(shader, 1, &cbuff, nullptr);
        glCompileShader(shader);
        CheckGLh ("SHADER::LOAD_SHADER: Compilation");

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_state);

        if (!compile_state) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            log::warning("Shader compilation error:\n", info_log);
        }

        return shader;
    }

    Shader::Shader() {
        id = -1;
    }

    void Shader::use() const {
        glUseProgram(this->id);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

    GLuint Shader::getID() const {
        return this->id;
    }

    void Shader::set(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::set(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::set(const std::string &name, double value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), (float) value);
    }

    void Shader::set(const std::string &name, unsigned int value) const {
        glUniform1ui(glGetUniformLocation(id, name.c_str()), value);
    }

// 2D Vectors
    void Shader::set(const std::string &name, const fvec2d &value) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void Shader::set(const std::string &name, const ivec2d &value) const {
        glUniform2i(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void Shader::set(const std::string &name, const uvec2d &value) const {
        glUniform2ui(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void Shader::set(const std::string &name, const dvec2d &value) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), (float) value.x, (float) value.y);
    }

// 3D Vectors
    void Shader::set(const std::string &name, const fvec3d &value) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::set(const std::string &name, const ivec3d &value) const {
        glUniform3i(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::set(const std::string &name, const uvec3d &value) const {
        glUniform3ui(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::set(const std::string &name, const dvec3d &value) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), (float) value.x, (float) value.y, (float) value.z);
    }

// 4D Vectors
    void Shader::set(const std::string &name, const fvec4d &value) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::set(const std::string &name, const ivec4d &value) const {
        glUniform4i(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::set(const std::string &name, const uvec4d &value) const {
        glUniform4ui(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::set(const std::string &name, const dvec4d &value) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), (float) value.x, (float) value.y, (float) value.z,
                    (float) value.w);
    }

    void Shader::set(const std::string &name, const Mat2f &value) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, false, value.GetCPtr());
    }

    void Shader::set(const std::string &name, const Mat3f &value) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, false, value.GetCPtr());
    }

    void Shader::set(const std::string &name, const Mat4f &value) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, value.GetCPtr());
    }

// Misc.
    void Shader::set(const std::string &name, const color &value) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), value.r, value.g, value.b, value.a);
    }
}