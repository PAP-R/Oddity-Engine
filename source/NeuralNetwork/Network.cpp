#include "Network.h"

#include <iostream>
#include <utility>

#include "Util/File.h"
#include <Math/Vector.h>

namespace OddityEngine::NeuralNetwork {
    inline void Network::resize(const size_t layer, const size_t size) {
        if (layer < layers.size() - 1) {
            layers[layer].resize_output(size);
            layers[layer + 1].resize_input(size);
        }
    }

    inline Network::Network(size_t input_count, size_t output_count) : input_count(input_count), output_count(output_count) {
        layers.emplace_back(input_count, output_count);
    }

    Network::Network(const Vector<>& csv) : layers(csv.auto_slice()) {
        input_count = layers.front().input_size();
        output_count = layers.back().output_size();
    }

    size_t Network::get_input_count() const {
        return input_count;
    }

    size_t Network::get_output_count() const {
        return output_count;
    }

    inline Vector<Random_evolve> Network::get_layers() const {
        return layers;
    }

    inline Vector<> Network::apply(const Vector<>& input) const {
        auto result = input;
        for (auto l : layers) {
            result = l.forward(result);
        }

        return result;
    }

    inline float Trainer::test(const Network& net, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list) {
        float error = 0;
        for (int i = 0; i < input_list.size(); i++) {
            auto value = net.apply(input_list[i]);
            if (value.size() != output_list[i].size()) {
                error = std::numeric_limits<float>::infinity();
            }
            else {
                error += (value - output_list[i]).abs().mean();
            }
        }

        return error / static_cast<float>(input_list.size());
    }

    inline Vector<Network> Trainer::train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list) {
        SortableVector<Network> best_nets(0);
        for (auto n : nets) {
            best_nets.sorted_insert(n, test(n, input_list, output_list));
            for (int i = 0; i < evolutions; i++) {
                auto evo = evolve(n);
                best_nets.sorted_insert(evo, test(evo, input_list, output_list));
            }
        }

        std::cout << best_nets.scores()[0] << "\n";

        if (std::isnan(best_nets.scores()[0]) || std::isinf(best_nets.scores()[0])) {
            return nets;
        }

        return best_nets.resize(nets.size());
    }

    Vector<Network> Trainer::train(Vector<Network> nets, const Vector<Vector<>>& input_list, const Vector<Vector<>>& output_list, const size_t epochs) {
        for (int i = 0; i < epochs; i++) {
            std::cout << "[ " << i + 1 << " / " << epochs << " ]: ";
            nets = train(nets, input_list, output_list);
        }

        return nets;
    }

    Vector<Network> Trainer::train(Vector<Network> nets, size_t count, std::function<Vector<>(void)> input, std::function<Vector<>(Vector<>)> output, size_t epochs) {
        Vector<Vector<>> input_list;
        Vector<Vector<>> output_list;

        for (size_t i = 0; i < count; i++) {
            auto temp_input = input();
            input_list.push_back(temp_input);

            output_list.push_back(output(temp_input));
        }

        return train(nets, input_list, output_list, epochs);
    }

    Network::operator std::string() const {
        std::ostringstream str;
        str << (*this);
        return str.str();
    }

    std::ostream& operator<<(std::ostream& os, const Network& network) {
        os << network.get_input_count() << " -> " << network.get_output_count() << ",\n";
        for (auto l : network.get_layers()) {
            os << l << "\n";
        }

        return os;
    }

    Vector<> Network::to_csv() const {
        Vector<> result;

        this->to_csv(&result);

        return result;
    }

    void Network::to_csv(Vector<>* result) const {
        layers.to_csv(result);
    }

    size_t Network::csv_count() const {
        return layers.csv_count();
    }


    inline Network Trainer::evolve(const Network& net) {
        auto evo = net;
        for (int i = 0; i < mutations; ++i) {
            auto random = Math::random(0., 1.);
            size_t l;
            if (random < add_chance / 2) {
                auto size = evo.layers.size();
                l = Math::random<size_t>(0, evo.layers.size());
                const size_t input_size = l == 0 ? net.input_count : evo.layers[l - 1].output_size();
                const size_t output_size = l == evo.layers.size() ? net.output_count : evo.layers[l].input_size();
                evo.layers.emplace(l, input_size, output_size);
            }
            else if (random < add_chance && evo.layers.size() > 1) {
                l = Math::random<size_t>(0, evo.layers.size() - 2);
                size_t size = evo.layers[l].output_size() + 1;
                if (size <= MAX_THROUGHPUT) {
                    evo.resize(l, size);
                }
            }
            else {
                l = Math::random<size_t>(0, evo.layers.size() - 1);
                evo.layers[l].evolve(learning_rate);
            }
        }

        return evo;
    }

    Trainer::Trainer(size_t mutations, size_t evolutions, float add_chance, float learning_rate, const std::function<float(const Network&, const Vector<Vector<>>&, const Vector<Vector<>>&)>& test_function) : mutations(mutations), evolutions(evolutions), add_chance(add_chance), learning_rate(learning_rate), test_function(test_function) {}
}
