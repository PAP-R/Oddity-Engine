#version 450

layout(location = 0) in vec3 fragDirection;
layout(location = 1) in vec2 fragScreenPos;
layout(location = 0) out vec4 outColor;


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec2 screenSize;
    float time;
} ubo;

float f(float x) {
    return sin(x - ubo.time * 10);
}

float sdf(vec2 point) {
    vec2 pos = vec2(0, 0);
    float radius = 50;
    return distance(pos, point) - radius;
}

float positive_only(float x) {
    return (x + abs(x)) / 2;
}

void main() {
    outColor = vec4(fragDirection, 1.0) / (positive_only(sdf(fragScreenPos)) + 1) + vec4(fragDirection, 1.0) / (abs(f(fragScreenPos.x) - fragScreenPos.y));
}