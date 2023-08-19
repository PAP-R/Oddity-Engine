#version 460 core

layout(location = 0) in vec3 vertexpos;

out vec3 fragmentpos;

uniform mat4 screen_projection;
uniform mat4 render_projection;

void main() {
    gl_Position = screen_projection * vec4(vertexpos, 1.0);
    fragmentpos = (render_projection * vec4(vertexpos, 1.0)).xyz;
}
