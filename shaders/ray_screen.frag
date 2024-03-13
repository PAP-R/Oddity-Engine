#version 450 core

#include <object_trace.glsl>
#include <definitions.glsl>

in vec3 ray_dir;

uniform vec3 camera_pos;
uniform float time;

out vec4 color;

vec3 hsv2rgb(float h, float s, float v) {
    vec3 c = vec3(h, s, v);
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    trace_result result = multi_trace_hit(camera_pos, ray_dir, 25, 0, object_count() - 1);

    if (result.distance < TOLERANCE) {
        color = vec4(hsv2rgb(float(result.obj) / object_count(), 1, 1), 1);
    }
    else {
        color = vec4(0);
    }

    if ((objects[result.obj].state & ERROR) != 0) {
        color += vec4(sin(TIME), 0, 0, 1);
    }
}