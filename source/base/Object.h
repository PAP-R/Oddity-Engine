#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include <vector>

using namespace std;

#include "glm/glm.hpp"

using namespace glm;

#include "HalfEdge.h"

class Object {
public:
    vec3 pos{0.0f, 0.0f, 0.0f};
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
