#version 330 core

in vec3 fragmentPosition;

out vec3 color;

uniform float TIME;

void main() {
    vec3 fp = fragmentPosition;
    float l = length(fp);

    float tm = 10;
    float swirl = 10;

    float r = 1. + 1. * sign(sin(atan(fp.x, fp.z) * 10 + tm * TIME + swirl * fp.y));

    color = vec3(1., 1., 1.);
    color = sin(color);
    color *= smoothstep(r, r + 0.01, l);
}