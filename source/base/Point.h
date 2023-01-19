#ifndef ODDITYENGINE_POINT_H
#define ODDITYENGINE_POINT_H

#include <glm/glm.hpp>

using namespace glm;

#include <vector>
#include <map>

using namespace std;

class Point {
public:
    vec3 pos{};
    size_t id;

    vector<tuple<Point*, size_t>> cons;

    Point();

    explicit Point(const vec3 &pos);

    Point(const vec3 &pos, size_t id);

    vector<float> toFloats();

    void insertSelf(vector<float> * dest);
};


#endif //ODDITYENGINE_POINT_H
