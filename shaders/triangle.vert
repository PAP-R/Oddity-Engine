#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inDirection;

layout(location = 0) out vec3 fragDirection;
layout(location = 1) out vec2 fragScreenPos;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec2 screenSize;
    vec2 mousePos;
    vec4 cameraPos;
    float time;
} ubo;

void main() {
//    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
//    gl_Position = ubo.proj * ubo.view * mat4(1) * vec4(inPosition, 0.0, 1.0);
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragDirection = normalize(ubo.proj * ubo.view * mat4(1) * vec4(inDirection, 1)).xyz;
    fragScreenPos = inPosition * ubo.screenSize / 2;
}