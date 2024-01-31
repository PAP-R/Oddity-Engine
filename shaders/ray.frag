#version 460 core

#include <object.frag>
#include <shape.frag>
#include <material.frag>
#include <random.frag>
#include <network.frag>

const uint MAX_STACK = 16;
const float FALLOFF = 0.0;
const float tolerance = 1E-3;

struct Ray {
    bool hit;
    vec3 origin;
    vec3 dir;
    vec3 pos;
    vec3 normal;
    vec3 uv;
    float len;
    float weight;
    uint count;
    uint object;
};

uniform uint bounces;
uniform uint spread;

uniform float cull;
uniform float split;


vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 expand(vec3 vec) {
    return vec4(vec, 1);
}

Ray mkray(vec3 pos, vec3 dir, float weight, uint count) {
    Ray ray;
    ray.count = count;
    ray.origin = pos;
    ray.dir = normalize(dir);
    ray.len = 1. / 0.;
    ray.pos = vec3(ray.len);
    ray.uv = vec3(0);
    ray.weight = weight;
    ray.hit = false;
    return ray;
}

Ray mkray_hit(vec3 origin, vec3 dir, vec3 pos, vec3 normal, vec3 uv, uint object, float weight, uint count) {
    Ray ray;
    ray.count = count;
    ray.origin = origin;
    ray.dir = normalize(dir);
    ray.len = length(pos - origin);
    ray.pos = pos;
    ray.normal = normalize(normal);
    ray.uv = uv;
    ray.weight = weight;
    ray.hit = true;
    ray.object = object;
    return ray;
}

Ray sphere_collision(Ray ray, uint object) {
    mat4 transform = get_transform(object);

    vec3 pos = transform2pos(transform);
    float radius = length(transform2scale(transform));

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

            ray.object = object;
        }
    }

    return ray;
}

float sphere_simple_collision(Ray ray, uint object) {
    mat4 transform = get_transform(object);

    vec3 pos = transform2pos(transform);
    float radius = length(transform2scale(transform));

    vec3 poso = pos - ray.origin;

    float len = dot(poso, ray.dir);

    vec3 c = len * ray.dir;

    float dstc = distance(poso, c);

    if (dstc <= radius) {
        return len - radius;
    }
    else {
        return 1./0.;
    }
}

Ray box_collision(Ray ray, uint object) {
    mat4 transform = get_transform(object);

    vec3 pos = transform2pos(transform);
    float radius = transform2scale(transform).x;

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

            ray.object = object;
        }
    }

    return ray;
}

Ray triangle_collision(Ray ray, uint object, uint v0, uint v1, uint v2) {
    ray.hit = false;

    mat4 transform = get_transform(object);

    vec3 a = (transform * vertices[v0]).xyz;
    vec3 b = (transform * vertices[v1]).xyz;
    vec3 c = (transform * vertices[v2]).xyz;

    vec3 edgeab = b - a;
    vec3 edgeac = c - a;

    vec3 normal = cross(edgeab, edgeac);

    float area = length(normal);

    float angle = dot(ray.dir, normal);

    if (abs(angle) <= tolerance) {
        return ray;
    }

//    normal = normal * signdir;
    float d = dot(normal, a);
//    float t = -(dot(normal, ray.origin) + d) / angle;
    float t = (d - dot(normal, ray.origin)) / angle;
    if (t <= tolerance) return ray;

    vec3 p = ray.origin + t * ray.dir;

    vec3 edgeouter = b - a;
    vec3 edgeinner = p - a;

    vec3 perpen = cross(edgeouter, edgeinner);

    float w = dot(perpen, normal);
    if (w < 0) return ray;

    edgeouter = c - b;
    edgeinner = p - b;

    perpen = cross(edgeouter, edgeinner);

    float u = dot(perpen, normal);
    if (u < 0) return ray;

    edgeouter = a - c;
    edgeinner = p - c;

    perpen = cross(edgeouter, edgeinner);

    float v = dot(perpen, normal);
    if (v < 0) return ray;

    ray.hit = true;
    ray.pos = p;
    ray.len = t;

    if (get_shape(object).shape == MESH_FLAT) {
        ray.normal = normalize(normal);
    }
    else {
        ray.normal = normalize(u * normals[v0].xyz + v * normals[v1].xyz + w * normals[v2].xyz);
    }

    ray.uv = u * uvs[v0].xyz + v * uvs[v1].xyz + w * uvs[v2].xyz;

    ray.object = object;

    return ray;
}

Ray mesh_collision(Ray ray, uint object) {
    buffershape shape = get_shape(object);
    float vertexend = shape.vertex_start + shape.vertex_size - (shape.vertex_size % 3);
    Ray temp, result = ray;

    for (uint i = shape.vertex_start; i < vertexend; i += 3) {
        temp = triangle_collision(ray, object, i, i + 1, i + 2);
        if (temp.hit && temp.len <= result.len) {
            result = temp;
        }
    }

    if (result.hit) {
        ray.object = object;
    }

    return result;
}

Ray network_colision(Ray ray, uint object) {

    return ray;
}

Ray collision_ray(Ray ray) {
    Ray result = ray;
    Ray temp = ray;

    for (uint obj = 0; obj < objects.length(); obj++) {
        mat4 transform = get_transform(obj);
        if (true || distance(ray.origin, transform2pos(transform)) - length(transform2scale(transform)) < result.len) {
            switch(get_shape(obj).shape) {
                case SPHERE:
                    temp = sphere_collision(ray, obj);
                    break;
                case MESH:
                case MESH_FLAT:
                    float approximate_len = sphere_simple_collision(ray, obj);
                    if (tolerance < approximate_len && approximate_len < temp.len) {
                        temp = mesh_collision(ray, obj);
                        break;
                    }
            }
            if (temp.hit && tolerance < temp.len && temp.len < result.len) {
                result = temp;
            }
        }
    }

    return result;
}


vec4 ray_reflect(Ray ray, inout Ray out_reflect, inout Ray out_refract) {
    uint state = pos_to_state(ray.dir);
    vec4 emission = vec4(0);
    if (length(get_albedo(ray.object, ray.uv).xyz) > 0) {
        if (get_shine(ray.object, ray.uv) > 0) {
            for (uint i = 0; i < spread; i++) {
                out_reflect = collision_ray(mkray(ray.pos, random_direction_ratio(reflect(ray.dir, ray.normal), ray.normal, state, 1. - get_shine(ray.object, ray.uv), 0, get_shine(ray.object, ray.uv)), 1., ray.count + 1));
                if (out_reflect.hit) emission += vec4(get_emission(out_reflect.object, out_reflect.uv).xyz * get_emission(out_reflect.object, out_reflect.uv).w * get_albedo(ray.object, ray.uv).xyz / spread, get_emission(out_reflect.object, out_reflect.uv).w);
            }
        }
        else {
            out_reflect = collision_ray(mkray(ray.pos, reflect(ray.dir, ray.normal), 1, ray.count + 1));
            if (out_reflect.hit) emission += vec4(get_emission(out_reflect.object, out_reflect.uv).xyz * get_emission(out_reflect.object, out_reflect.uv).w * get_albedo(ray.object, ray.uv).xyz * out_reflect.weight, get_emission(out_reflect.object, out_reflect.uv).w);
        }
    }

    if (get_albedo(ray.object, ray.uv).w < 1) {
        out_refract = collision_ray(mkray(ray.pos, refract(ray.dir, ray.normal, 1.001), 1 - get_albedo(ray.object, ray.uv).w, ray.count + 1));
        if (out_refract.hit) emission += vec4(get_emission(out_refract.object, out_refract.uv).xyz * get_emission(out_refract.object, out_refract.uv).w * get_albedo(ray.object, ray.uv).xyz * out_refract.weight, get_emission(out_refract.object, out_refract.uv).w);
    }

    return emission;
}

vec4 collision_multi_ray(Ray ray, uint count, uint spread, inout float len) {
    uint state = pos_to_state(ray.dir);
    vec4 color = vec4(1);
    vec4 emission = vec4(0);
    Ray current = ray;

    Ray stack[MAX_STACK];
    stack[0] = ray;
    int stack_index = 1;

    stack_index -= 1;
    current = collision_ray(stack[stack_index]);

    if (current.hit) {
        len = current.len;

        emission += vec4(get_emission(current.object, current.uv).xyz * get_emission(current.object, current.uv).w * color.xyz * current.weight, get_emission(current.object, current.uv).w);

        color *= get_albedo(current.object, current.uv);

        if (emission.x >= 1 && emission.y >= 1 && emission.z >= 1) {
            return emission;
        }

        if (current.count < count) {
            if (length(get_albedo(current.object, current.uv).xyz) > 0) {
                if (get_shine(current.object, current.uv) > 0) {
                    for (uint i = 0; i < spread; i++) {
                        stack[stack_index] = mkray(current.pos, random_direction_ratio(reflect(current.dir, current.normal), current.normal, state, 1. - get_shine(current.object, current.uv), 0, get_shine(current.object, current.uv)), 1., current.count + 1);
                        stack_index = (stack_index + 1);
                    }
                }
                else {
                    stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), 1, current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }

            if (get_albedo(current.object, current.uv).w < 1) {
                stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.001), 1 - get_albedo(current.object, current.uv).w, current.count + 1);
                stack_index = (stack_index + 1);
            }
        }
    }

    while (stack_index > 0) {
        stack_index -= 1;
        current = collision_ray(stack[stack_index]);

        if (current.hit) {
            emission += vec4(get_emission(current.object, current.uv).xyz * get_emission(current.object, current.uv).w * color.xyz * current.weight, get_emission(current.object, current.uv).w);

            color *= get_albedo(current.object, current.uv);

            if (current.count < count) {
                if (length(get_albedo(current.object, current.uv).xyz) > 0) {
                    if (get_shine(current.object, current.uv) > 0) {
                        for (uint i = 0; i < spread; i++) {
                            stack[stack_index] = mkray(current.pos, random_direction_ratio(reflect(current.dir, current.normal), current.normal, state, 1. - get_shine(current.object, current.uv), 0, get_shine(current.object, current.uv)), 1., current.count + 1);
                            stack_index = (stack_index + 1);
                        }
                    }
                    else {
                        stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), 1, current.count + 1);
                        stack_index = (stack_index + 1);
                    }
                }

                if (get_albedo(current.object, current.uv).w < 1) {
                    stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.001), 1 - get_albedo(current.object, current.uv).w, current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }
        }
    }

    return emission;
}

vec4 collision_multi_ray_continue(Ray ray, uint count, uint spread) {
    uint state = pos_to_state(ray.dir);
    vec4 color = vec4(1);
    vec4 emission = vec4(0);
    Ray current = ray;

    Ray stack[MAX_STACK];
    stack[0] = ray;
    int stack_index = 1;

    emission += vec4(get_emission(current.object, current.uv).xyz * get_emission(current.object, current.uv).w * color.xyz * current.weight, get_emission(current.object, current.uv).w);

    color *= get_albedo(current.object, current.uv);

    if (current.count < count) {
        if (length(get_albedo(current.object, current.uv).xyz) > 0) {
            if (get_shine(current.object, current.uv) > 0) {
                for (uint i = 0; i < spread; i++) {
                    stack[stack_index] = mkray(current.pos, random_direction_ratio(reflect(current.dir, current.normal), current.normal, state, 1. - get_shine(current.object, current.uv), 0, get_shine(current.object, current.uv)), 1., current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }
            else {
                stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), 1, current.count + 1);
                stack_index = (stack_index + 1);
            }
        }

        if (get_albedo(current.object, current.uv).w < 1) {
            stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.001), 1 - get_albedo(current.object, current.uv).w, current.count + 1);
            stack_index = (stack_index + 1);
        }
    }

    while (stack_index > 0) {
        stack_index -= 1;
        current = collision_ray(stack[stack_index]);

        if (current.hit) {
            emission += vec4(get_emission(current.object, current.uv).xyz * get_emission(current.object, current.uv).w * color.xyz * current.weight, get_emission(current.object, current.uv).w);

            color *= get_albedo(current.object, current.uv);

            if (current.count < count) {
                if (length(get_albedo(current.object, current.uv).xyz) > 0) {
                    if (get_shine(current.object, current.uv) > 0) {
                        for (uint i = 0; i < spread; i++) {
                            stack[stack_index] = mkray(current.pos, random_direction_ratio(reflect(current.dir, current.normal), current.normal, state, 1. - get_shine(current.object, current.uv), 0, get_shine(current.object, current.uv)), 1., current.count + 1);
                            stack_index = (stack_index + 1);
                        }
                    }
                    else {
                        stack[stack_index] = mkray(current.pos, reflect(current.dir, current.normal), 1, current.count + 1);
                        stack_index = (stack_index + 1);
                    }
                }

                if (get_albedo(current.object, current.uv).w < 1) {
                    stack[stack_index] = mkray(current.pos, refract(current.dir, current.normal, 1.001), 1 - get_albedo(current.object, current.uv).w, current.count + 1);
                    stack_index = (stack_index + 1);
                }
            }
        }
    }

    return emission;
}