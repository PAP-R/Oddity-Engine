#version 460 core

in vec3 fragmentpos;
in vec2 uv;

uniform float TIME;

out vec4 color;

void main() {
    color = vec4(uv, 0.0, 1.0);
}