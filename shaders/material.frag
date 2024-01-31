#version 460 core

#include <buffer.frag>

struct buffermaterial {
    uint albedo;
    uint emission;
    uint normal_shine;
};

layout(std430, binding = MATERIAL) buffer materialbuffer {
    buffermaterial materials[];
};

layout(std430, binding = TEXTURE_TRANSFORM) buffer texture_transform_buffer {
    vec2 texture_transform[];
};

layout(binding = 0) uniform sampler2DArray textures;

buffermaterial get_material(uint object) {
    return materials[objects[object].material];
}

vec4 get_from_texture(uint index, vec3 uv) {
//    return texture(textures, (vec4(uv.x, uv.y, uv.z + index, 1)).xyz);
    return texture(textures, vec3(texture_transform[index] * uv.xy, uv.z + index));
}

vec4 get_albedo(uint object, vec3 uv) {
    return get_from_texture(get_material(object).albedo, uv);
}

vec4 get_emission(uint object, vec3 uv) {
    return get_from_texture(get_material(object).emission, uv);
}

vec3 get_normal(uint object, vec3 uv) {
    return get_from_texture(get_material(object).normal_shine, uv).xyz;
}

float get_shine(uint object, vec3 uv) {
    return get_from_texture(get_material(object).normal_shine, uv).w;
}