#ifndef ODDITYENGINE_TRACER_H
#define ODDITYENGINE_TRACER_H

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
using namespace glm;

namespace OddityEngine::Graphics {
    enum Objecttype {
        SPHERE,
        CUBE,
        MESH
    };

    struct alignas(16) bufferobject {
        mat4 transform;
        float radius;
        uint32 type;
        uint32 vertexstart;
        uint32 vertexcount;
    };

    struct alignas(16) buffermaterial {
        vec4 color;
        vec4 emission;
        float roughness;
    }

    class Tracer {

    };
}

#endif //ODDITYENGINE_TRACER_H
