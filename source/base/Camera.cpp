#include "Camera.h"

vec3 Camera::direction() const {
    return {
        sin(angle.y),
        -sin(angle.x) * cos(angle.y),
        cos(angle.x) * cos(angle.y),
    };
}

vec3 Camera::right() const {
    return {
        cos(angle.y),
        sin(angle.x) * sin(angle.y),
        -cos(angle.x) * sin(angle.y)
    };
}

vec3 Camera::up() const {
    return cross(right(), direction());
}

Camera::Camera(vec3 position, vec3 angle, float fov) : position(position), angle(angle), fov(fov) {}

