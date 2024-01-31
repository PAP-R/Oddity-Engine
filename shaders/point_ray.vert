#version 460 core

#include <object.frag>

layout(location = 0) in vec4 in_vertexpos;
layout(location = 1) in vec4 vertexnormal;
layout(location = 2) in vec4 vertexuv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 normal;
out vec3 uv;
out vec3 world_pos;

out float radius;

void main() {
    gl_Position = mvp * vec4(in_vertexpos.xyz, 1);
    world_pos = (model * vec4(in_vertexpos.xyz, 1)).xyz;
    radius = (transform2scale(model) * in_vertexpos.w).x * 2;
    normal = normalize((model * vertexnormal).xyz);
    uv = vertexuv.xyz;
}
