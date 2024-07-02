#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec2 screenSize;
    vec2 mousePos;
    vec4 cameraPos;
    float time;
} ubo;


struct Object {
    vec4 pos;
    vec4 velocity;
    vec4 acceleration;
    vec4 size;
    vec4 color;
    uint type;
};

layout(std140, binding = 1) buffer ObjectSSBO {
    Object objects[];
};

void main() {
    uint self = gl_WorkGroupID.x;

    objects[self].velocity += objects[self].acceleration * ubo.deltaTime / 2;
    objects[self].pos += objects[self].velocity * ubo.deltaTime;
    objects[self].velocity += objects[self].acceleration * ubo.deltaTime / 2;
}