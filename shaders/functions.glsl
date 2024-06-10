float sign(float x) {
    return x / abs(x);
}

float abs(float x) {
    return sqrt(x * x);
}

float positive_only(float x) {
    return (x + abs(x)) / 2;
}

float negative_only(float x) {
    return -positive_only(x);
}

float clamp(float x, float min, float max) {
    return x + negative_only(x + min) - min - positive_only(x + max);
}

vec4 sphere_closest(vec3 point, vec3 pos, float radius) {
    vec3 diff = pos - point;
    vec3 dir = normalize(diff);
    float dist = length(diff) - radius;
    return vec4(dir * dist, dist);
}

float ray_sphere_closest_length(vec3 p, vec3 d, vec3 o, float r) {
    return (- d.x * (p.x - o.x) - d.y * (p.y - o.y) - d.z * (p.z - o.z)) / (d.x + d.y + d.x);
}

vec4 ray_sphere_closest(vec3 p, vec3 d, vec3 o, float r) {
    float t = ray_sphere_closest_length(p, d, o, r);
    vec3 dist = t * dir;
    vec4 res = sphere_closest(dist, o, r);
    return vec4(dist + res.xyz, length(dist) + res.z);
}