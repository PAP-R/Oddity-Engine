#version 460 core

layout(location = 0) in vec3 vertexpos;

out vec3 fragmentpos;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexpos, 1.0);
    fragmentpos = vertexpos;
}
