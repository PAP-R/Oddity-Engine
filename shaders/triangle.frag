#version 450

layout(location = 0) in vec3 fragDirection;
layout(location = 1) in vec2 fragScreenPos;
layout(location = 0) out vec4 outColor;


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec2 screenSize;
    vec2 mousePos;
    vec4 cameraPos;
    float time;
} ubo;

float f(float x) {
    return sin(x - ubo.time * 10);
}

float sdf(vec2 point) {
    vec2 pos = vec2(0, 0);
    float radius = 50;
    return distance(pos, point) - radius;
}

vec4 sphere_closest(vec3 point, vec3 pos, float radius) {
    vec3 diff = pos - point;
    vec3 dir = normalize(diff);
    float dist = length(diff) - radius;
    return vec4(dir * dist, dist);
}

vec4 sphere_closest_ray(vec3 origin, vec3 dir, float t, vec3 pos, float radius) {
    return sphere_closest(origin + dir * t, pos, radius);
}

vec4 line_closest(vec3 p, vec3 a, vec3 b, float radius) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0, 1);
    return vec4(0, 0, 0, length(pa - ba * h) - radius);
}

vec4 box_closest(vec3 p, vec3 a, vec3 b, float th) {
    float l = distance(b, a);
    vec3 d = (b - a) / l;
    vec3 q = (p - (a + b) / 2);
    q.xy = mat2(d.x, -d.y, d.y, d.x) * q.xy;
    q.xy = abs(q.xy) - vec2(1, th) / 2;
    return vec4(vec3(0), length(max(q.xy, 0)) + min(max(q.x, q.y), 0));
}

vec4 cross_closest(vec3 p, vec3 o, float th) {
    vec3 d = abs(o - p);
    return vec4(vec3(0), min(min(d.x, d.y), d.z) - th);
}

float positive_only(float x) {
    return (x + abs(x)) / 2;
}

float opSmooth(float d1, float d2, float k) {
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0, 1);
    return mix(d2, d1, h) - k * h * (1 - h);
}

void main() {
    vec3 actualMouse = vec3(ubo.mousePos - ubo.screenSize / 2, 0);
    outColor = vec4(fragDirection, 1);
//    outColor = vec4(fragDirection, 1.0) / (positive_only(sdf(fragScreenPos)) + 1) + vec4(fragDirection, 1.0) / (abs(f(fragScreenPos.x) - fragScreenPos.y));
    vec4 sphere_dist = sphere_closest(vec3(fragScreenPos, 0), vec3(0, 0, 0), 50);
//    outColor.xyz += fragDirection * positive_only(1 - length(sphere_dist.xyz));
//    outColor.xyz += vec3(fragDirection * (positive_only(1 - positive_only(sphere_dist.w))));
//    outColor.xyz += vec3(fragDirection * (positive_only(1 - positive_only(sphere_closest(vec3(fragScreenPos, 0), vec3(ubo.mousePos - ubo.screenSize / 2, 0), 20).w))));
//    outColor.xyz += vec3(fragDirection * (positive_only(1 - positive_only(line_closest(vec3(fragScreenPos, 0), vec3(ubo.mousePos - ubo.screenSize / 2, 0), vec3(fragScreenPos, 0) + sphere_dist.xyz, 5).w)))); //Weird thing
//    outColor.xyz += normalize(sphere_dist.xyz) * (positive_only(1 - positive_only(box_closest(vec3(fragScreenPos, 0), actualMouse, actualMouse + sphere_closest(actualMouse, vec3(0), 50).xyz, 5).w)));
//    outColor.xyz += normalize(sphere_dist.xyz) * (positive_only(1 - positive_only(line_closest(vec3(fragScreenPos, 0), actualMouse, actualMouse + sphere_closest(actualMouse, vec3(0), 50).xyz, 5).w)));

    float line = line_closest(vec3(fragScreenPos, 0), actualMouse, actualMouse + sphere_closest(actualMouse, vec3(0), 50).xyz, 5).w;
    float circle = sphere_closest(vec3(fragScreenPos, 0), vec3(0, 0, 0), 50).w;
    float cross = cross_closest(vec3(fragScreenPos, 0), actualMouse, 3).w;

    float intensity = positive_only(1 - positive_only(cross)) + positive_only(1 - positive_only(opSmooth(circle, line, 10))) + positive_only(1 - positive_only(opSmooth(circle, cross, 10)));

    outColor.xyz *= clamp(intensity, 0, 1);
}