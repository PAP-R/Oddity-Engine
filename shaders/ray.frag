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

vec4 sphere_collision(vec3 pos, float radius, vec3 raypos, vec3 ray) ;

Collision ray(vec3 position, vec3 direction) ;

vec4 multiray(vec3 pos, vec3 dir, uint count, uint spread) ;

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
    vec3 rayn = normalize(ray);
    vec3 posn = pos - raypos;
    float len = dot(rayn, posn);
    if (len <= 0) {
        return vec4(0);
    }
    vec3 closest = len * rayn;
    float discp = distance(closest, posn);
    if (discp <= radius) {
        float diff = sqrt((radius * radius) - (discp * discp));
        if (diff >= len) {
            return vec4(closest + (diff * rayn), 1);
        }
        return vec4(closest - (diff * rayn), 1);
    }
    else {
        return vec4(closest, 0);
    }
}

Collision ray(vec3 position, vec3 direction) {
    vec3 rayn = normalize(direction);
    vec4 result = vec4(1. / 0.), temp = vec4(0), color = vec4(0);
    int index = -1;
    for (int i = 0; i < objects.length(); i++) {
        switch (objects[i].type) {
            case SPHERE:
                temp = sphere_collision(objects[i].pos.xyz, objects[i].scale.x, position, rayn);
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

    if (index > -1) {
        col.hit = true;
        col.normal = normalize(result.xyz - objects[index].pos.xyz);
        col.dir_in = rayn;
        col.dir_out = reflect(rayn, col.normal);
    }
    else {
        col.hit = false;
        col.dir_in = vec3(0);
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
//            for (int i = 0; i < spread; i++) {
//                stack[stack_in] = ray(current.pos, current.dir_out);
//                if (stack[stack_in].object > -1) {
//                    stack[stack_in].count = current.count + 1;
//                    stack_in = (stack_in + 1) % MAX_STACK;
//                }
//            }
            temp = ray(current.pos, current.dir_out);
            if (temp.hit) {
                stack[stack_in] = temp;
                stack[stack_in].count = current.count + 1;
                stack_in = (stack_in + 1) % MAX_STACK;
            }

            if (current.color.w < 1) {
                temp = ray(current.pos, refract(current.dir_in, current.normal, 1.1));
                if (temp.hit) {
                    stack[stack_in] = temp;
                    stack[stack_in].count = temp.count + 1;
                    stack_in = (stack_in + 1) % MAX_STACK;
                }
            }
        }
        if (current.hit) {
            color *= current.color;//vec4(abs(current.color.xyz) * current.color.w, current.color.w);
            emission += vec4(current.emission.xyz * current.emission.w * color.xyz, current.emission.w);
        }
        stack_out = (stack_out + 1) % MAX_STACK;
    }

    return emission;

    Collision result;
    vec4 tempcolor = vec4(0);
    vec3 nextpos = pos, nextdir = dir;
    for (int i = 0; i < count; i++) {
        result = ray(nextpos, nextdir);
        if (result.object == -1) {
            return color;
        }
        color += result.color * result.color.w / (1 + distance(result.pos.xyz, nextpos) * FALLOFF);
        nextpos = result.pos.xyz;
        if (result.color.w < 1) {
            vec3 temppos = nextpos, tempdir = nextdir;
            Collision tempresult;
            tempresult = ray(nextpos, nextdir);
            color += tempresult.color * tempresult.color.w * (1 - result.color.w) / (1 + distance(tempresult.pos.xyz, nextpos) * FALLOFF);
        }
        nextdir = reflect(nextdir, normalize(result.pos.xyz - objects[result.object].pos.xyz));
    }
    return color;
}

void main() {
    color = multiray(vec3(0), fragmentpos, bounces, 1, 1.0);
}
