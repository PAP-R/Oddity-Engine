#version 460 core

const uint SPHERE = 0;
const uint MESH = 1;

const uint MAX_STACK = 8;
const float FALLOFF = 0.0;
const float tolerance = 1E-5;

struct bufferobject {
    mat4 transform;
    float radius;
    uint type;
    uint material;
    uint vertexstart;
    uint vertexcount;
};

struct buffermaterial {
    vec4 color;
    vec4 emission;
    float roughness;
};

struct buffervertex {
    vec4 pos;
    uint material;
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
    float roughness;
    float checkcount;
};

layout(std140, std430, binding = 3) buffer objectbuffer {
    bufferobject objects[];
};

layout(std140, std430, binding = 4) buffer materialbuffer {
    buffermaterial materials[];
};

layout(std140, std430, binding = 5) buffer vertexbuffer {
    buffervertex vertices[];
};

in vec3 fragmentpos;

out vec4 color;

uniform float TIME;
uniform uint bounces;
uniform uint spread;
uniform mat4 render_projection;
uniform vec3 CAMERAPOS;

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

Ray sphere_collision(Ray ray, mat4 transform) {
    vec3 pos = transform[3].xyz;
    float radius = length(transform[0].xyz);
    vec3 origin = pos;
    float len = dot(ray.dir, pos - ray.origin);
    vec3 c = len * ray.dir + ray.origin;
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

Ray triangle_collision(Ray ray, mat4 transform, buffervertex v0, buffervertex v1, buffervertex v2) {
    ray.hit = false;

    vec3 a = (transform * v0.pos).xyz;
    vec3 b = (transform * v1.pos).xyz;
    vec3 c = (transform * v2.pos).xyz;

    vec3 edgeab = b - a;
    vec3 edgeac = c - a;

    vec3 normal = normalize(cross(edgeab, edgeac));

    float angle = dot(ray.dir, normal);

    if (abs(angle) <= tolerance) {
        return ray;
    }

//    normal = normal * signdir;
    float d = dot(normal, a);
    float t = (d - dot(normal, ray.origin)) / angle;

    vec3 hitpos = ray.origin + t * ray.dir;

    if (t > tolerance && dot(cross(b - a, hitpos - a), normal) >= 0 &&
        dot(cross(c - b, hitpos - b), normal) >= 0 &&
        dot(cross(a - c, hitpos - c), normal) >= 0) {
        ray.hit = true;
        ray.pos = hitpos;
        ray.len = t;
        ray.normal = normal;
    }

    return ray;
}

Ray mesh_collision(Ray ray, mat4 transform, uint vertexstart, uint vertexcount) {
    float vertexend = vertexstart + vertexcount - vertexcount % 3;
    Ray temp, result = ray;
    result.len = 1./0.;
    for (uint i = vertexstart; i < vertexend; i += 3) {
        temp = triangle_collision(ray, transform, vertices[i], vertices[i + 1], vertices[i + 2]);
        if (temp.hit && temp.len <= result.len) {
            result = temp;
        }
    }

    return result;
}

Ray collision_ray(Ray ray) {
    Ray result = ray;
    Ray temp = ray;
    uint index;

    for (uint i = 0; i < objects.length(); i++) {
        bufferobject object = objects[i];
        switch(object.type) {
            case SPHERE:
                temp = sphere_collision(ray, object.transform);
                break;
            case MESH:
                temp = mesh_collision(ray, object.transform, object.vertexstart, object.vertexcount);
                break;
        }
        if (temp.hit && temp.len < result.len) {
            result = temp;
            index = i;
        }
    }

    if (result.hit) {
        ray.color = vec4(1);//materials[objects[index].material].color;
        ray.emission = materials[objects[index].material].emission;
        ray.roughness = materials[objects[index].material].roughness;
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
//            color *= vec4(abs(current.normal), 1);
//            color *= vec4(current.pos, 1);
//            color *= vec4(abs(current.pos), 1);
            emission += current.color;
            emission += vec4(current.emission.xyz * current.emission.w * color.xyz, current.emission.w);
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
    color = collision_multi_ray(mkray(CAMERAPOS, fragmentpos, 0), bounces);
}