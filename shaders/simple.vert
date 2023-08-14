#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec3 fragmentPosition;

uniform mat4 screen_projection;

void main() {
    gl_Position = screen_projection * vec4(vertexPosition_modelspace, 1);
    fragmentPosition = vertexPosition_modelspace;
}