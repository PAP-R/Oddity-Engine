#version 460 core
#extension GL_ARB_shader_viewport_layer_array : enable

layout(location = 0) in vec4 vertexpos;
layout(location = 1) in vec4 vertexnormal;
layout(location = 2) in vec4 vertexuv;


uniform mat4 mvp;
uniform mat4 model;
uniform int layer_in;

out vec4 view_pos;
out vec4 pos;
out vec4 normal;
out vec4 uv;

void main() {
    gl_Position = mvp * vertexpos;
    view_pos = mvp * vertexpos;
    pos = model * vertexpos;
    normal = model * vertexnormal;
    uv = vertexuv;
    gl_Layer = layer_in;
}
