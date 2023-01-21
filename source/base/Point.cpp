#include "Point.h"

Point::Point() = default;

Point::Point(const vec3 &pos) : pos(pos) {}

vector<float> Point::toFloats() {
    return {pos.x, pos.y, pos.z};
}

void Point::insertSelf(vector<float> * dest) {
    for (auto i = 0; i < 3; i++) {
        dest->emplace_back(pos[i]);
    }
}

Point::Point(const vec3 &pos, size_t id) : pos(pos), id(id) {}

Point::Point(const vec3 &pos, const vec3 &dir, size_t id) : pos(pos), dir(dir), id(id) {}
