#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include <vector>

using namespace std;

#include "glm/glm.hpp"

using namespace glm;

#include "source/object/HalfEdge.h"
#include "Mesh.h"

class Object {
public:
    vec3 pos{0};
    vec3 dir{0, 1, 0};
    vec3 scale{1};

    Object(const vec3 &pos=vec3(0), const vec3 &dir=vec3(0, 1, 0), const vec3 &scale=vec3(1));
};

#endif //ODDITYENGINE_OBJECT_H
