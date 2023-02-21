#include "Sphere.h"
#include "fmt/format.h"
#include "source/base/tools/Debug.h"

Sphere::Sphere(const vec3 &pos, const vec3 &dir, const vec3 &scale, const std::string &vertexShader, const std::string &fragmentShader) : Object{pos, dir, scale}, Graphics{vertexShader, fragmentShader}, Physics(&(Object::pos), &(Object::scale)) {
    float r = 2.0f;

    vec3 middleUp(0, r, 0);
    vec3 middleDown(0, -r, 0);

    size_t pointCount = 4;

    vector<vec3> pointsUp;

    for (size_t i = 0; i < pointCount; i++) {
        pointsUp.emplace_back(sin(i * 2 * pi<float>() / pointCount) * r, 0, cos(i * 2 * pi<float>() / pointCount) * r);
        Debug::add_point(pointsUp.back(), fmt::format("{}", i));
    }

    add_face(pointsUp, middleUp);

    vector<vec3> pointsDown;

    for (size_t i = pointCount; i > 0; i--) {
        pointsDown.emplace_back(sin(i * 2 * pi<float>() / pointCount) * r, 0, cos(i * 2 * pi<float>() / pointCount) * r);
        Debug::add_point(pointsDown.back(), fmt::format("{}", i));
    }

    add_face(pointsDown, middleDown);

    for (size_t i = 0; i < 4; i++) {
        subdivide();
        normalize();
    }


    addData(3, getVertices(), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    addData(3, getIndices(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    set_size(get_index_count());
}
