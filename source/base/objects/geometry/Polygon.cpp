#include "Polygon.h"

#include <algorithm>
#include <iterator>

Polygon::Polygon() = default;

Polygon::Polygon(const vector<vec3> &points) : points(points) {}

size_t Polygon::add_point(vec3 point) {
    auto iterator = std::find(points.begin(), points.end(), point);
    if (iterator == points.end()) {
        points.emplace_back(point);
        return points.size() - 1;
    }
    return iterator - points.begin();
}

void Polygon::add_face(const vector<vec3> &points, vec3 middle) {
    for (size_t i = 0; i < points.size(); i++) {
        add_triangle(points[i], points[(i + 1) % points.size()], middle);
    }
}

void Polygon::add_face(const vector<vec3> &points) {
    vec3 middle = get_middle(points);
    add_face(points, middle);
}

void Polygon::add_triangle(size_t first, size_t second, size_t third) {
    triangles.emplace_back(first, second, third);
}

void Polygon::add_triangle(vec3 first, vec3 second, vec3 third) {
    add_triangle(add_point(first), add_point(second), add_point(third));
}

void Polygon::remove_triangle(Triangle *triangle) {
    triangles.erase(std::remove(triangles.begin(), triangles.end(), *triangle), triangles.end());
}

vec3 Polygon::get_middle(const vector<vec3> &points) {
    vec3 middle(0);

    for (size_t i = 0; i < points.size(); i++) {
        middle += points[i];
    }

    return middle / vec3(points.size());
}

vec3 Polygon::get_middle(Triangle *triangle){
    return get_middle({points[triangle->x], points[triangle->y], points[triangle->z]});
}

void Polygon::subdivide(Triangle *triangle) {
    vec<3, vec3> inner;
    for (size_t i = 0; i < 3; i++) {
        inner[i] = (points[(*triangle)[i]] + points[(*triangle)[(i + 1) % 3]]) / vec3(2);
    }

    add_triangle(inner.x, inner.y, inner.z);

    for (size_t i = 0; i < 3; i++) {
        add_triangle(points[(*triangle)[i]], inner[i], inner[(i + 2) % 3]);
    }
    remove_triangle(triangle);
}

void Polygon::subdivide() {
    vector<Triangle> copy;
    std::copy(triangles.begin(), triangles.end(), back_inserter(copy));

    for (auto triangle : copy) {
        subdivide(&triangle);
    }
}

void Polygon::subdivide(size_t steps) {
    for (size_t i = 0; i < steps; i++) {
        subdivide();
    }
}

void Polygon::clear() {
    points.clear();
    triangles.clear();
}

void Polygon::normalize() {
    for (size_t i = 0; i < points.size(); i++) {
        points[i] = ::normalize(points[i]);
    }
}