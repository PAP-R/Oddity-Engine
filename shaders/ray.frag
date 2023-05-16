#version 460 core

const uint SPHERE = 0;

const uint MAX_STACK = 16;
const float FALLOFF = 0.01;

struct bufferobject {
    vec4 color;
    vec4 pos;
    vec4 scale;
    uint type;
};

struct Collision {
    vec4 color;
    vec3 pos;
    vec3 dir_in;
    vec3 dir_out;
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
        return vec4(closest - (diff * rayn), 1);
    }
    else {
        return vec4(closest, 0);
    }
}

Collision ray(vec3 position, vec3 direction) {
    vec3 ray = normalize(direction);
    vec4 result = vec4(1. / 0.), temp = vec4(0), color = vec4(0);
    int index = -1;
    for (int i = 0; i < objects.length(); i++) {
        switch (objects[i].type) {
            case SPHERE:
                temp = sphere_collision(objects[i].pos.xyz, objects[i].scale.x, position, ray);}
                if (temp.w == 1 && distance(temp.xyz, position) < distance(result.xyz, position)) {
                    result = temp;
                    color = objects[i].color;
                    index = i;
                }
        }
    Collision col;
    col.color = color;
    col.pos = result.xyz;
    col.object = index;
    col.dir_in = direction;
    col.dir_out = reflect(direction, normalize(result.xyz - objects[index].pos.xyz));
    return col;
}

vec4 multiray(vec3 pos, vec3 dir, uint count, uint spread, float spreadsize) {
    Collision start;
    start.color = vec4(0, 0, 0, 0);
    start.pos = pos;
    start.dir_out = dir;
    start.count = 0;
    Collision stack[MAX_STACK];
    stack[0] = start;
    uint stack_in = 1;
    uint stack_out = 0;

    Collision temp;

    vec4 color = vec4(0);

    while (stack_in != stack_out) {
        temp = stack[stack_out];
        if (temp.count < count) {
            stack[stack_in] = ray(temp.pos, temp.dir_out);
            stack[stack_in].count = temp.count + 1;
            stack_in = (stack_in + 1) % MAX_STACK;
            if (temp.color.w < 1) {
                stack[stack_in] = ray(temp.pos, temp.dir_in);
                stack[stack_in].count = temp.count + 1;
                stack_in = (stack_in + 1) % MAX_STACK;
            }
        }
        color += vec4(temp.color.xyz * temp.color.w, temp.color.w);
        stack_out = (stack_out + 1) % MAX_STACK;
    }

    return color;

    Collision result;
    vec4 color = vec4(0);
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
