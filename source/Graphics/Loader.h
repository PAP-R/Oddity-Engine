#ifndef ODDITYENGINE_LOADER_H
#define ODDITYENGINE_LOADER_H

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace OddityEngine {
    namespace Graphics {
        namespace Loader {
            struct Object {
                std::vector<glm::vec3> vertices;
                std::vector<glm::vec3> normals;
                std::vector<glm::vec3> colors;
                std::vector<glm::vec2> uvs;
                std::vector<glm::vec<3, size_t>> faces;
            };

            Object obj(const std::string& path);
        } // Loader
    } // OddityEngine
} // Graphics

#endif //ODDITYENGINE_LOADER_H
