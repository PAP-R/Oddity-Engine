#version 460 core

layout(location = 0) in vec3 vertexpos;

out vec4 fragmentpos;

uniform mat4 screen_projection;
uniform mat4 mvp;

void main() {
    gl_Position = screen_projection * vec4(vertexpos, 1.0);
    fragmentpos = mvp * vec4(vertexpos, 1.0);
//    fragmentpos = vec4(vertexpos, 1.0);
}
