#version 460 core

in vec3 fragmentPosition;

uniform vec3 GLIL;
uniform vec3 CAMERAPOS;

out vec4 color;

void main() {
    vec3 normal = normalize(fragmentPosition);

    float angleLight = acos(dot(normal, normalize(GLIL - fragmentPosition)));
    float angleCam = acos(dot(normal, normalize(CAMERAPOS - fragmentPosition)));

    float reflect = 1 - (abs(angleLight - angleCam) / radians(180));

    color = vec4(vec3(1.0) * reflect, 1.0);
}