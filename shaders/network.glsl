#version 460 core

#include <buffer.glsl>

const uint MAX_THROUGHPUT = 10;

layout(std430, binding = NETWORK) buffer networkbuffer {
    float nets[];
};

float function(uint func, float x, float y) {
    switch(func) {
        case 0:
            return -x;
        case 1:
            return 1 / x;
        case 2:
            return sqrt(x);
        case 3:
            return x * x;
        case 4:
            return x * x * x;
        case 5:
            return sin(x);
        case 6:
            return cos(x);
        case 7:
            return tanh(x);
        case 8:
            return 1 / (1 + exp(x));
        case 9:
            return x * y;
    }
    return x;
}

float[MAX_THROUGHPUT] apply(uint net_start, float[MAX_THROUGHPUT] net_input) {
    float throughput[MAX_THROUGHPUT];

    uint layer_count = uint(nets[net_start + 1]);

    uint layer_start = net_start + 2;

    for (uint l = 0; l < layer_count; l++) {
        uint weight_start = layer_start + 2;
        uint bias_start = weight_start + uint(nets[weight_start]);
        uint function_start = bias_start + uint(nets[bias_start]);

        uint weight_layer_start = weight_start + 2;
        
        for (uint x = 0; x < nets[weight_start + 1]; x++) {
            for (uint y = 0; y < nets[weight_layer_start + 1]; y++) {
                throughput[x] = nets[weight_layer_start + 2 + y] * net_input[y];
            }
            weight_layer_start = weight_layer_start + uint(nets[weight_layer_start]);
        }

        net_input = throughput;

        for (uint x = 0; x < nets[bias_start + 1]; x++) {
            throughput[x] = net_input[x] + nets[bias_start + 2 + x];
        }

        net_input = throughput;

        uint function_layer_start = function_start + 2;

        for (uint i = 0; i < nets[function_start + 1]; i += 3) {
            uint x = i + 1;
            uint y = i + 2;

            throughput[x] = function(i, throughput[x], throughput[y]);
        }

        net_input = throughput;

        layer_start = layer_start + uint(nets[layer_start]);
    }

    return net_input;
}