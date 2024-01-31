#version 460 core

#include <random.frag>

layout(points) in;
layout(triangle_strip, max_vertices = 128) out;

in float radius[];
in vec3 world_pos[];

out vec3 dir;
out vec3 pos;
out vec3 wpos;
out vec3 center;
out float point_radius;

uniform mat4 projection;
uniform mat4 mvp;
uniform mat4 ray_mvp;
uniform mat4 view;
uniform mat4 vp;

uniform vec3 camera_pos;
uniform vec3 camera_dir;

void main() {
    float dir_ratio = 2.1;

    for (int i = 0; i < gl_in.length(); i++) {
        uint state = uint(TIME * pos_to_state(world_pos[i]));
        vec3 dir_diff = random_direction_ratio(normalize(world_pos[i] - camera_pos), vec3(0), state, 1, 0, 0.1) - camera_dir;
        vec3 right = normalize(cross(camera_dir, dir_diff)) * radius[i];
        vec3 up = normalize(cross(camera_dir, right)) * radius[i] * max(1.1, length(dir_diff) * dir_ratio);
//        up = vec3(0, radius[i], 0);

        point_radius = radius[i];

        for (int j = 0, a = 1, b = 1; j < 4; j++) {
            pos = world_pos[i] + dir_diff / 2 + a * right + b * up;
            gl_Position.xyz = (vp * vec4(pos, 1)).xyz;
            gl_Position.w = gl_in[i].gl_Position.w;
            //        gl_Position = vec4((projection * vec4(from_camera.xyz + up + right, 1)).xyz, from_camera.w);
            //        gl_Position = projection * vec4(gl_Position.xyz, 1) + vec4(0, 0, 0, gl_Position.w);
            //        dir = ((ray_mvp * (vec4(gl_in[i].gl_Position.xyz + up + right, 1))).xyz);
//            pos = world_pos[i] + a * right + b * up;
//            pos = gl_in[i].gl_Position.xyz;
            dir = pos - camera_pos;
//            dir = pos;

            EmitVertex();

            a *= b;
            b *= -1;
        }

        EndPrimitive();
    }
}