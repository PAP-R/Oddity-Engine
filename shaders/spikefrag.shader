#version 460 core

in vec3 fragmentPosition;

out vec3 color;

uniform float TIME;


const float lm = 10.;

const float tm = 10.;

const float r = 0.0;
const float rm = 0.0001;


float wave(float shift) {
    vec3 fp = fragmentPosition;
    float l = abs(fp.x) * abs(fp.y) * abs(fp.z);
    return sin(l * lm + tm * TIME + shift);
}


void main() {
    vec3 fp = fragmentPosition;
    float l = length(fp);

    color = vec3(1.);

    color *= smoothstep(r, r + rm, abs(fp.x) * abs(fp.y) * abs(fp.z)) * smoothstep(0.5, 0.51, wave(0));
}