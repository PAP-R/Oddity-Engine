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
const uint ERROR = 1 << 6;

//SHAPE
const uint SHAPE_SPHERE = 0;
const uint SHAPE_NETWORK = 1;
const uint SHAPE_PLANE = 2;
const uint SHAPE_CUBE = 3;

struct Object {
    vec4 position;
    vec4 velocity;
    vec4 acceleration;

    vec4 angle;
    vec4 angle_velocity;
    vec4 angle_acceleration;

    vec4 orientation;

    mat4 transform;
    mat4 inverse_transform;

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

Object default_object() {
    return Object(vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(0), vec4(1, 0, 0, 0), mat4(1), mat4(1), float[10](0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0, 0, 0, 0, 0, 0, 0, 0);
}

layout(std140, std430, binding = OBJECT) buffer object_buffer {
    Object objects[];
};

uint object_count() {
    return objects.length();
}

struct trace_result {
    vec3 position;
    vec3 diff;

    float distance;

    uint obj;

    vec3 normal;
    vec3 uv;
};

trace_result closest_sphere(vec3 point, uint obj) {
    vec3 relative = (objects[obj].inverse_transform * vec4(point - objects[obj].position.xyz, 1)).xyz;

    trace_result result;

    result.diff = - relative;
//    result.diff = objects[obj].position.xyz - point;

    result.diff = (objects[obj].transform * vec4(normalize(result.diff) * (length(result.diff) - objects[obj].radius), 1)).xyz;
//    result.position = (objects[obj].transform * vec4(result.diff + relative, 1)).xyz;
    result.position = result.diff + point;

    result.distance = length(result.diff);

    if (distance(result.position, objects[obj].position.xyz) > distance(point, objects[obj].position.xyz)) {
        result.distance *= -1;
    }

    result.normal = normalize(-result.diff);

    result.obj = obj;

    return result;
}

vec4 closest_network(vec3 point, uint obj) {
    point = (objects[obj].inverse_transform * vec4(point, 1)).xyz;
    float temp[MAX_THROUGHPUT];

    for (uint i = 0; i < 3; i++) {
        temp[i] = point[i];
        temp[i + 3] = objects[obj].position[i];
        temp[i + 6] = objects[obj].angle[i];
    }

    temp = apply(objects[obj].net_index, temp);

    return vec4(temp[0], temp[1], temp[2], temp[3]);
}

trace_result closest_cube(vec3 point, uint obj) {
    trace_result result;

    vec3 relative = (objects[obj].inverse_transform * vec4(point - objects[obj].position.xyz, 1)).xyz;
//    vec3 relative = point - objects[obj].position.xyz;
    vec3 edge = vec3(objects[obj].radius);

    vec3 clamped = clamp(relative, -edge, edge);

    result.diff = (objects[obj].transform * vec4(clamped - relative, 1)).xyz;
//    result.position = (objects[obj].transform * vec4(result.diff + relative, 1)).xyz;
    result.position = result.diff + point;

    result.distance = length(result.diff);

    if (abs(relative.x) < edge.x && abs(relative.y) < edge.y && abs(relative.z) < edge.z) {
        result.distance *= -1;
    }

    result.normal = normalize(-result.diff);

    result.obj = obj;

    return result;
}

trace_result closest(vec3 point, uint obj) {
    switch (objects[obj].shape) {
        default:
            return closest_sphere(point, obj);
        case SHAPE_SPHERE:
            return closest_sphere(point, obj);
        case SHAPE_CUBE:
            return closest_cube(point, obj);
    }
}

trace_result multi_closest(vec3 point, float rounding, uint start, uint end) {
    uint count = end - start + 1;

    float min_dist = 1./0.;
    trace_result result;

    for (uint i = 0; i < count; i++) {
        trace_result temp = closest(point, start + i);

        if (abs(temp.distance) < min_dist) {
            result = temp;
            min_dist = abs(temp.distance);
        }
    }

    return result;
}