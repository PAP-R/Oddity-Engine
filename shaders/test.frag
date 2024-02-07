#version 450 core

in vec3 dir;

out vec4 color;

void main() {
    color = vec4(dir, 1);
}
