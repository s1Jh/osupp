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

#include "MeshLoaders.hpp"

#include "define.hpp"

#include "Util.hpp"
#include "Vec3.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

NS_BEGIN

bool LoadOBJ(const std::string &fpath, Mesh &mesh)
{
    LOG_ENTER("OBJ LOAD");
    std::ifstream ifs(fpath);
    if (!ifs.is_open()) {
        return false;
    }

    // will also clear the current mesh
    mesh.setAttributeDescriptors({
                                     AttributeType::VEC3, // position
                                     AttributeType::VEC3, // uv/w coordinate
                                     AttributeType::VEC3  // normal
                                 });

    std::string strline;
    std::vector<fvec3d> verts, uvs, normals;
    std::vector<std::vector<unsigned int>> faces;

    while (std::getline(ifs, strline)) {
        std::istringstream line(strline);

        std::string id;

        line >> id;

        if (id == "#") {
            // skip
        }
        else if (id == "v") {
            // vertex
            fvec3d v;
            line >> v.x >> v.y >> v.z;
            verts.push_back(v);
        }
        else if (id == "vt") {
            // vertex uv
            fvec3d v;
            line >> v.x >> v.y >> v.z;
            uvs.push_back(v);
        }
        else if (id == "vn") {
            // vertex normal
            fvec3d v;
            line >> v.x >> v.y >> v.z;
            normals.push_back(v);
        }
        else if (id == "f") {
            // face
            std::vector<unsigned int> this_face(9);
            std::string face_vertex, face_vertex_entry;
            int i = 0;
            while (std::getline(line, face_vertex, ' ')) {
                int j = 0;
                std::stringstream vertex_stream(face_vertex);
                while (std::getline(vertex_stream, face_vertex_entry, '/')) {
                    if (!face_vertex_entry.empty()) {
                        this_face[3 * i + j] = std::stoi(face_vertex_entry);
                        j++;
                    }
                }

                if (!face_vertex.empty())
                    i++;
            }

            faces.push_back(this_face);
        }
        else if (id == "usemtl") {
            // skip
        }
    }

    log::info("Read: ", verts.size(), " vertex positions, ", uvs.size(),
              " uv coordinates, ", normals.size(), " normals");

    for (unsigned int i = 0; i < verts.size(); i++) {
        mesh.insertVertex({i < verts.size() ? verts[i].x : 0.f,
                           i < verts.size() ? verts[i].y : 0.f,
                           i < verts.size() ? verts[i].z : 0.f,

                           i < uvs.size() ? uvs[i].x : 0.f,
                           i < uvs.size() ? uvs[i].y : 0.f,
                           i < uvs.size() ? uvs[i].z : 0.f,

                           i < normals.size() ? normals[i].x : 0.f,
                           i < normals.size() ? normals[i].y : 0.f,
                           i < normals.size() ? normals[i].z : 0.f});
    }

    for (auto &face: faces) {
        for (int i = 0; i < 3; i++) {
            mesh.insertIndice(face[i * 3] - 1);
        }
    }

    mesh.upload();
    return true;
}

NS_END
