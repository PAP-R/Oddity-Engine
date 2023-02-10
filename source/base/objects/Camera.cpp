#include "Camera.h"

vec3 Camera::direction() const {
    return {
            cos(angle.y) * sin(angle.x),
            sin(angle.y),
            cos(angle.y) * cos(angle.x)
    };
}

vec3 Camera::right() const {
    return {
            sin(angle.x - 3.14f / 2.0f),
            0,
            cos(angle.x - 3.14f / 2.0f)
    };
}

vec3 Camera::up() const {
    return cross(right(), direction());
}

Camera::Camera(const vec3 &position, const vec2 &angle, float fov) : position(position), angle(angle), fov(fov) {}
