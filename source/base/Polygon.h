#ifndef ODDITYENGINE_POLYGON_H
#define ODDITYENGINE_POLYGON_H

#include <glm/glm.hpp>

using namespace glm;

#include <vector>

using namespace std;

#include "Point.h"

class Polygon {
private:
    vector<Point> points;
    vector<Polygon*> neighbours;
    vec3 normal;
    vec3 pos;
};


#endif //ODDITYENGINE_POLYGON_H
