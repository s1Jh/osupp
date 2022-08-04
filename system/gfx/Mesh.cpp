#include "Mesh.hpp"

#include <fstream>
#include <sstream>
#include <utility>

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>
#include <filesystem>

#include "Line.hpp"
#include "Math.hpp"
#include "MeshLoaders.hpp"
#include "Util.hpp"
#include "Vec3.hpp"

NS_BEGIN

Mesh::Mesh() noexcept
    : renderMode(RenderMode::Triangles)
{ clear(); }

void Mesh::clear() noexcept
{
    vertices.clear();
    indices.clear();
    dataDescriptors = {AttributeType::Vec3}; // just position
    totalDataPerVertex = 3;
};

void Mesh::setAttributeDescriptors(std::vector<AttributeType> descriptors)
{
    clear();
    dataDescriptors = std::move(descriptors);

    totalDataPerVertex = 0;
    for (const auto &desc: dataDescriptors) {
        totalDataPerVertex += (int) desc;
    }
}

void Mesh::addAttributeDescriptor(AttributeType desc)
{
    clear();
    dataDescriptors.push_back(desc);

    totalDataPerVertex = 0;
    for (const auto &descriptor: dataDescriptors) {
        totalDataPerVertex += (int) descriptor;
    }
}

unsigned int Mesh::getAttributeCount()
{ return dataDescriptors.size(); }

unsigned int Mesh::insertVertex(const Vertex &v)
{
    unsigned int index = vertices.size() / totalDataPerVertex;

    int i = 0;
    for (; i < Min((int) v.size(), totalDataPerVertex); i++) {
        vertices.push_back(v[i]);
    }
    for (; i < totalDataPerVertex; i++) {
        vertices.push_back(0.f);
    }

    vertexCount += totalDataPerVertex;
    return index;
}

unsigned int Mesh::insertVertices(const std::vector<Vertex> &vs)
{
    unsigned int index = vertices.size() / totalDataPerVertex;

    for (auto &v: vs)
        insertVertex(v);

    return index;
}

void Mesh::insertIndice(unsigned int i)
{
    indices.push_back(i);
    elementCount++;
}

void Mesh::insertIndices(const std::vector<unsigned int> &is,
                         unsigned int offset)
{
    for (auto &i: is)
        insertIndice(i + offset);
}

unsigned int Mesh::insertTriangle(const Vertex &v1, const Vertex &v2,
                                  const Vertex &v3)
{
    unsigned int first = vertexCount / totalDataPerVertex;

    insertIndice(insertVertex(v1));
    insertIndice(insertVertex(v2));
    insertIndice(insertVertex(v3));

    return first;
}

const Mat4<float> &Mesh::getTransform() const
{ return meshTransform; }

void Mesh::setTransform(Mat4<float> mat)
{ meshTransform = std::move(mat); }

bool Mesh::isValid() const
{
    if (data)
        return data->VAO != 0 && data->VBO != 0 && data->EBO != 0;
    return false;
}

void Mesh::deleteMesh()
{
    if (!data)
        return;

    if (data->VAO != 0)
        glDeleteVertexArrays(1, &data->VAO);
    if (data->VBO != 0)
        glDeleteBuffers(1, &data->VBO);
    if (data->EBO != 0)
        glDeleteBuffers(1, &data->EBO);
    CheckGLh("Buffer deletion");
}

bool Mesh::upload()
{
    data.reset(new GLObjs, GLObjDeleter);
    deleteMesh();

    glGenVertexArrays(1, &data->VAO);
    glGenBuffers(1, &data->EBO);
    glGenBuffers(1, &data->VBO);
    if (CheckGLh("Buffer creation") != 0) {
        deleteMesh();
        return false;
    }

    vertexCount = (int) vertices.size() / totalDataPerVertex;
    elementCount = (int) indices.size();

    glBindVertexArray(data->VAO);
    if (CheckGLh("Bind") != 0) {
        deleteMesh();
        return false;
    }
    glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
    glBufferData(GL_ARRAY_BUFFER, (long) vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
    if (CheckGLh("Vertex data upload") != 0) {
        deleteMesh();
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (long) indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);
    if (CheckGLh("Element data upload") != 0) {
        deleteMesh();
        return false;
    }

    long offset = 0;

    for (unsigned int i = 0; i < dataDescriptors.size(); i++) {
        int elements = (int) dataDescriptors[i];
        unsigned size = elements * sizeof(float);
        glVertexAttribPointer(i, elements, GL_FLOAT, GL_FALSE,
                              totalDataPerVertex * (int) sizeof(float),
                              (void *) offset);
        glEnableVertexAttribArray(i);
        offset += size;
        CheckGLh("Setting attribute pointer " + std::to_string(i));
    }

    glBindVertexArray(0);
    if (CheckGLh("Done") != 0) {
        deleteMesh();
        return false;
    }

    clear();

    return true;
}

int Mesh::getVertexCount() const
{ return vertexCount; }

int Mesh::getElementCount() const
{ return elementCount; }

unsigned int Mesh::getVAO() const
{
    if (data)
        return data->VAO;
    return 0;
}

unsigned int Mesh::getVBO() const
{
    if (data)
        return data->VBO;
    return 0;
}

unsigned int Mesh::getEBO() const
{
    if (data)
        return data->EBO;
    return 0;
}

RenderMode Mesh::getRenderMode() const
{ return renderMode; }

void Mesh::setRenderMode(RenderMode renderModeIn)
{
    Mesh::renderMode = renderModeIn;
}

unsigned int Mesh::getAttributeDescriptorCount() const
{
    return dataDescriptors.size();
}

void Mesh::GLObjDeleter(Mesh::GLObjs *obj)
{
    log::debug("Deleting mesh ", obj->VAO, " (", obj, ')');
    if (obj->VAO != 0)
        glDeleteVertexArrays(1, &obj->VAO);
    if (obj->VBO != 0)
        glDeleteBuffers(1, &obj->VBO);
    if (obj->EBO != 0)
        glDeleteBuffers(1, &obj->EBO);
    CheckGLh("Buffer deletion");
}

bool Mesh::load(const std::filesystem::path &path)
{
    return LoadOBJ(path, *this);
}

bool Mesh::create()
{
    setAttributeDescriptors({AttributeType::Vec2});
    insertVertices({{-1.f, -1.f}, {0.f, 1.f}, {1.f, -1.f}});
    insertIndices({0, 1, 2});

    return upload();
}

void detail::RenderFunctor<Mesh>::operator()(Renderer &renderer,
                                             const Mesh &mesh,
                                             const Shader &shader,
                                             const Shader::Uniforms &uniforms,
                                             const Shader::Textures &textures,
                                             const Mat3f &transform)
{
    if (!mesh.isValid())
        return;

    shader.use();
    CheckGLh("Bound shader");

    for (auto &uniform: uniforms) {
#define MATCHF(type)                                                           \
  if constexpr (std::is_same_v<T, type>)                                       \
    shader.set(uniform.first, arg);
#define MATCH(type)                                                            \
  else if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, arg);
#define MATCHP(type)                                                           \
  else if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, *arg);
#define END_MATCH()                                                            \
  else log::warning("Unable to deduce type for shader uniform ", uniform.first);
        std::visit(
            [&](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;
                MATCHF(float)
                MATCH(int)MATCH(unsigned int) MATCH(double) MATCH(fvec2d) MATCH(ivec2d)MATCH(uvec2d) MATCH(dvec2d) MATCH(
                    fvec3d) MATCH(ivec3d)MATCH(uvec3d) MATCH(dvec3d) MATCH(fvec4d) MATCH(ivec4d)MATCH(uvec4d) MATCH(
                    dvec4d) MATCH(color) MATCHP(fvec2d *)MATCHP(ivec2d *) MATCHP(uvec2d *) MATCHP(dvec2d *)MATCHP(fvec3d *) MATCHP(
                    ivec3d *) MATCHP(uvec3d *)MATCHP(dvec3d *) MATCHP(fvec4d *)MATCHP(ivec4d *) MATCHP(uvec4d *)MATCHP(
                    dvec4d *) MATCHP(Mat2f *)MATCHP(Mat3f *) MATCHP(Mat4f *)MATCHP(color *) END_MATCH()
            },
            uniform.second);
#undef MATCHF
#undef MATCH
#undef MATCHP
#undef END_MATCH
    }
    CheckGLh("Set shader uniforms");

    shader.set("transform", transform);
    shader.set("camera", renderer.camera.getMatrix());
    DumpGlErrors();

    for (auto &texture: textures) {
        texture.second->use(texture.first);
    }
    CheckGLh("Bound textures");

    glBindVertexArray(mesh.getVAO());
    glDrawElements(static_cast<unsigned int>(mesh.getRenderMode()),
                   mesh.getElementCount(), GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    for (auto &texture: textures) {
        Texture::unbind(texture.first);
    }
    CheckGLh("Unbound");
}

NS_END