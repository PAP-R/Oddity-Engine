#version 450 core

layout(location = 0) in vec3 vertexpos;

uniform mat4 screen_projection;

out vec2 uv;

void main() {
    gl_Position = screen_projection * vec4(vertexpos, 1.0);
    //    gl_Position = vertexpos;
    uv = vertexpos.xy;
}