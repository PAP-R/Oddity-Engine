#ifndef ODDITYENGINE_OBJECT_H
#define ODDITYENGINE_OBJECT_H

#include <vector>

using namespace std;

#include "glm/glm.hpp"

using namespace glm;

#include "Point.h"

class Object {
public:
    vector<Point> points;
    vector<float> vertices;

    Object();
    explicit Object(const vector<Point> &points);
};

#endif //ODDITYENGINE_OBJECT_H
