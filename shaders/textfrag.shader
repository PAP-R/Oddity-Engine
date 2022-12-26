#version 330 core

in vec2 UV;
//in vec3 fragmentColor;

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {
    color = texture(myTextureSampler, UV).rgb;
    //color = clamp(fragmentColor * vec3(5.0), 0.0, 1.0);
}