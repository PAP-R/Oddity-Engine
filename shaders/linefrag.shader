#version 460 core

in vec3 fragmentPosition;

out vec3 color;

uniform float TIME;


const float lm = 10.;

const float tm = 10.;

const float r = 0.8;
const float rm = 0.01;


float wave(float shift) {
    vec3 fp = fragmentPosition;
    float l = abs(fp.x) * abs(fp.y) * abs(fp.z);
    return sin(l * lm + tm * TIME + shift);
}


void main() {
    vec3 fp = fragmentPosition;
    float l = length(fp);

    color = vec3(1.);

    color *= step(r, abs(fp.x * fp.y)) + step(r, abs(fp.y * fp.z)) + step(r, abs(fp.z * fp.x));
}