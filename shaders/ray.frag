#version 460 core

const uint SPHERE = 0;

const uint MAX_STACK = 16;
const float FALLOFF = 0.0;

struct bufferobject {
    vec4 color;
    vec4 emission;
    vec4 pos;
    vec4 scale;
    uint type;
};

struct Collision {
    bool hit;
    vec4 color;
    vec4 emission;
    vec3 pos;
    vec3 dir_in;
    vec3 dir_out;
    vec3 normal;
    uint count;
    int object;
};

layout(std140, std430, binding = 3) buffer objectbuffer {
    bufferobject objects[];
};

in vec3 fragmentpos;

out vec4 color;
uniform float TIME;
uniform uint bounces;
uniform mat4 MVP;

uint pos_to_state(vec3 pos) {
    return uint(pow((1 + pos.x) * 1000, 2)) * uint(pow((1 + pos.y) * 1000, 2)) * uint(pow((1 + pos.z) * 1000, 2));// + uint(pow((1 + TIME) * 1000, 2));
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

vec3 random_hemisphere_direction(vec3 normal, inout uint state) {
    vec3 dir = random_direction(state);
    return dir * sign(dot(normal, dir));
}

vec4 sphere_collision(vec3 pos, float radius, vec3 raypos, vec3 ray) {
    ray = normalize(ray);
    pos = pos - raypos;
    int sign = -1;
    if (length(pos) < radius) {
        sign = 1;
    }
    float len = dot(ray, pos);
    vec3 closest = len * ray;
    float discp = distance(closest, pos);
    if (discp <= radius) {
        float diff = sign * sqrt((radius * radius) - (discp * discp));
        if (len <= diff) {
            return vec4(closest, 0);
        }
        return vec4(closest + (diff * ray), 1);
    }
    else {
        return vec4(closest, 0);
    }
}

Collision ray(vec3 position, vec3 dir) {
    dir = normalize(dir);
    vec4 result = vec4(1. / 0.), temp = vec4(0), color = vec4(0);
    int index = -1;
    for (int i = 0; i < objects.length(); i++) {
        switch (objects[i].type) {
            case SPHERE:
                temp = sphere_collision(objects[i].pos.xyz, objects[i].scale.x, position, dir);
        }
        if (temp.w == 1 && distance(temp.xyz, position) < distance(result.xyz, position)) {
            result = temp;
            index = i;
        }
    }

    Collision col;
    col.color = objects[index].color;
    col.emission = objects[index].emission;
    col.pos = result.xyz;
    col.object = index;
    col.dir_in = dir;

    if (index > -1) {
        col.hit = true;
        col.normal = normalize(result.xyz - objects[index].pos.xyz);
//        col.normal = col.normal * sign(dot(col.normal, rayn));
        col.dir_out = reflect(dir, col.normal);
    }
    else {
        col.hit = false;
        col.dir_out = vec3(0);
    }

    return col;
}

vec4 multiray(vec3 pos, vec3 dir, uint count, uint spread, float spreadsize) {
    Collision start;
    start.hit = false;
    start.pos = pos;
    start.dir_out = dir;
    start.count = 0;

    start.color = vec4(1);

    start.normal = vec3(0, 0, 1);

    Collision stack[MAX_STACK];
    stack[0] = start;
    uint stack_in = 1;
    uint stack_out = 0;

    uint state = pos_to_state(pos);

    Collision current, temp;

    vec4 color = vec4(1);
    vec4 emission = vec4(0);

    while (stack_in != stack_out) {
        current = stack[stack_out];
        if (current.count < count) {
            if (current.color.w > 0) {
                temp = ray(current.pos, current.dir_out);
                if (temp.hit) {
                    stack[stack_in] = temp;
                    stack[stack_in].count = current.count + 1;
                    stack_in = (stack_in + 1) % MAX_STACK;
                }
            }

            if (current.color.w < 1) {
//                temp = ray(current.pos, current.dir_in);
                temp = ray(current.pos, refract(current.dir_in, current.normal, 1.01));
                if (temp.hit) {
                    stack[stack_in] = temp;
                    stack[stack_in].count = temp.count + 1;
                    stack_in = (stack_in + 1) % MAX_STACK;
                }
            }
        }
        if (current.hit) {
            emission += vec4(current.emission.xyz * current.emission.w * color.xyz, current.emission.w);
//            color *= vec4(current.normal, 1);
            color *= current.color;
        }
        stack_out = (stack_out + 1) % MAX_STACK;
    }

    return emission;
}

void main() {
    color = multiray(vec3(0), fragmentpos, bounces, 1, 1.0);
}
