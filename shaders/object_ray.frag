#version 460 core

#include <object.frag>
#include <shape.frag>
#include <material.frag>
#include <ray.frag>

in vec3 dir;
in vec3 pos;
in vec3 normal;
in vec3 uv;

uniform uint object;
uniform vec3 camera_pos;

out vec4 color;

void main() {
    bufferobject obj = objects[object];
//    color = vec4(1, 0, 0, 1);
//    color = vec4(uv.xy, 1, 1);
//    color = get_emission(objects[object].material, uv.xyz);
//    color = fragmentpos;
//    color = normal;

//    vec3 actual_normal = normalize(cross(dFdx(vec3(view_pos)), dFdy(vec3(view_pos))));

    uint state = uint(TIME * pos_to_state(pos.xyz));

    vec3 dir = normalize(vec3(pos) - camera_pos);
    vec3 rdir = random_direction_ratio(dir, vec3(0), state, 1, 0, 0.001);

//    color = vec4(rdir, 1);
    color = collision_multi_ray_continue(mkray_hit(camera_pos, rdir, vec3(pos), hemisphere_direction(vec3(normal), dir), vec3(uv), object, 1, 0), bounces, spread);
}