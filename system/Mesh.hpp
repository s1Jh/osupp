#pragma once

#include "define.hpp"
#include "Matrix.hpp"
#include <vector>
#include <memory>

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

namespace GAME_TITLE {

    enum class AttributeType {
        Scalar = 1, Vec2, Vec3, Vec4, Color = Vec4
    };

    class Mesh {
    public:
        Mesh();

        using Vertex = std::vector<float>;

        [[nodiscard]] const Mat4<float> &getTransform() const;

        void setTransform(Mat4<float>);

        bool load(const std::string &fpath);

        void clear();

        void setAttributeDescriptors(std::vector<AttributeType> descriptors);

        void addAttributeDescriptor(AttributeType desc);

        unsigned int getAttributeCount();

        unsigned int insertVertex(const Vertex &v);

        unsigned int insertVertices(const std::vector<Vertex> &vs);

        void insertIndice(unsigned int i);

        void insertIndices(const std::vector<unsigned int> &is);

        unsigned int insertTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);

        [[nodiscard]] bool isValid() const;

        bool upload();

        void deleteMesh();

        [[nodiscard]] int getVertexCount() const;

        [[nodiscard]] int getElementCount() const;

        [[nodiscard]] unsigned int getVAO() const;

        [[nodiscard]] unsigned int getVBO() const;

        [[nodiscard]] unsigned int getEBO() const;

        [[nodiscard]] unsigned int getRenderMode() const;

        void setRenderMode(unsigned int renderMode);

        [[nodiscard]] unsigned int getAttributeDescriptorCount() const;

    private:
        struct GLObjs {
            unsigned int VAO = 0;
            unsigned int VBO = 0;
            unsigned int EBO = 0;
        };

        static void GLObjDeleter(GLObjs *obj);

        Mat4<float> meshTransform;
        std::vector<AttributeType> dataDescriptors;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        unsigned int renderMode = GL_TRIANGLES;

        int totalDataPerVertex = 0;
        int vertexCount = 0;
        int elementCount = 0;

        std::shared_ptr<GLObjs> data;
    };
}