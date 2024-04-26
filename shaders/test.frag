#version 450 core

in vec3 dir;

out vec4 color;

uniform uint function;

void main() {
    color = colorselector(dir);
}

vec4 colorselector:colorcreator(vec3 col) {
    return vec4(col, 1);
}

#selector(function) vec4 colorselector(vec3 col)