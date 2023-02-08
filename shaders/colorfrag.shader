#version 330 core

in vec3 fragmentColor;
in vec3 fragmentPosition;
in vec2 UV;

out vec3 color;

uniform float TIME;

void main() {
    vec3 fp = fragmentPosition;
    vec2 q = UV - vec2(0.5f, 0.5f);
    float l = length(fp);

    float r = 1. + 0.1 * sin(atan(fp.x, fp.z) * 4 + TIME);

    color = vec3(fp.x, fp.y, fp.z);
    color = sin(color);
    color *= smoothstep(r, r + 0.01, l);
    //color = vec3(fragmentColor.r, fragmentColor.g, fragmentColor.b);
}