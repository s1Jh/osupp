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

#include "Mesh.hpp"

#include <sstream>
#include <utility>
#include <filesystem>

#include "GL.hpp"
#include "Math.hpp"
#include "MeshLoaders.hpp"
#include "Util.hpp"
#include "Vector.hpp"

namespace PROJECT_NAMESPACE {

namespace video
{

Mesh::Mesh() noexcept
	: renderMode(RenderMode::Triangles)
{ clear(); }

void Mesh::clear() noexcept
{
	vertices.clear();
	indices.clear();
	dataDescriptors = {AttributeType::VEC3}; // just position
	totalDataPerVertex = 3;
};

void Mesh::setAttributeDescriptors(std::vector<AttributeType> descriptors)
{
	clear();
	dataDescriptors = std::move(descriptors);

	totalDataPerVertex = 0;
	for (const auto &desc : dataDescriptors) {
		totalDataPerVertex += (int)desc;
	}
}

void Mesh::addAttributeDescriptor(AttributeType desc)
{
	clear();
	dataDescriptors.push_back(desc);

	totalDataPerVertex = 0;
	for (const auto &descriptor : dataDescriptors) {
		totalDataPerVertex += (int)descriptor;
	}
}

unsigned int Mesh::getAttributeCount()
{ return dataDescriptors.size(); }

unsigned int Mesh::insertVertex(const Vertex &v)
{
	unsigned int index = vertices.size() / totalDataPerVertex;

	int i = 0;
	for (; i < math::Min((int)v.size(), totalDataPerVertex); i++) {
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

	for (auto &v : vs)
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
	for (auto &i : is)
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

int Mesh::getVertexCount() const
{ return vertexCount; }

int Mesh::getElementCount() const
{ return elementCount; }

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

std::optional<GLMeshObjs> Mesh::createData()
{
	GLMeshObjs data;

	glGenVertexArrays(1, &data.VAO);
	glGenBuffers(1, &data.EBO);
	glGenBuffers(1, &data.VBO);
	if (CheckGLh("Buffer creation") != 0) {
		return {};
	}

	vertexCount = (int)vertices.size() / totalDataPerVertex;
	elementCount = (int)indices.size();

	glBindVertexArray(data.VAO);
	if (CheckGLh("Bind") != 0) {
		return {};
	}
	glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
	glBufferData(GL_ARRAY_BUFFER, (long)vertices.size() * sizeof(float),
				 vertices.data(), GL_STATIC_DRAW);
	if (CheckGLh("Vertex data upload") != 0) {
		return {};
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 (long)indices.size() * sizeof(unsigned int), indices.data(),
				 GL_STATIC_DRAW);
	if (CheckGLh("Element data upload") != 0) {
		return {};
	}

	long offset = 0;

	for (unsigned int i = 0; i < dataDescriptors.size(); i++) {
		int elements = (int)dataDescriptors[i];
		unsigned size = elements * sizeof(float);
		glVertexAttribPointer(i, elements, GL_FLOAT, GL_FALSE,
							  totalDataPerVertex * (int)sizeof(float),
							  (void *)offset);
		glEnableVertexAttribArray(i);
		offset += size;
		CheckGLh("Setting attribute pointer " + std::to_string(i));
	}

	glBindVertexArray(0);
	if (CheckGLh("Done") != 0) {
		return {};
	}

	return data;
}

void Mesh::deleteData(const GLMeshObjs &obj)
{
	if (obj.VAO != 0)
		glDeleteVertexArrays(1, &obj.VAO);
	if (obj.VBO != 0)
		glDeleteBuffers(1, &obj.VBO);
	if (obj.EBO != 0)
		glDeleteBuffers(1, &obj.EBO);
	CheckGLh("Buffer deletion");

}

}

template<>
Resource<video::Mesh> Load(const std::filesystem::path &path)
{
	Resource<video::Mesh> r;
    if (!LoadOBJ(path, *r.held))
		return {nullptr};

	return r;
}

template<>
Resource<video::Mesh> Create()
{
	Resource<video::Mesh> r;
    r->setAttributeDescriptors({video::AttributeType::VEC2});
	r->insertVertices({{-1.f, -1.f}, {0.f, 1.f}, {1.f, -1.f}});
	r->insertIndices({0, 1, 2});

	return r;

}

}