#version 460 core

layout(location = 0) in vec4 vertexpos;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vertexpos;
}
