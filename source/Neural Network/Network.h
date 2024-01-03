#ifndef NETWORK_H
#define NETWORK_H

#include <Math/Vector.h>
#include "Internal/Layer.h"
#include "Internal/Random_evolve.h"
#include "Math/SortableVector.h"

namespace OddityEngine::NeuralNetwork {
    template<size_t input_count, size_t output_count>
    class Network {
    protected:
        Vector<std::shared_ptr<Layer>> layers;
        size_t mutations;
        size_t evolutions;
        double add_chance;
        double learning_rate;

        void resize(const size_t layer, const size_t size) {
            if (layer < layers.size() - 1) {
                layers[layer]->resize_output(size);
                layers[layer + 1]->resize_input(size);
            }
        }

        Network evolve() {
            auto evo = *this;
            for (int i = 0; i < layers.size(); i++) {
                evo.layers[i] = layers[i]->copy();
            }
            for (int i = 0; i < evo.mutations; ++i) {
                auto random = Math::random(0., 1.);
                size_t l;
                if (random < evo.add_chance / 2) {
                    l = Math::random<size_t>(0, evo.layers.size());
                    const size_t input_size = l == 0 ? input_count : evo.layers[l - 1]->output_size();
                    const size_t output_size = l == evo.layers.size() ? output_count : evo.layers[l]->input_size();
                    evo.layers.insert(l, std::make_shared<Random_evolve>(input_size, output_size));
                }
                else if (random < evo.add_chance) {
                    l = Math::random<size_t>(0, evo.layers.size() - 2);
                    evo.resize(l, evo.layers[l]->output_size() + 1);
                }
                else {
                    l = Math::random<size_t>(0, evo.layers.size() - 1);
                    evo.layers[l]->evolve(evo.learning_rate);
                }
            }

            return evo;
        }

    public:
        explicit Network(size_t mutations = 1, size_t evolutions = 1, double add_chance = 0.05, double learning_rate = 0.1) : layers(1, nullptr), mutations(mutations), evolutions(evolutions), add_chance(add_chance), learning_rate(learning_rate) {
            layers[0] = std::make_shared<Random_evolve>(input_count, output_count);
        }

        Vector<std::shared_ptr<Layer>> get_layers() {
            return layers;
        }

        Vector<double> apply(Vector<double> input) {
            for (auto l : layers) {
                input = l->forward(input);
            }

            return input;
        }

        double test(const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
            double error = 0;
            for (int i = 0; i < input_list.size(); i++) {
                auto value = this->apply(input_list[i]);
                if (value.size() != output_list[i].size()) {
                    error = std::numeric_limits<double>::infinity();
                }
                else {
                    error += (value - output_list[i]).abs().mean();
                }
            }

            return error / static_cast<double>(input_list.size());
        }

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
            SortableVector<Network> best_nets(0);
            for (auto n : nets) {
                best_nets.sorted_insert(n, n.test(input_list, output_list));
                for (int i = 0; i < n.evolutions; i++) {
                    auto evo = n.evolve();
                    best_nets.sorted_insert(evo, evo.test(input_list, output_list));
                }
            }

            return best_nets.resize(nets.size());
        }

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list, const size_t epochs) {
            for (int i = 0; i < epochs; i++) {
                nets = train(nets, input_list, output_list);
            }

            return nets;
        }
    };

    template<size_t in, size_t out>
    inline std::ostream& operator << (std::ostream& os, Network<in, out>& network) {
        for (auto l : network.get_layers()) {
            os << l << "\n";
        }

        return os;
    }

    template<size_t in, size_t out>
    inline std::ostream& operator << (std::ostream& os, Network<in, out>* network) {
        os << *network;

        return os;
    }
}

#endif //NETWORK_H
