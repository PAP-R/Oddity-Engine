#ifndef RANDOM_EVOLVE_H
#define RANDOM_EVOLVE_H

#include "Layer.h"
#include "Math/random.h"

namespace OddityEngine::NeuralNetwork {
    class Random_evolve : public Layer {
    protected:
        inline static Vector<double(*)(double)> available_functions = {
            [](const double x){return x * x;},
            [](const double x){return sin(x);},
            [](const double x){return cos(x);},
            [](const double x){return tanh(x);},
            [](const double x){return 1 / (1 + pow(std::numbers::e, -x));},
        };

    public:
        Random_evolve(size_t input_count, size_t output_count) : Layer(input_count, output_count) {}

        Layer* copy() override {
            return new Random_evolve(*this);
        }

        void evolve(double rate) override {
            size_t index;
            switch (Math::random<size_t>(0, 2)) {
                case 0:
                    index = Math::random<size_t>(0, weights.size() - 1);
                    weights[index / weights.columns()][index % weights.columns()] += Math::random(-rate, rate);
                    break;
                case 1:
                    index = Math::random<size_t>(0, bias.size() - 1);
                    bias[index] += Math::random(-rate, rate);
                    break;
                case 2:
                    index = Math::random<size_t>(0, functions.size() - 1);
                    functions[index] = available_functions[Math::random<size_t>(0, available_functions.size() - 1)];
            }
        }

        Vector<double> forward(const Vector<double> input) override {
            this->input = input;
            return this->functions(this->weights * input + this->bias);
        }

        Vector<double> backward(Vector<double> output_gradient, float learning_rate) override {
            return output_gradient;
        }
    };
}



#endif //RANDOM_EVOLVE_H
