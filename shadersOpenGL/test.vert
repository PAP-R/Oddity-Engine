#version 450 core

layout(location = 0) in vec3 vertexpos;

uniform mat4 screen_projection;

out vec3 dir;

void main() {
    gl_Position = screen_projection * vec4(vertexpos, 1.0);
    //    gl_Position = vertexpos;
    dir = normalize(vertexpos);
}