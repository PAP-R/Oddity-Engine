#!/usr/bin/sh

glslc ./shaders/triangle.vert -o "./[output]/shaders/vert.spv"
glslc ./shaders/triangle.frag -o "./[output]/shaders/frag.spv"
glslc ./shaders/ball.frag -o "./[output]/shaders/ball.spv"