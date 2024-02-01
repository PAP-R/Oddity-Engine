#ifndef DENSE_H
#define DENSE_H

#include <Math/basics.h>

#include "Layer.h"

#include <Math/random.h>

namespace OddityEngine::NeuralNetwork {
    class Dense : public Layer {
    public:
        explicit Dense(size_t input_count, size_t output_count, float start_min = -1, float start_max = 1) : Layer(input_count, output_count) {
            for (int x = 0; x < output_count; x++) {
                for (int y = 0; y < input_count; y++) {
                    this->weights[x][y] = Math::random(start_min, start_max);
                }
                this->bias[x] = Math::random(start_min, start_max);
            }
        }

        Vector<> forward(const Vector<> input) override {
            this->input = input;
            return this->weights * input + this->bias;
        };

        Vector<> backward(Vector<> output_gradient, const float learning_rate) override {
            auto weights_gradient = output_gradient * (transpose(this->input));
            this->weights -= weights_gradient * learning_rate;
            this->bias -= output_gradient * learning_rate;
            return this->weights.transpose() * output_gradient;
        };
    };
}


#endif //DENSE_H
