#version 460 core

#include <buffer.frag>
#include <object.frag>

const uint SPHERE = 0;
const uint CUBE = 1;
const uint MESH = 2;
const uint MESH_FLAT = 3;
const uint SDF = 4;

struct buffershape {
    uint shape;
    uint vertex_start;
    uint vertex_size;
    uint normal_start;
    uint normal_size;
    uint uv_start;
    uint uv_size;
    vec4 bounding_box_center;
    vec4 bounding_box_size;
};

layout(std430, binding = SHAPE) buffer shapebuffer {
    buffershape shapes[];
};

layout(std430, binding = VERTEX) buffer vertexbuffer {
    vec4 vertices[];
};

layout(std430, binding = NORMAL) buffer normalbuffer {
    vec4 normals[];
};

layout(std430, binding = UV) buffer uvbuffer {
    vec4 uvs[];
};

buffershape get_shape(uint object) {
    return shapes[objects[object].shape];
}