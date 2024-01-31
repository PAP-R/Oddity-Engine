#version 460 core

#include <object.frag>
#include <shape.frag>
#include <material.frag>
#include <ray.frag>
#include <layered_buffer.frag>

in vec3 dir;
in vec3 pos;
in vec3 normal;
in vec3 uv;

uniform uint object;
uniform vec3 camera_pos;
uniform vec2 screen_size;
uniform uint layer_elements;
uniform uint bounces;

uniform uint frame_clock;

out vec4 color;

layout(pixel_center_integer) in vec4 gl_FragCoord;
//in vec4 gl_FragCoord;

void main() {
    uint state = uint(TIME * pos_to_state(pos.xyz));

    vec3 dir = normalize(pos.xyz - camera_pos);
    vec3 rdir = random_direction_ratio(dir, vec3(0), state, 1, 0, 0.001);

    uint index = uint(gl_FragCoord.x * screen_size.y + gl_FragCoord.y);

    if ((0 >= layer[index].depth) || (layer[index].depth >= gl_FragCoord.z) || (layer[index].frame_clock != frame_clock)) {
        if (length(get_albedo(object, uv.xyz).xyz) > 0) {
            layer[index].normal = normalize(gl_FrontFacing ? normal.xyz : -normal.xyz);
//            layer[index].normal = normal.xyz;

            layer[index].dir = dir;
            layer[index].uv = uv.xyz;
            layer[index].bounces = uint((bounces - 1) * max(1 - get_shine(object, uv.xyz), 0));
        }
        else {
            layer[index].bounces = 0;
        }
        layer[index].color = get_emission(object, uv.xyz);
        layer[index].pos = pos.xyz;
        layer[index].depth = gl_FragCoord.z;
        layer[index].object = object;
        layer[index].frame_clock = frame_clock;

        color = get_emission(object, uv.xyz);

        layer[index].frag_coord = gl_FragCoord;
    }


}