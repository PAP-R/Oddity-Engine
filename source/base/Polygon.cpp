#include "Polygon.h"

#include "HalfEdge.h"
#include "glm/gtc/constants.hpp"

void Polygon::test(size_t pointCount, float radius) {
    vector<vec3> points;
    vector<size_t> pointI;
    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount), cos(i * 2 * pi<float>() / pointCount), 0.0f);
        pointI.emplace_back(i);
    }

    HalfEdge::addPolygon(pointI);
}
