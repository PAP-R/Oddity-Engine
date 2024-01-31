#version 460 core

uniform float TIME;

uint pos_to_state(vec3 pos) {
    return uint(pow((2 + pos.x) * 1000, 2)) * uint(pow((2 + pos.y) * 1000, 2)) * uint(pow((2 + pos.z) * 1000, 2)) + uint(TIME * 100);// + uint(pow((1 + TIME) * 1000, 2));
}

float random(inout uint state) {
    state = state * 747796405 + 2891336453;
    uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

float randomNormalDistribution(inout uint state) {
    float theta = 2 * 3.1415926 * random(state);
    float rho = sqrt(-2 * log(random(state)));
    return rho * cos(theta);
}

vec3 random_direction(inout uint state) {
    float x = randomNormalDistribution(state);
    float y = randomNormalDistribution(state);
    float z = randomNormalDistribution(state);
    return normalize(vec3(x, y, z));
}

vec3 hemisphere_direction(vec3 dir, vec3 normal) {
    return dir * sign(dot(normal, dir));
}

vec3 random_hemisphere_direction(vec3 normal, inout uint state) {
    vec3 dir = random_direction(state);
    return dir * sign(dot(normal, dir));
}

vec3 random_direction_ratio(vec3 direction, vec3 normal, inout uint state, float rdir, float rnormal, float rrandom) {
    return normalize(direction * rdir + normal * rnormal + random_hemisphere_direction(normal, state) * rrandom);
}