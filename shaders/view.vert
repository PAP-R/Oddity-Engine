#version 460 core

layout(location = 0) in vec3 vertexpos;

out vec2 uv;

uniform mat4 screen_projection;

void main() {
    gl_Position = screen_projection * vec4(vertexpos, 1.0);
    uv = (vertexpos.xy + vec2(1, 1)) / 2.0;
}
