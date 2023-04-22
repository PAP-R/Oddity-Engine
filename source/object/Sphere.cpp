#include "Sphere.h"
#include "fmt/format.h"
#include "source/base/tools/Debug.h"

#include <glm/gtx/vector_angle.hpp>

#include <algorithm>

Sphere::Sphere(const vec3 &pos, const vec3 &dir, const vec3 &scale, const std::string &vertexShader, const std::string &fragmentShader) : Object{pos, dir, scale}, Graphics{vertexShader, fragmentShader}, Physics(&(Object::pos), &(Object::scale)) {
    setShape(PHYSICS_SHAPE_SPHERE);

    float r = 2.0f;

    vec3 middleUp(0, 2, 0);
    vec3 middleDown(0, -r, 0);

    size_t pointCount = 3;

    for (size_t i = 0; i < 3; i++) {
        vec3 point(0);
        point[i] = scale[i];
        Debug::add_point(point, fmt::format("{:.0f}", scale[i]));
    }

    vector<vec3> pointsUp;
    vector<vec3> pointsDown;

    for (size_t i = 0; i < pointCount; i++) {
        pointsUp.emplace_back(sqrt(3) * sin(i * 2 * pi<float>() / pointCount), -1, sqrt(3) * cos(i * 2 * pi<float>() / pointCount));
        pointsDown.emplace_back(pointsUp.back());
        Debug::add_point(pointsUp.back(), fmt::format("{}", i));
    }

    reverse(pointsDown.begin(), pointsDown.end());

    add_face(pointsUp, middleUp);
    add_face(pointsDown);




    for (size_t i = pointCount; false && i > 0; i--) {
        pointsDown.emplace_back(sin(i * 2 * pi<float>() / pointCount) * r, -1, cos(i * 2 * pi<float>() / pointCount) * r);
        Debug::add_point(pointsDown.back(), fmt::format("{}", i));
    }

    subdivide(5);
    normalize();

//    for (size_t i = 0; i < 5; i++) {
//        normalize();
//    }


    addData(3, getVertices(), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    addData(3, getIndices(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    set_size(get_index_count());
}
