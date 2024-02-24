#version 450 core

#include <buffer.glsl>
#include <definitions.glsl>
#include <network.glsl>

//State
const uint SHOW = 1 << 1;
const uint HIT = 1 << 2;
const uint MOVE = 1 << 3;
const uint CLIP = 1 << 4;
const uint CONNECTED = 1 << 5;

//SHAPE
const uint SHAPE_SPHERE = 0;
const uint SHAPE_NETWORK = 1;

struct Object {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;

    vec4 angle;
    vec4 angle_velocity;
    vec4 angle_acceleration;

    float test_value[10];

    float radius;
    float mass;
    float restitution;
    uint state;

    uint shape;

    uint net_index;

    uint prev;
    uint next;
};

Object make_empty_object() {
    return Object(vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), float[10](0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0, 0, 0, 0, 0, 0, 0, 0);
}

layout(std140, std430, binding = OBJECT) buffer object_buffer {
    Object objects[];
};

vec4 closest_sphere(vec3 point, uint obj) {
    vec4 diff = vec4(objects[obj].position.xyz - point, 0);

    diff.xyz = normalize(diff.xyz) * (length(diff.xyz) - objects[obj].radius);

    diff.w = length(diff.xyz);

    if (distance(diff.xyz + point, objects[obj].position.xyz) > distance(point, objects[obj].position.xyz)) {
        diff.w *= -1;
    }

    return diff;
}

vec4 closest_network(vec3 point, uint obj) {
    float temp[MAX_THROUGHPUT];

    for (uint i = 0; i < 3; i++) {
        temp[i] = point[i];
        temp[i + 3] = objects[obj].position[i];
        temp[i + 6] = objects[obj].angle[i];
    }

    temp = apply(objects[obj].net_index, temp);

    return vec4(temp[0], temp[1], temp[2], temp[3]);
}

vec4 closest(vec3 point, uint obj) {
    switch (objects[obj].shape) {
        default:
            return closest_sphere(point, obj);
        case SHAPE_SPHERE:
            return closest_sphere(point, obj);
        case SHAPE_NETWORK:
            return closest_network(point, obj);
    }
}

vec4 closest_point(vec3 point, uint obj) {
    return closest(point, obj) + vec4(point, 0);
}

float sdf(vec3 point, uint obj) {
    return length(objects[obj].position.xyz - point) - objects[obj].radius;
}

vec4 multi_closest(vec3 point, float rounding, uint start, uint end) {
    uint count = end - start + 1;

    float min_dist = 1./0.;
    vec4 result;

    for (uint i = 0; i < count; i++) {
        vec4 temp = closest(point, start + i);

        if (temp.w < min_dist) {
            result = temp;
            min_dist = temp.w;
        }
    }

    return result;
}

vec4 multi_closest_point(vec3 point, float rounding, uint start, uint end) {
    return multi_closest(point, rounding, start, end) + vec4(point, 0);
}

vec4 multi_trace(vec3 point, vec3 dir, uint max_steps, uint start, uint end) {
    dir = normalize(dir);
    vec3 start_point = point;
    vec3 min_point = point;
    vec4 current_diff = multi_closest(point, 1, start, end);
    vec4 min_diff = current_diff;

    for (uint i = 0; i < max_steps; i++) {
        float angle = dot(dir, current_diff.xyz);
        if (angle > 0) {
            point = point + dir * angle;
        }
        else {
            point = point + dir * current_diff.w;
        }

        current_diff = multi_closest(point, 1, start, end);

        if (0 <= current_diff.w && current_diff.w < min_diff.w) {
            min_point = point;
            min_diff = current_diff;
            if (min_diff.w < TOLERANCE) {
                break;
            }
        }
    }

    return min_diff + vec4(min_point - start_point, 0);
}

vec4 trace(vec3 point, vec3 dir, uint max_steps, uint obj) {
    return multi_trace(point, dir, max_steps, obj, obj);
}

vec4 trace_point(vec3 point, vec3 dir, uint max_steps, uint obj) {
    return trace(point, dir, max_steps, obj) + vec4(point, 0);
}