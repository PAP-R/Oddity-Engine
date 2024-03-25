#version 450 core

#include <buffer.glsl>

#define CONSTRAINT_GRAVITY 0
#define CONSTRAINT_KEEP_IN 1
#define CONSTRAINT_KEEP_OUT 2

struct Constraint {
    float offset;
    uint type;
    uint first;
    uint second;
};

layout(std140, std430, binding = CONSTRAINT) buffer constraing_buffer {
    Constraint constraints[];
};

