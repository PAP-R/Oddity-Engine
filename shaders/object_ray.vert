#version 460 core

layout(location = 0) in vec4 vertexpos;
layout(location = 1) in vec4 vertexnormal;
layout(location = 2) in vec4 vertexuv;


uniform mat4 mvp;
uniform mat4 model;

out vec3 dir;
out vec3 pos;
out vec3 normal;
out vec3 uv;

void main() {
    gl_Position = mvp * vertexpos;
    dir = gl_Position.xyz;
    pos = (model * vertexpos).xyz;
    normal = normalize((model * vertexnormal).xyz);
    uv = (vertexuv).xyz;
}
