#version 460 core

#include <random.frag>
#include <ray.frag>
#include <layered_buffer.frag>

in vec3 dir;
in vec3 pos;
in vec3 center;
in float point_radius;

uniform float TIME;
uniform uint object;
uniform vec3 camera_pos;
uniform vec2 screen_size;
uniform uint layer_elements;
uniform uint bounces;
uniform vec3 camera_dir;

uniform uint frame_clock;

out vec4 color;

vec3 f (vec3 pos_in) {
    return vec3(pos_in.x, sin(pos_in.x), pos_in.z);
}

void main() {
    uint state = uint(TIME * pos_to_state(dir));
    vec3 rdir = random_direction_ratio(normalize(dir), vec3(0), state, 1, 0, 0.001);

    float camera_distance = distance(pos, camera_pos);

    uint index = uint(gl_FragCoord.x * screen_size.y + gl_FragCoord.y);

    if ((0 >= layer[index].depth) || (layer[index].depth >= gl_FragCoord.z * (camera_distance - point_radius) / camera_distance) || (layer[index].frame_clock != frame_clock)) {
        Ray ray = mkray(camera_pos.xyz, rdir.xyz, 1, 0);

        Ray sphere_ray = sphere_collision(ray, object);

        if (sphere_ray.hit && sphere_ray.len >= 0) {
            gl_FragDepth = gl_FragCoord.z * sphere_ray.len / distance(pos, camera_pos);
            layer[index].frame_clock = frame_clock;
            layer[index].pos = sphere_ray.pos;
            layer[index].dir = sphere_ray.dir;
            layer[index].normal = sphere_ray.normal;
            layer[index].uv = sphere_ray.uv;
            layer[index].depth = gl_FragDepth;
            layer[index].bounces = uint((bounces - 1) * (1 - get_shine(sphere_ray.object, sphere_ray.uv)));
            layer[index].object = sphere_ray.object;

            color = get_emission(object, sphere_ray.uv.xyz);
        }
        else {
            gl_FragDepth = 1./0.;
//            color = vec4(normalize(normalize(rdir.xyz) - camera_dir), 1);
        }

//        if (distance(dir, f(dir)) < 1) {
//            color = vec4(1);
//        }
//        else {
//            color = vec4(0, 0, 0, 1);
//        }
//        color = vec4(1, 0, 1, 1);
    }
}
