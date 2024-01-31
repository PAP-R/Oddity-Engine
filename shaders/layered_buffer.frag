#version 460 core

#include <buffer.frag>

struct layered_buffer_object {
    vec3 pos;
    vec3 dir;
    vec3 normal;
    vec3 uv;
    float depth;
    uint object;
    uint bounces;
    uint frame_clock;
    vec4 frag_coord;
    vec4 color;
};

layout(std430, binding = LAYER) buffer layer_buffer {
    layered_buffer_object layer[];
};