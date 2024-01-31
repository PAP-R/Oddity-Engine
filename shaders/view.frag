#version 460 core

in vec2 uv;

uniform vec2 view_size;

out vec4 color;

layout(binding = 0) uniform sampler2DArray textures;
uniform uint texture_count;

layout(std140, std430, binding = 3) buffer texture_transform_buffer {
    vec4 texture_transform[];
};

layout(pixel_center_integer) in vec4 gl_FragCoord;

void main() {
    color = vec4(0);

    for (int i = 0; i < texture_count; i++) {
        vec2 pos = (gl_FragCoord.xy / view_size + texture_transform[i].xy) * texture_transform[i].zw;

        color += texture(textures, vec3(pos, i));
    }
}
