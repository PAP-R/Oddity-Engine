#version 460 core

in vec3 fragmentpos;

uniform float TIME;

out vec4 color;

void main() {
    color = vec4(fragmentpos, 1.0);
}