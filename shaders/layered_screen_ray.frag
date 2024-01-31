#version 460 core

#include <object.frag>
#include <shape.frag>
#include <ray.frag>
#include <layered_buffer.frag>

in vec4 fragmentpos;

uniform float TIME;
uniform vec3 camera_pos;

uniform vec2 screen_size;
uniform uint layer_elements;

uniform float ratio;

uniform uint frame_clock;

layout(pixel_center_integer) in vec4 gl_FragCoord;
//in vec4 gl_FragCoord;

out vec4 color;

void main() {
    uint index = uint(((gl_FragCoord.x) / ratio * screen_size.y + (gl_FragCoord.y) / ratio));

    uint buffer_size = uint(screen_size.x * screen_size.y);

    if ((layer[index].frame_clock == frame_clock) && (layer[index].bounces > 0)) {
        vec3 uv = layer[index].uv;
        uint obj = layer[index].object;
        vec4 albedo = get_albedo(obj, uv);

        vec3 pos = layer[index].pos;

        gl_FragDepth = layer[index].depth;

        vec3 normal = layer[index].normal;
        vec3 dir = layer[index].dir;

        uint state = uint(TIME * pos_to_state(fragmentpos.xyz));

        Ray ray = mkray_hit(pos.xyz - dir.xyz * gl_FragDepth, dir.xyz, pos.xyz, normal.xyz, uv.xyz, obj, 1, 0);

        Ray reflect;
        Ray refract;

        color = ray_reflect(ray, reflect, refract);

        if (reflect.hit) {
            layer[index].pos = reflect.pos;
            layer[index].dir = reflect.dir;
            layer[index].normal = reflect.normal;
            layer[index].uv = reflect.uv;
//            layer[index].depth = reflect.len;
            layer[index].bounces = uint((bounces - 1) * (1 - get_shine(reflect.object, reflect.uv)));
            layer[index].object = reflect.object;
        }
    }
    else {
        color = vec4(0, 0, 0, 1);
    }
}