#version 460 core

const uint SPHERE = 0;
const uint MESH = 1;

const uint MAX_STACK = 8;
const float FALLOFF = 0.0;
const float tolerance = 1E-3;

struct bufferobject {
    vec4 color;
    vec4 emission;
    mat4 transform;
    float radius;
    float roughness;
    uint type;
    uint vertexstart;
    uint vertexcount;
};

struct buffervertex {
    vec4 pos;
    vec4 color;
    vec4 normal;
    vec2 uv;
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

layout(std140, std430, binding = 4) buffer verticexbuffer {
    buffervertex vertices[];
};

in vec3 fragmentpos;

out vec4 color;

uniform float TIME;
uniform uint bounces;
uniform uint spread;
uniform mat4 MVP;
uniform vec3 CAMERAPOS;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

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

vec3 random_hemisphere_direction(vec3 normal, inout uint state) {
    vec3 dir = random_direction(state);
    return dir * sign(dot(normal, dir));
}

vec3 random_direction_ratio(vec3 direction, vec3 normal, inout uint state, float rdir, float rnormal, float rrandom) {
    return normalize(direction * rdir + normal * rnormal + random_hemisphere_direction(direction, state) * rrandom);
}

Ray mkray(vec3 pos, vec3 dir, uint count) {
    Ray ray;
    ray.count = count;
    ray.origin = pos;
    ray.dir = normalize(dir);
    ray.len = 1. / 0.;
    ray.pos = vec3(ray.len);
    ray.hit = false;
    ray.checkcount = 0;
    return ray;
}

Ray sphere_collision(Ray ray, bufferobject object) {
    mat4 transform = object.transform;

    vec3 pos = transform[3].xyz;
    float radius = length(transform[0].xyz) * object.radius;

    vec3 poso = pos - ray.origin;

    float len = dot(poso, ray.dir);

    vec3 c = len * ray.dir;

    float dstc = distance(poso, c);

    if (dstc <= radius) {
        float dst = sqrt(pow(radius, 2) - pow(dstc, 2));

        if (len > -dst + tolerance) {
            ray.hit = true;
            float sign = 1;

            if (len > dst + tolerance) {
                sign = -1;
            }

            ray.len = len + sign * dst;
            ray.pos = ray.origin + ray.dir * ray.len;

            ray.normal = normalize(ray.pos - pos);

            ray.color = object.color;
            ray.emission = object.emission;
            ray.roughness = object.roughness;
        }
    }

    return ray;
}

Ray triangle_collision(Ray ray, bufferobject object, buffervertex v0, buffervertex v1, buffervertex v2) {
    ray.hit = false;

    vec3 a = (object.transform * v0.pos).xyz;
    vec3 b = (object.transform * v1.pos).xyz;
    vec3 c = (object.transform * v2.pos).xyz;

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

        ray.color = object.color;
        ray.emission = object.emission;
        ray.roughness = object.roughness;
    }

    return ray;
}

Ray mesh_collision(Ray ray, bufferobject object) {
    float vertexend = object.vertexstart + object.vertexcount - object.vertexcount % 3;
    Ray temp, result = ray;

    float checkcount = 0;

    lowp vec4 mid;
    lowp float a, b, c, radius, dist;
    lowp mat3x4 tri;
    for (uint i = object.vertexstart; i < vertexend; i += 3) {
         tri = object.transform * mat3x4(vertices[i].pos, vertices[i + 1].pos, vertices[i + 2].pos);
//        first = object.transform * vertices[i].pos;
//        second = object.transform * vertices[i + 1].pos;
//        third =  object.transform * vertices[i + 2].pos;
//        mid = (first + second + third) / 3;
        mid = (tri[0] + tri[1] + tri[2]) / 3;
        a = distance(mid, tri[0]);
        b = distance(mid, tri[1]);
        c = distance(mid, tri[2]);
        radius = a > b ? (a > c ? a : c) : (b > c ? b : c);
        dist = distance(ray.origin, mid.xyz);
        if (dist - radius <= result.len && sqrt(pow(dist, 2) - pow(dot(mid.xyz - ray.origin, ray.dir), 2)) <= radius) {
            checkcount++;
            temp = triangle_collision(ray, object, vertices[i], vertices[i + 1], vertices[i + 2]);
            if (temp.hit && temp.len <= result.len) {
                result = temp;
            }
        }
    }

    result.checkcount = checkcount;

    return result;
}

Ray collision_ray(Ray ray) {
    Ray result = ray;
    Ray temp = ray;

    float checkcount = 0;

    for (uint i = 0; i < objects.length(); i++) {
        bufferobject object = objects[i];
        if (distance(ray.origin, object.transform[3].xyz) - object.radius < result.len) {
            switch(object.type) {
                case SPHERE:
                    temp = sphere_collision(ray, object);
                    checkcount++;
                    break;
                case MESH:
                    temp = mesh_collision(ray, object);
                    checkcount += temp.checkcount;
                    break;
            }
            if (temp.hit && temp.len < result.len) {
                result = temp;
                ray.len = result.len;
            }
        }
    }

    result.checkcount = checkcount;

    return result;
}

vec4 collision_multi_ray(Ray ray, uint count, uint spread) {
    uint state = pos_to_state(ray.dir);
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

//            Tests

//            emission = vec4(vec3(current.len > 0 ? 1 : 0), 1);
//            color *= vec4(current.normal, 1);
//            color *= vec4(abs(current.normal), 1);
//            color *= vec4(current.pos, 1);
//            color *= vec4(abs(current.pos), 1);
//            color *= vec4(vec3(current.len / 100), 1);
//            color *= vec4(hsv2rgb(vec3(mod(current.checkcount / 4.1, vertices.length()), 1, 1)), 1);
//            color *= vec4(hsv2rgb(vec3(mod(current.checkcount / 4.1, 1), 1, 1)), 1);
//            color *= vec4(hsv2rgb(vec3(mod(current.checkcount / vertices.length(), 1), 1, 1)), 1);
//            color *= vec4(vec3(current.checkcount / vertices.length()), 1);


//            emission += color;
            emission += vec4(current.emission.xyz * current.emission.w * color.xyz, current.emission.w);
            color *= current.color;

            if (current.count < count) {
                if (current.roughness > 0) {
                    for (uint i = current.count; i < spread; i++) {
                        stack[stack_index] = mkray(current.pos, random_direction_ratio(reflect(current.dir, current.normal), current.normal, state, 1. - current.roughness, 0, current.roughness), current.count + 1);
                        stack_index = (stack_index + 1);
                    }
                }
                else {
                    stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), current.count + 1);
                    stack_index = (stack_index + 1);
                }

                if (current.color.w < 1) {
                    stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.001), current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }
        }
    }

    return emission;
}

void main() {
    color = collision_multi_ray(mkray(CAMERAPOS, fragmentpos, 0), bounces, spread);
}