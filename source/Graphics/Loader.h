#ifndef ODDITYENGINE_LOADER_H
#define ODDITYENGINE_LOADER_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <Graphics/Object/Object.h>

namespace OddityEngine {
    namespace Graphics {
        namespace Loader {
            struct Object {
                std::vector<glm::vec3> vertices;
                std::vector<glm::vec3> normals;
                std::vector<glm::vec3> colors;
                std::vector<glm::vec2> uvs;
                std::vector<glm::vec<3, size_t>> faces;

                std::vector<glm::vec4> get_vertices() {
                    std::vector<glm::vec4> _vertices;
                    for (auto i : get_vertex_indices()) {
                        _vertices.emplace_back(vertices[i], 1);
                    }
                    return _vertices;
                }

                std::vector<glm::vec3> get_indexed_vertices() {
                    return vertices;
                }

                std::vector<size_t> get_vertex_indices() {
                    std::vector<size_t> indices;
                    for (auto f : faces) {
                        indices.emplace_back(f[0] - 1);
                    }
                    return indices;
                }

                std::vector<glm::vec4> get_normals() {
                    std::vector<glm::vec4> _normals;
                    for (auto i : get_normal_indices()) {
                        _normals.emplace_back(normals[i], 0);
                    }
                    return _normals;
                }

                std::vector<size_t> get_normal_indices() {
                    std::vector<size_t> indices;
                    for (auto f : faces) {
                        indices.emplace_back(f[2] - 1);
                    }
                    return indices;
                }

                std::vector<glm::vec4> get_uvs() {
                    std::vector<glm::vec4> _uvs;
                    for (auto i : get_vertex_indices()) {
                        _uvs.emplace_back(uvs[i], 0, 1);
                    }
                    return _uvs;
                }

                std::vector<size_t> get_uv_indices() {
                    std::vector<size_t> indices;
                    for (auto f : faces) {
                        indices.emplace_back(f[1] - 1);
                    }
                    return indices;
                }
            };

            Object obj_file(const std::string& path);

            Graphics::Object* object(const std::string& path);
        } // Loader
    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_LOADER_H
