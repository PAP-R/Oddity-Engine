#version 450 core

#include <buffer.glsl>

const uint NO_SHOW = 1 << 1;

struct Object {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;

    vec4 angle;
    vec4 angle_velocity;
    vec4 angle_acceleration;

    vec4 test_value;

    float mass;
    float drag;
    uint state;
};

layout(std140, std430, binding = PHYSICS) buffer object_buffer {
    Object objects[];
};