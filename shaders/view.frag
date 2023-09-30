#version 460 core

in vec2 uv;

out vec4 color;

uniform sampler2D render_texture;
uniform vec2 view_size;

void main() {
    color = texture(render_texture, uv);
//    color = vec4(uv, 1, 1);

    vec2 pos = uv * view_size;
    int radius = 4;

    for (int x = -radius; x <= radius; x += 1) {
        for (int y = -radius; y <= radius; y += 1) {
            color += texture(render_texture, vec2((pos.x + x) / view_size.x, (pos.y + y) / view_size.y));
//            color += vec4((pos.x + x) / view_size.x, (pos.y + y) / view_size.y, 1, 1);
            color /= 2;
        }
    }
}
