#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include <vector>

using namespace std;

#include "glm/glm.hpp"

using namespace glm;

#include "Point.h"

class Object {
public:
    vec3 pos{0.0f, 0.0f, 0.0f};
    vector<Point> points;

    vector<float> vertices;

    Object();
    explicit Object(const vector<Point> &points);
};

#endif //ODDITYENGINE_OBJECT_H
