#pragma once

#include <string>
#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include "../types/Vec2.hpp"
#include "../types/Vec3.hpp"
#include "../types/Vec4.hpp"
#include "../types/Color.hpp"
#include "../types/Matrix.hpp"
#include "../define.hpp"
#include "Texture.hpp"

#include <unordered_map>
#include <string>
#include <variant>

namespace GAME_TITLE {

    class Shader {
    public:
        Shader();

        explicit Shader(const std::string &path);

        bool fromString(const std::string &vert_src, const std::string &frag_src);

        bool load(const std::string &path);

        // Calls OpenGL's glUseProgram with it's id.
        void use() const;

        // Unbinds currently active program.
        static void unbind();

        // Returns it's ID.
        [[nodiscard]] GLuint getID() const;

        // Setters that set a value and handle any errors
        // 1 Dimensional variables
        void set(const std::string &name, int) const;

        void set(const std::string &name, float) const;

        void set(const std::string &name, unsigned int) const;

        void set(const std::string &name, double) const;

        // 2D vectors
        void set(const std::string &name, const fvec2d &value) const;

        void set(const std::string &name, const ivec2d &value) const;

        void set(const std::string &name, const uvec2d &value) const;

        void set(const std::string &name, const dvec2d &value) const;

        // 3D vectors
        void set(const std::string &name, const fvec3d &value) const;

        void set(const std::string &name, const ivec3d &value) const;

        void set(const std::string &name, const uvec3d &value) const;

        void set(const std::string &name, const dvec3d &value) const;

        // 4D vectors
        void set(const std::string &name, const fvec4d &value) const;

        void set(const std::string &name, const ivec4d &value) const;

        void set(const std::string &name, const uvec4d &value) const;

        void set(const std::string &name, const dvec4d &value) const;

        // Matrices
        void set(const std::string &name, const Mat2f &value) const;

        void set(const std::string &name, const Mat3f &value) const;

        void set(const std::string &name, const Mat4f &value) const;

        // Misc.
        void set(const std::string &name, const color &value) const;

        using Textures = std::unordered_map<int, Texture *>;
        using Uniforms = std::unordered_map<std::string, std::variant<
                int, float, unsigned int, double,
                fvec2d, ivec2d, uvec2d, dvec2d,
                fvec3d, ivec3d, uvec3d, dvec3d,
                fvec4d, ivec4d, uvec4d, dvec4d,
                fvec2d *, ivec2d *, uvec2d *, dvec2d *,
                fvec3d *, ivec3d *, uvec3d *, dvec3d *,
                fvec4d *, ivec4d *, uvec4d *, dvec4d *,
                Mat2f *, Mat3f *, Mat4f *, color *, Texture *>>;

    private:
        static unsigned int compileShader(const std::string &file, unsigned int type);

        GLuint id;
    };
}