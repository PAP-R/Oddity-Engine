#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <algorithm>
#include <utility>
#include <vector>
#include <any>
#include <memory>

#include "Internal/Dense.h"
#include "Internal/Layer.h"
#include "Internal/Losses.h"

namespace OddityEngine::NeuralNetwork {
    template<size_t input_count, size_t output_count>
    class Network {
    protected:
        std::vector<Layer*> net;
        float learning_rate;

    public:
        Network(std::vector<Layer*> net, float learning_rate = 0.1) : net(std::move(net)), learning_rate(learning_rate) {}
        Network(float learning_rate = 0.1) : learning_rate(learning_rate) {
            this->net.emplace_back(new Dense(input_count, output_count));
        }

        Math::Vector<> train(Math::Vector<> input, Math::Vector<> output) {
            auto throughput = input.copy();
            for (auto layer : net) {
                throughput = layer->forward(throughput);
            }

            auto gradient = mse_prime(output, throughput);
            for (int i = net.size() - 1; i >= 0; i--) {
                gradient = net[i]->backward(gradient, learning_rate);
            }

            return throughput;
        }

        Math::Vector<> apply(Math::Vector<> input) {
            Math::Vector<> throughput = input;
            for (auto layer : net) {
                throughput = layer->forward(throughput);
            }
            return throughput;
        }
    };
}


#endif //NEURALNETWORK_H
