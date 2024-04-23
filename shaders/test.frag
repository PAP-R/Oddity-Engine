#version 450 core

in vec3 dir;

out vec4 color;

void main() {
    color = colorcreator();
}

vec4 colorcreator() {
    return vec4(dir, 1);
}