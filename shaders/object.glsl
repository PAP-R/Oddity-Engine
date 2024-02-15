#version 450 core

#include <buffer.glsl>
#include <definitions.glsl>

const uint SHOW = 1 << 1;
const uint HIT = 1 << 2;

shared struct Object {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;

    vec4 angle;
    vec4 angle_velocity;
    vec4 angle_acceleration;

    float test_value[10];

    float radius;
    float mass;
    float restitution;
    uint state;
};

Object make_empty_object() {
    return Object(vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), float[10](0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0, 0, 0, 0);
}

layout(std140, std430, binding = OBJECT) buffer object_buffer {
    Object objects[];
};

vec3 closest(vec3 point, uint obj) {
    vec3 diff = objects[obj].position.xyz - point;
    return normalize(diff) * (length(diff) - objects[obj].radius);
}

vec3 closest_point(vec3 point, uint obj) {
    return point + closest(point, obj);
}

float sdf(vec3 point, uint obj) {
    return length(objects[obj].position.xyz - point) - objects[obj].radius;
}

vec4 trace(vec3 point, vec3 dir, uint max_steps, uint obj) {
    dir = normalize(dir);
    vec3 start_point = point;
    vec3 min_point = point;
    vec3 current_diff = closest(point, obj);
    vec3 min_diff = current_diff;
    float current_distance = length(current_diff);
    float min_distance = current_distance;

    for (uint i = 0; i < max_steps; i++) {
        point = point + dir * current_distance;
        current_diff = closest(point, obj);
        current_distance = length(current_diff);

        if (0 <= current_distance && current_distance < min_distance) {
            min_point = point;
            min_distance = current_distance;
            min_diff = current_diff;
            if (min_distance < TOLERANCE) {
                break;
            }
        }
    }

    return vec4(min_diff + min_point - start_point, min_distance);
}