#version 450 core

in vec2 uv;

uniform vec2 view_size;

out vec4 color;

layout(binding = 0) uniform sampler2DArray textures;
uniform uint texture_count;

struct Render_transform {
    vec2 scale;
    vec2 pos;
};

layout(std140, std430, binding = 3) buffer texture_transform_buffer {
    Render_transform texture_transform[];
};

layout(pixel_center_integer) in vec4 gl_FragCoord;

void main() {
    color = vec4(0);

    for (int i = 0; i < texture_count; i++) {
        vec2 pos = uv * texture_transform[i].scale - texture_transform[i].pos;

        if (0 <= pos.x && pos.x <= 1 && 0 <= pos.y && pos.y <= 1) {
            color += texture(textures, vec3(pos, i));
        }
    }
}
