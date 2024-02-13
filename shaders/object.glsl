#version 450 core

#include <buffer.glsl>

const uint SHOW = 1 << 1;
const uint HIT = 1 << 2;

struct Object {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;

    vec4 angle;
    vec4 angle_velocity;
    vec4 angle_acceleration;

    vec4 test_value;

    float mass;
    float restitution;
    uint state;
};

Object make_empty_object() {
    return Object(vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), 0, 0, 0);
}

layout(std140, std430, binding = OBJECT) buffer object_buffer {
    Object objects[];
};

vec3 closest(vec3 point, uint obj) {
    vec3 diff = objects[obj].position.xyz - point;
    return normalize(diff) * (length(diff) - objects[obj].test_value.x);
}

vec3 closest_point(vec3 point, uint obj) {
    return point + closest(point, obj);
}

float sdf(vec3 point, uint obj) {
    return length(objects[obj].position.xyz - point) - objects[obj].test_value.x;
}