#include "MeshLoaders.hpp"

#include "define.hpp"

#include "Vec3.hpp"
#include "Util.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace GAME_TITLE {

    bool LoadOBJ(const std::string &fpath, Mesh& mesh) {
        LOG_ENTER("OBJ LOAD");
        std::ifstream ifs(fpath);
        if (!ifs.is_open()) {
            return false;
        }

        // will also clear the current mesh
        mesh.setAttributeDescriptors({
                                        AttributeType::Vec3, // position
                                        AttributeType::Vec3, // uv/w coordinate
                                        AttributeType::Vec3  // normal
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
            } else if (id == "v") {
                // vertex
                fvec3d v;
                line >> v.x >> v.y >> v.z;
                verts.push_back(v);
            } else if (id == "vt") {
                // vertex uv
                fvec3d v;
                line >> v.x >> v.y >> v.z;
                uvs.push_back(v);
            } else if (id == "vn") {
                // vertex normal
                fvec3d v;
                line >> v.x >> v.y >> v.z;
                normals.push_back(v);
            } else if (id == "f") {
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
            } else if (id == "usemtl") {
                // skip
            }
        }

        log::info("Read: ", verts.size(), " vertex positions, ", uvs.size(), " uv coordinates, ", normals.size(),
                  " normals");

        for (int i = 0; i < verts.size(); i++) {
            mesh.insertVertex({
                                 i < verts.size() ? verts[i].x : 0.f,
                                 i < verts.size() ? verts[i].y : 0.f,
                                 i < verts.size() ? verts[i].z : 0.f,

                                 i < uvs.size() ? uvs[i].x : 0.f,
                                 i < uvs.size() ? uvs[i].y : 0.f,
                                 i < uvs.size() ? uvs[i].z : 0.f,

                                 i < normals.size() ? normals[i].x : 0.f,
                                 i < normals.size() ? normals[i].y : 0.f,
                                 i < normals.size() ? normals[i].z : 0.f
                         });
        }

        for (auto &face: faces) {
            for (int i = 0; i < 3; i++) {
                mesh.insertIndice(face[i * 3] - 1);
            }
        }

        mesh.upload();
        return true;
    }
}