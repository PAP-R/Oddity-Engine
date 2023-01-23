#ifndef ODDITYENGINE_POINT_H
#define ODDITYENGINE_POINT_H

#include <glm/glm.hpp>

using namespace glm;

#include <vector>

using namespace std;

class Point {
public:
    size_t id;
    vec3 pos;
    vec3 dir;
    vec3 color;

    vector<tuple<Point*, size_t>> cons;

    Point();

    explicit Point(const vec3 &pos);

    Point(const vec3 &pos, size_t id);

    Point(const vec3 &pos, const vec3 &dir, size_t id);

    vector<float> toFloats();

    void insertSelf(vector<float> * dest);
};


#endif //ODDITYENGINE_POINT_H
