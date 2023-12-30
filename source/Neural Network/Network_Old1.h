#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <algorithm>
#include <utility>
#include <vector>
#include <any>
#include <iostream>
#include <memory>

#include "Internal/Dense.h"
#include "Internal/Layer.h"
#include "Internal/Losses.h"

namespace OddityEngine::NeuralNetwork {
    template<size_t input_count, size_t output_count>
    class Network_Old1 {
    protected:
        std::vector<Layer*> net;
        float learning_rate;

    public:
        Network_Old1(std::vector<Layer*> net, float learning_rate = 0.1) : net(std::move(net)), learning_rate(learning_rate) {}
        Network_Old1(float learning_rate = 0.1) : learning_rate(learning_rate) {
            this->net.emplace_back(new Dense(input_count, output_count));
        }

        [[nodiscard]] float train(const Vector<>& input, const Vector<>& output) const {
            std::cout << "Training start" << std::endl << input << std::endl << std::endl;
            auto throughput = input;
            std::cout << throughput << std::endl << std::endl;
            for (auto layer : net) {
                throughput = layer->forward(throughput);
            std::cout << throughput << std::endl << std::endl;
            }

            auto gradient = mse_prime(output, input);
            for (int i = net.size() - 1; i >= 0; i--) {
                gradient = net[i]->backward(gradient, learning_rate);
            }

            return mse(output, throughput);
        }

        Vector<> train(const Vector<Vector<>>& inputs, const Vector<Vector<>>& outputs) {
            Vector<> error(1);
            for (int i = 0; i < inputs.size(); i ++) {
                error[0] += train(inputs[i], outputs[i]);
            }

            return error;
        }

        void train(const Vector<Vector<>>& inputs, const Vector<Vector<>>& outputs, const size_t epochs) {
            for (int i = 0; i < epochs; i++) {
                fmt::print("{:d} : {:s}\n\n", i, std::string(train(inputs, outputs)));
            }
        }

        Vector<> apply(const Vector<>& input) {
            auto throughput = input;
            for (auto layer : net) {
                throughput = layer->forward(throughput);
            }

            return throughput;
        }
    };
}


#endif //NEURALNETWORK_H
