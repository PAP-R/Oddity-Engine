#version 330 core

in vec3 fragmentColor;

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {
    color = clamp(fragmentColor * vec3(5.0), 0.0, 1.0);
}