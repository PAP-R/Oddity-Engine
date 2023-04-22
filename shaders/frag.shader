#version 460 core

in vec3 fragmentPosition;

out vec4 color;

void main() {
    color = vec4(abs(fragmentPosition), 1.0);
}