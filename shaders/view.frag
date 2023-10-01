#version 460 core

#define M_PI 3.1415926535897932384626433832795

in vec2 uv;

out vec4 color;

uniform sampler2D render_texture;
uniform vec2 view_size;
uniform float sample_size;

void main() {
    vec2 pos = view_size * uv;
    color = vec4(0);
//    color = vec4(uv, 1, 1);

    float min_angle = acos(1. - 1. / sample_size);

    for (float x = 1.; x <= sample_size; x++) {
        for (float y = 1.; y <= sample_size * sin(acos(x / sample_size)); y++) {
            color += texture(render_texture, vec2((pos.x + x) / view_size.x, (pos.y + y) / view_size.y));
            color += texture(render_texture, vec2((pos.x + x) / view_size.x, (pos.y - y) / view_size.y));
            color += texture(render_texture, vec2((pos.x - x) / view_size.x, (pos.y + y) / view_size.y));
            color += texture(render_texture, vec2((pos.x - x) / view_size.x, (pos.y - y) / view_size.y));
        }
    }

//        color += texture(render_texture, vec2((pos.x) / view_size.x, (pos.y + i) / view_size.y)); }

    color /= pow(sample_size, 2) * M_PI + 1;
//    color /= sample_size * M_PI + 1;

    color += texture(render_texture, pos / view_size);
}
