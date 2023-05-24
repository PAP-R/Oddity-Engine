#version 460 core

const uint SPHERE = 0;

const uint MAX_STACK = 16;
const float FALLOFF = 0.0;

struct bufferobject {
    vec4 color;
    vec4 emission;
    vec4 pos;
    vec4 scale;
    uint vertexstart;
    uint vertexcount;
    uint type;
};

struct Ray {
    vec3 origin;
    vec3 dir;
    bool hit;
    vec3 pos;
    vec3 normal;
    float len;
    uint count;
    vec4 color;
    vec4 emission;
};

//struct Collision {
//    bool hit;
//    vec4 color;
//    vec4 emission;
//    vec3 pos;
//    vec3 dir_in;
//    vec3 dir_out;
//    vec3 normal;
//    uint count;
//    int object;
//};

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

Ray mkray(vec3 pos, vec3 dir, uint count) {
    Ray ray;
    ray.count = count;
    ray.origin = pos;
    ray.dir = normalize(dir);
    ray.len = 1. / 0.;
    ray.pos = vec3(ray.len);
    ray.hit = false;
    return ray;
}

Ray sphere_collision(Ray ray, vec3 pos, float radius) {
    vec3 origin = pos - ray.origin;
    float len = dot(ray.dir, pos);
    vec3 c = len * ray.dir;
    float dstc = distance(pos, c);
    float sign = -1;

    if (dstc <= radius) {
        float dst = sqrt(pow(radius, 2) - pow(dstc, 2));

        if (len <= dst) {
            ray.hit = false;
            if (-dst <= len && len < dst) {
                ray.hit = true;
                sign = 1;
            }
        }
        else {
            ray.hit = true;
        }

        ray.len = len + sign * dst;
        ray.pos = ray.origin + ray.dir * ray.len;

        ray.normal = -sign * normalize(ray.pos - pos);
    }

    return ray;
}

Ray collision_ray(Ray ray) {
    Ray result = ray;
    Ray temp = ray;
    uint index;

    for (uint i = 0; i < objects.length(); i++) {
        bufferobject object = objects[i];
        switch(object.type) {
            case SPHERE:
                temp = sphere_collision(ray, object.pos.xyz, object.scale.x);
        }
        if (temp.hit && temp.len < result.len) {
            result = temp;
            index = i;
        }
    }

    if (result.hit) {
        result.color = objects[index].color;
        result.emission = objects[index].emission;
    }

    return result;
}

vec4 collision_multi_ray(Ray ray, uint count) {
    vec4 color = vec4(1);
    vec4 emission = vec4(0);
    Ray current = ray;

    Ray stack[MAX_STACK];
    stack[0] = ray;
    int stack_index = 1;

    while (stack_index > 0) {
        current = stack[stack_index - 1];
        stack_index = (stack_index - 1);

        //        if (color.x == 0 && color.y == 0 && color.z == 0) {
        //            break;
        //        }

        current = collision_ray(current);

        if (current.hit) {
            //            color *= vec4(current.normal, 1);
            //            color *= vec4(current.pos, 1);
            //            color *= vec4(abs(current.pos), 1);
            emission += vec4(current.emission.xyz * current.emission.w * color.xyz * current.color.w, current.emission.w);
            color *= current.color;
            if (current.count < count) {
                stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), current.count + 1);
                stack_index = (stack_index + 1);

                if (current.color.w < 1) {
                    stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.01), current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }
        }
    }

    return emission;
}

void main() {
    color = collision_multi_ray(mkray(vec3(0), fragmentpos, 0), bounces);
}