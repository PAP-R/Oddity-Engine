#version 460 core

#include <buffer.frag>

struct bufferobject {
    uint transform;
    uint shape;
    uint material;
};

layout(std430, binding = OBJECT) buffer objectbuffer {
    bufferobject objects[];
};

layout(std430, binding = TRANSFORM) buffer transformbuffer {
    mat4 transforms[];
};

uint get_object_index(bufferobject object) {
    for (int i = 0; i < objects.length(); i++) {
        if (object == objects[i]) {
            return i;
        }
    }
    return objects.length();
}

mat4 get_transform(uint object) {
    return transforms[objects[object].transform];
}

vec3 transform2pos(mat4 transform) {
    return transform[3].xyz;
}

vec3 transform2scale(mat4 transform) {
    return vec3(length(transform[0].xyz), length(transform[1].xyz), length(transform[2].xyz));
}