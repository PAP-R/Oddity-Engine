#ifndef ODDITYENGINE_LOADER_H
#define ODDITYENGINE_LOADER_H

#include "glm/glm.hpp"
using namespace glm;

#include <string>
#include <vector>
using namespace std;

namespace OddityEngine::Graphics::Loader {
    struct Object {
        vector<vec3> vertices;
        vector<vec3> normals;
        vector<vec3> colors;
        vector<vec2> uvs;
        vector<vec<3, size_t>> faces;
    };

    Object obj(string path);
}

#endif //ODDITYENGINE_LOADER_H
