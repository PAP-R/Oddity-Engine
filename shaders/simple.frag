#version 460 core

in vec3 fragmentPosition;

uniform float TIME;

out vec4 color;

void main() {
    color = vec4(sin(TIME), 0, cos(TIME), 1.0);
}