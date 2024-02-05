#version 450 core

in vec3 pos;

out vec4 color;

void main() {
    color = vec4(abs(pos), 1.0);
}
