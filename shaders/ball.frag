#version 450

#define PI 3.1415926535897932384626433832795

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

struct Ray {
    vec3 origin;
    vec3 dir;
    vec3 hitDir;
    bool hit;
};



float positive_only(float x) {
    return (x + abs(x)) / 2;
}

float negative_only(float x) {
    return -positive_only(x);
}




vec4 op_smooth(vec4 d1, vec4 d2, float k) {
    float h = clamp(0.5 + 0.5 * (d2.w - d1.w) / k, 0, 1);
    vec4 diff = mix(d2, d1, h);
    return diff - vec4(normalize(diff.xyz), 1) * k * h * (1 - h);
}

vec4 line_closest(vec3 p, vec3 a, vec3 b, float radius) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0, 1);
    return vec4(0, 0, 0, length(pa - ba * h) - radius);
}

vec4 sphere_closest(vec3 point, vec3 pos, float radius) {
    vec3 diff = pos - point;
    vec3 dir = normalize(diff);
    float dist = length(diff) - radius;
    return vec4(dir * dist, dist);
}

float ray_sphere_closest_length(vec3 p, vec3 d, vec3 o, float r) {
    float A = 1;
    float B = 2 * d.x * (p.x - o.x) + 2 * d.y * (p.y - o.y) + 2 * d.z * (p.z - o.z);
    float C = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y) + (p.z - o.z) * (p.z - o.z) - r * r;
    float thing = B * B - 4 * A * C;
    if (thing < 0) {
        return positive_only((d.x * (o.x - p.x) + d.y * (o.y - p.y) + d.z * (o.z - p.z)) / (d.x * d.x + d.y * d.y + d.z * d.z));
    }
    float resHitN = (-B - sqrt(thing)) / 2 * A;
    if (resHitN >= 0) {
        return resHitN;
    }
    float resHitP = (-B + sqrt(thing)) / 2 * A;
    if (resHitP >= 0) {
        return resHitP;
    }
    return 0;
}

Ray ray_sphere_closest(vec3 p, vec3 d, vec3 o, float r) {
    d = normalize(d);
    float t = positive_only(ray_sphere_closest_length(p, d, o, r));
    vec4 res = sphere_closest(p + d * t, o, r);
    return Ray(p, d * t, d * t + res.xyz, res.z <= 0);
}

float outer_clamp(float x, float minVal, float maxVal) {
    return x - clamp(x, minVal, maxVal) - sign(negative_only(x - minVal)) * minVal + sign(positive_only(x - maxVal)) * maxVal;
}

vec3 outer_clamp(vec3 x, vec3 minVal, vec3 maxVal) {
    return vec3(outer_clamp(x.x, minVal.x, maxVal.x), outer_clamp(x.y, minVal.y, maxVal.y), outer_clamp(x.z, minVal.z, maxVal.z));
}

float jump(float x, float minVal, float maxVal) {
    return (sign(abs(x - minVal) - abs(x - maxVal)) + 1) / 2 * minVal + (sign(abs(x - maxVal) - abs(x - minVal)) + 1) / 2 * minVal;
}

vec3 jump(vec3 x, vec3 minVal, vec3 maxVal) {
    return vec3(jump(x.x, minVal.x, maxVal.x), jump(x.y, minVal.y, maxVal.y), jump(x.z, minVal.z, maxVal.z));
}

vec4 cube_closest(vec3 p, vec3 o, vec3 s) {
    vec3 d = p - o;

    vec3 q = clamp(d, -s, s) - d;

    if (abs(d).x < s.x && abs(d).y < s.y && abs(d).z < s.z) {
        return vec4(q, -length(q));
    }

    return vec4(q, length(q));
}

void main() {
    float intensityMultiplier = 5;
    vec3 actualMouse = vec3(ubo.mousePos - ubo.screenSize / 2, 0);
    outColor = vec4(1);

    float radius = 100;
    float thickness = 2;

    vec3 intensity = vec3(0);

    for (float i = 0; i < 2 * PI && false; i += PI / 8) {
        Ray ray = ray_sphere_closest(actualMouse, vec3(cos(i + ubo.time / 10), sin(i + ubo.time / 10), 0), vec3(0), radius);
        intensity.x += positive_only(intensityMultiplier - positive_only(line_closest(vec3(fragScreenPos, 0), actualMouse, (ray.origin + ray.dir) * vec3(1, 1, 0), thickness).w)) / intensityMultiplier;
        if (length(ray.dir) != 0) {
            intensity.y += positive_only(intensityMultiplier - positive_only(line_closest(vec3(fragScreenPos, 0), (ray.origin + ray.dir) * vec3(1, 1, 0), (ray.origin + ray.hitDir) * vec3(1, 1, 0), thickness).w)) / intensityMultiplier;
            intensity.z += positive_only(intensityMultiplier - positive_only(line_closest(vec3(fragScreenPos, 0), actualMouse, (ray.origin + ray.hitDir) * vec3(1, 1, 0), thickness).w)) / intensityMultiplier;
        }
    }


    intensity.z += positive_only(intensityMultiplier - positive_only(line_closest(vec3(fragScreenPos, 0), actualMouse, actualMouse + cube_closest(actualMouse, vec3(0), vec3(radius)).xyz, thickness).w)) / intensityMultiplier;
//    intensity.z += positive_only(1 - positive_only(line_closest(vec3(fragScreenPos, 0), actualMouse, actualMouse + sphere_closest(actualMouse, vec3(0), radius).xyz, thickness).w));
    intensity.x += positive_only(intensityMultiplier - positive_only(cube_closest(vec3(fragScreenPos, 0), vec3(0), vec3(radius)).w)) / 2 / intensityMultiplier;

//    outColor.xyz *= 0.5 * clamp(intensity, 0, 1) + 0.5 * step(0.5, mod(intensity / 20, 1));
    outColor.xyz *= intensity;
}