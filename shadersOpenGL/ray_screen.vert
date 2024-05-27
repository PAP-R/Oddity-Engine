#version 450 core

layout(location = 0) in vec3 vertex_pos;

out vec3 ray_dir;

uniform mat4 screen_projection;
uniform mat4 render_vp;
uniform mat4 mvp;

void main() {
    gl_Position = screen_projection * vec4(vertex_pos, 1);

    ray_dir = (render_vp * vec4(vertex_pos, 1)).xyz;
}