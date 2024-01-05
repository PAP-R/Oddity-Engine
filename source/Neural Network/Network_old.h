#ifndef NETWORK_H
#define NETWORK_H

#include <memory>
#include <valarray>
#include <variant>
#include <Math/Vector.h>
#include <../Util/Matrix.h>

#include "Internal/Layer.h"
#include "Internal/Random_evolve.h"
#include "Math/random.h"
#include "Math/SortableVector.h"


namespace OddityEngine::NeuralNetwork {
    using Layer_types = std::variant<Layer, Random_evolve>;

    template<size_t input_count, size_t output_count>
    class Network {
    protected:
        Vector<Layer*> layers;
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

        static Network evolve(Network net) {
            for (int i = 0; i < net.mutations; i++) {
                auto add = Math::random(0., 1.);
                size_t l;
                if (add < net.add_chance / 2) {
                    l = Math::random<size_t>(0, net.layers.size());
                    const size_t input_size = l == 0 ? input_count : net.layers[l - 1]->output_size();
                    const size_t output_size = l == net.layers.size() ? output_count : net.layers[l]->input_size();
                    net.layers.insert(l, new Random_evolve(input_size, output_size));
                }
                else if (add < net.add_chance) {
                    l = Math::random<size_t>(0, net.layers.size() - 2);
                    net.resize(l, net.layers[l]->output_size() + 1);
                }
                else {
                    l = Math::random<size_t>(0, net.layers.size() - 1);
                    net.layers[l]->evolve(net.learning_rate);
                }

            }

            return net;
        }

        static Network* evolve(Network* net) {
            auto evo = new Network(*net);
            *evo = evolve(*evo);
            return evo;
        }

    public:
        explicit Network(size_t mutations = 1, size_t evolutions = 1, double add_chance = 0.05, double learning_rate = 0.1) : mutations(mutations), evolutions(evolutions), add_chance(add_chance), learning_rate(learning_rate), layers(1, nullptr) {
            layers[0] = new Random_evolve(input_count, output_count);
        }

        ~Network() {
            for (auto l : layers) {
                delete l;
            }
        }

        Network(const Network& other)
            : layers(other.layers.size()),
              mutations{other.mutations},
              evolutions{other.evolutions},
              add_chance{other.add_chance},
        learning_rate{other.learning_rate} {
            for (int i = 0; i < layers.size(); i++) {
                layers[i] = other.layers[i]->copy();
            }
        }

        Network(Network&& other) noexcept
            : layers(other.layers.size()),
              mutations{other.mutations},
              evolutions{other.evolutions},
              add_chance{other.add_chance},
        learning_rate{other.learning_rate} {
            for (int i = 0; i < layers.size(); i++) {
                layers[i] = other.layers[i]->copy();
            }
        }

        Network& operator=(const Network& other) {
            if (this == &other)
                return *this;
            layers.resize(other.layers.size());
            for (int i = 0; i < layers.size(); i++) {
                layers[i] = other.layers[i]->copy();
            }
            mutations = other.mutations;
            evolutions = other.evolutions;
            add_chance = other.add_chance;
            learning_rate = other.learning_rate;
            return *this;
        }

        Network& operator=(Network&& other) noexcept {
            if (this == &other)
                return *this;
            layers.resize(other.layers.size());
            for (int i = 0; i < layers.size(); i++) {
                layers[i] = other.layers[i]->copy();
            }
            mutations = other.mutations;
            evolutions = other.evolutions;
            add_chance = other.add_chance;
            learning_rate = other.learning_rate;
            return *this;
        }

        Vector<Layer*>& get_layers() {
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

        double train(const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
            double lowest = this->test(input_list, output_list);
            Network lowest_net = *this;

            for (int i = 0; i < evolutions; i++) {
                auto evo = evolve(*this);
                auto evo_error = evo.test(input_list, output_list);

                if (evo_error < lowest) {
                    lowest_net = evo;
                    lowest = evo_error;
                }
            }

            *this = lowest_net;
            return lowest;
        }

        static Vector<Network*> train(Vector<Network*>& nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
            SortableVector<Network*> best_nets(0);

            for (auto n : nets) {
                auto score = n->test(input_list, output_list);
                best_nets.sorted_insert(n, score);
                for (int i = 0; i < n->evolutions; i++) {
                    auto evo = evolve(n);
                    score = evo->test(input_list, output_list);
                    best_nets.sorted_insert(evo, score);
                }
            }

            for (int i = nets.size(); i < best_nets.size(); i++) {
                delete(best_nets[i]);
            }

            std::cout << best_nets.scores()[0] << "\n";
            return best_nets.resize(nets.size());
        }

        double train(const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list, size_t epochs) {
            double error = 0;
            for (int i = 0; i < epochs; ++i) {
                error = train(input_list, output_list);
            }

            return error;
        }

        static Vector<Network> train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list, size_t epochs) {
            for (int i = 0; i < epochs; ++i) {
                std::cout << "[" << i << "/" << epochs << "]: ";
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
