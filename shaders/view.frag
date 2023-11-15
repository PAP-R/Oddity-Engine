#version 460 core

in vec2 uv;

out vec4 color;

layout(binding = 0) uniform sampler2DArray textures;
uniform uint texture_count;

layout(std140, std430, binding = 3) buffer Rendererbuffer {
    vec4 texture_transform[];
};

void main() {
    color = vec4(0);
    for (int i = 0; i < texture_count; i++) {
        vec2 pos = (uv + texture_transform[i].xy) * texture_transform[i].zw * 0.1;

        color += texture(textures, vec3(pos, i));
    }
}
