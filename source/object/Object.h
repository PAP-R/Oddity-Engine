#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include <vector>

using namespace std;

#include "glm/glm.hpp"

using namespace glm;

#include "HalfEdge.h"

class Object {
public:
    vec3 pos{0};
    vec3 dir{0, 1, 0};
    vec3 scale{1};

    HalfEdge *edge;
    vector<vec3> points;
    vector<float> vertices;
    vector<float> uv;

    vec2 texPos{-1.0f};
    vec2 texSize{2.0f};

    vector<float> pointsToFloat();

    Object();
    explicit Object(const vector<vec3> &points);
};

#endif //ODDITYENGINE_OBJECT_H
