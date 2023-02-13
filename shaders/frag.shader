#version 460 core

in vec3 fragmentPosition;

out vec3 color;

uniform float TIME;


const float lm = 10.;

const float tm = -10.;

float wave(float shift) {
    vec3 fp = fragmentPosition;
    float l = length(fp);
    return sin(l * lm + tm * TIME + shift);
}

void main() {
    vec3 fp = fragmentPosition;
    float l = length(fp);
    float swirl = 10;

    float r = 0.3;//1. + abs(10 + fp.x * fp.y * fp.z) * sign(sin(atan(fp.x, fp.z) * 10 + tm * TIME + swirl * fp.y));

    float t = wave(0.);

    color = vec3(wave(0), wave(0), wave(0));
    color = sin(color);
    color *= smoothstep(r, r + 0.01, color);
    r += 1;
    color *= smoothstep(r + 5, r - 0.01, color);
}