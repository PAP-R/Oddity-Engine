#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>

#include "Layer.h"

namespace OddityEngine::NeuralNetwork {
    class Activation : public Layer {
    public:
        enum ACTIVATION {
            TANH
        };

    protected:
        ACTIVATION activation;

    public:
        Activation(const ACTIVATION activation, const size_t node_count) : Layer(node_count, node_count), activation(activation) {}

        Vector<> forward(const Vector<> input) override {
            auto result = input;
            switch(activation) {
                default:
                    return result;
                case TANH:
                    for (int i = 0; i < result.size(); i++) {
                        result[i] = tanh(result[i]);
                    }
                    return result;
            }
        }

        Vector<> backward(Vector<> output_gradient, const float learning_rate) override {
            switch(activation) {
                default:
                    return output_gradient;
                case TANH:
                    for (int i = 0; i < output_gradient.size(); i++) {
                        output_gradient[i] *= (1 - pow(tanh(Layer::input[i]), 2));
                    }
                    return output_gradient;

            }
        }
    };
}



#endif //ACTIVATION_H
