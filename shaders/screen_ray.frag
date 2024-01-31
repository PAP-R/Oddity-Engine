#version 460 core

#include <ray.frag>

in vec4 fragmentpos;

uniform float TIME;
uniform vec3 camera_pos;

out vec4 color;

void main() {
    vec3 dir = normalize(fragmentpos.xyz);

    uint state = uint(TIME * pos_to_state(fragmentpos.xyz));
    color = collision_multi_ray(mkray(camera_pos, random_direction_ratio(dir, dir, state, 1, 1, 0.001), 1, 0), bounces, spread);
}
