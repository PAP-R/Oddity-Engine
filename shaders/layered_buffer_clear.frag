#version 460 core

#include <layered_buffer.frag>

uniform vec2 screen_size;
uniform uint layer_elements;

layout(pixel_center_integer) in vec4 gl_FragCoord;

void main() {
    uint index = uint(((gl_FragCoord.x - 1) * screen_size.y + (gl_FragCoord.y - 1)) * layer_elements);
    for (int i = 0; i < layer_elements; i++) {
        layer[index + i] = vec4(0);
    }
}