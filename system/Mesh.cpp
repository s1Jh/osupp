#include "Mesh.hpp"

#include <fstream>
#include <sstream>
#include <utility>

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include "Line.hpp"
#include "Math.hpp"
#include "MeshLoaders.hpp"
#include "Util.hpp"
#include "Vec3.hpp"

NS_BEGIN

Mesh::Mesh()
    : renderMode(RenderMode::Triangles)
{ clear(); }

void Mesh::clear()
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
{ meshTransform = mat; }

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

bool Mesh::load(const std::string &path, Resources *res)
{
    return LoadOBJ(path, *this);
}

bool Mesh::create(Resources *res)
{
    setAttributeDescriptors({AttributeType::Vec2});
    insertVertices({{-1.f, -1.f}, {0.f, 1.f}, {1.f, -1.f}});
    insertIndices({0, 1, 2});

    return upload();
}

NS_END