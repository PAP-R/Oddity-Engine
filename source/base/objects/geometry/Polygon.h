#ifndef ODDITYENGINE_POLYGON_H
#define ODDITYENGINE_POLYGON_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
using namespace glm;

#include <vector>
using namespace std;

typedef vec<3, size_t> Triangle;

class Polygon {
protected:

    vector<vec3> points;
    vector<Triangle> triangles;

public:
    Polygon();

    explicit Polygon(const vector<vec3> &points);

    void add_triangle(size_t first, size_t second, size_t third);

    void subdivide(Triangle *triangle);

    vec3 get_middle(Triangle *triangle);

    size_t add_point(vec3 point);

    void add_triangle(vec3 first, vec3 second, vec3 third);

    void add_face(const vector<vec3> &points);

    void remove_triangle(Triangle *triangle);

    void subdivide();

    void subdivide(size_t steps);

    void clear();

    static vec3 get_middle(const vector<vec3> &points);

    void normalize();
};


#endif //ODDITYENGINE_POLYGON_H
