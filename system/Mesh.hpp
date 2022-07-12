#pragma once

#include "Matrix.hpp"
#include "Resource.hpp"
#include "define.hpp"

#include <memory>
#include <vector>
#include <filesystem>

NS_BEGIN

enum class AttributeType
{
    Scalar = 1, Vec2, Vec3, Vec4, Color = Vec4
};

// direct mapping to opengl render modes
enum class RenderMode
{
    Points,
    Lines,
    LineLoop,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads,
    QuadStrip,
    Polygon
};

class Mesh: public detail::Resource
{
public:
    Mesh();

    bool load(const std::filesystem::path &path) override;

    bool create() override;

    using Vertex = std::vector<float>;

    [[nodiscard]] const Mat4<float> &getTransform() const;

    void setTransform(Mat4<float>);

    void clear();

    void setAttributeDescriptors(std::vector<AttributeType> descriptors);

    void addAttributeDescriptor(AttributeType desc);

    unsigned int getAttributeCount();

    unsigned int insertVertex(const Vertex &v);

    unsigned int insertVertices(const std::vector<Vertex> &vs);

    void insertIndice(unsigned int i);

    void insertIndices(const std::vector<unsigned int> &is,
                       unsigned int offset = 0);

    unsigned int insertTriangle(const Vertex &v1, const Vertex &v2,
                                const Vertex &v3);

    [[nodiscard]] bool isValid() const;

    bool upload();

    void deleteMesh();

    [[nodiscard]] int getVertexCount() const;

    [[nodiscard]] int getElementCount() const;

    [[nodiscard]] unsigned int getVAO() const;

    [[nodiscard]] unsigned int getVBO() const;

    [[nodiscard]] unsigned int getEBO() const;

    [[nodiscard]] RenderMode getRenderMode() const;

    void setRenderMode(RenderMode renderMode);

    [[nodiscard]] unsigned int getAttributeDescriptorCount() const;

private:
    struct GLObjs
    {
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
    };

    static void GLObjDeleter(GLObjs *obj);

    Mat4<float> meshTransform;
    std::vector<AttributeType> dataDescriptors;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    RenderMode renderMode;

    int totalDataPerVertex = 0;
    int vertexCount = 0;
    int elementCount = 0;

    std::shared_ptr<GLObjs> data;
};

using MeshP = std::shared_ptr<Mesh>;

NS_END