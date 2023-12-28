#ifndef DENSE_H
#define DENSE_H

#include <sys/stat.h>

#include "Layer.h"

namespace OddityEngine::NeuralNetwork {
    class Dense : public Layer {
    public:
        static float random(const float from, const float to) {
            return from + static_cast<float>(rand()) / (RAND_MAX / (to - from));
        }

        explicit Dense(size_t input_count, size_t output_count, float start_min = -1, float start_max = 1) {
            this->weights.resize(output_count, input_count);
            for (int x = 0; x < output_count; x++) {
                for (int y = 0; y < input_count; y++) {
                    this->weights[x, y] = random(start_min, start_max);
                }
                this->bias[x] = random(start_min, start_max);
            }
        }

        Math::Vector<> forward(const Math::Vector<> input) override {
            this->input = input;
            return this->weights * input + this->bias;
        };

        Math::Vector<> backward(Math::Vector<> output_gradient, const float learning_rate) override {
            auto weights_gradient = output_gradient * this->input;
            this->weights -= weights_gradient * learning_rate;
            this->bias -= output_gradient * learning_rate;
            return this->weights * output_gradient;
        };
    };
}


#endif //DENSE_H
