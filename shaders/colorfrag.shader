#version 330 core

in vec3 fragmentColor;

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {
    color = (fragmentColor + 1.0f) / 2;
}