#ifndef RANDOM_EVOLVE_H
#define RANDOM_EVOLVE_H

#include "Layer.h"
#include "Math/random.h"

namespace OddityEngine::NeuralNetwork {
    class Random_evolve : public Layer {
    public:
        Random_evolve(size_t input_count, size_t output_count) : Layer(input_count, output_count) {}

        Layer* copy() override {
            return new Random_evolve(*this);
        }

        void evolve(double rate) override {
            auto index = Math::random<size_t>(0, weights.size() + bias.size() - 2);
            auto value = Math::random(-rate, rate);

            if (index < weights.size()) {
                weights[index / weights.columns()][index % weights.columns()] += value;
            }
            else {
                bias[index - weights.size()] += value;
            }
        }

        Vector<double> forward(const Vector<double> input) override {
            this->input = input;
            return this->weights * input + this->bias;
        }

        Vector<double> backward(Vector<double> output_gradient, float learning_rate) override {
            return output_gradient;
        }
    };
}



#endif //RANDOM_EVOLVE_H
