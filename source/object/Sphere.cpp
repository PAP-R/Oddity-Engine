#include "Sphere.h"
#include "fmt/format.h"
#include "source/base/tools/Debug.h"

Sphere::Sphere(const vec3 &pos, const vec3 &dir, const vec3 &scale, const std::string &vertexShader, const std::string &fragmentShader) : Object{pos, dir, scale}, Graphics{vertexShader, fragmentShader}, Physics(&(Object::pos), &(Object::scale)) {
    float r = 2.0f;

    vector<vec3> points;

    size_t pointCount = 5;

    for (size_t i = 0; i < pointCount; i++) {
        points.emplace_back(sin(i * 2 * pi<float>() / pointCount) * r, r, cos(i * 2 * pi<float>() / pointCount) * r);
        Debug::add_point(points.back(), fmt::format("{}", i));
    }

    add_face(points);
    subdivide(3);
    normalize();

    addData(3, getVertices(), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    addData(3, getIndices(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    set_size(get_index_count());
}
