#version 460 core

#include <buffer.frag>

layout(std430, binding = NETWORK) buffer networkbuffer {
    highp double networks[];
};