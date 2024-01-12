#include "Network.h"

#include <iostream>

#include "Util/File.h"

namespace OddityEngine::NeuralNetwork {
    inline void Network::resize(const size_t layer, const size_t size) {
        if (layer < layers.size() - 1) {
            layers[layer].resize_output(size);
            layers[layer + 1].resize_input(size);
        }
    }

    inline Network Network::evolve() {
        auto evo = *this;
        for (int i = 0; i < evo.mutations; ++i) {
            auto random = Math::random(0., 1.);
            size_t l;
            if (random < evo.add_chance / 2) {
                l = Math::random<size_t>(0, evo.layers.size());
                const size_t input_size = l == 0 ? input_count : evo.layers[l - 1].output_size();
                const size_t output_size = l == evo.layers.size() ? output_count : evo.layers[l].input_size();
                evo.layers.emplace(l, input_size, output_size);
            }
            else if (random < evo.add_chance && evo.layers.size() > 1) {
                l = Math::random<size_t>(0, evo.layers.size() - 2);
                evo.resize(l, evo.layers[l].output_size() + 1);
            }
            else {
                l = Math::random<size_t>(0, evo.layers.size() - 1);
                evo.layers[l].evolve(evo.learning_rate);
            }
        }

        return evo;
    }

    inline Network::Network(size_t input_count, size_t output_count, size_t mutations, size_t evolutions, double add_chance, double learning_rate): input_count(input_count), output_count(output_count), layers(), mutations(mutations), evolutions(evolutions), add_chance(add_chance), learning_rate(learning_rate) {
        layers.emplace_back(input_count, output_count);
    }

    size_t Network::get_input_count() const {
        return input_count;
    }

    size_t Network::get_output_count() const {
        return output_count;
    }

    size_t Network::get_mutations() const {
        return mutations;
    }

    size_t Network::get_evolutions() const {
        return evolutions;
    }

    double Network::get_add_chance() const {
        return add_chance;
    }

    double Network::get_learning_rate() const {
        return learning_rate;
    }

    inline Vector<Random_evolve> Network::get_layers() const {
        return layers;
    }

    inline Vector<double> Network::apply(Vector<double> input) {
        for (auto l : layers) {
            input = l.forward(input);
        }

        return input;
    }

    inline double Network::test(const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
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

    inline Vector<Network> Network::train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list) {
        SortableVector<Network> best_nets(0);
        for (auto n : nets) {
            best_nets.sorted_insert(n, n.test(input_list, output_list));
            for (int i = 0; i < n.evolutions; i++) {
                auto evo = n.evolve();
                best_nets.sorted_insert(evo, evo.test(input_list, output_list));
            }
        }

        std::cout << best_nets.scores()[0] << "\n";
        return best_nets.resize(nets.size());
    }

    Vector<Network> Network::train(Vector<Network> nets, const Vector<Vector<double>>& input_list, const Vector<Vector<double>>& output_list, const size_t epochs) {
        for (int i = 0; i < epochs; i++) {
            std::cout << "[ " << i + 1 << " / " << epochs << " ]: ";
            nets = train(nets, input_list, output_list);
        }

        return nets;
    }

    Network::operator std::string() const {
        std::ostringstream str;
        str << (*this);
        return str.str();
    }

    std::ostream& operator<<(std::ostream& os, const Network& network) {
        os << network.get_input_count() << "," << network.get_output_count() << "," << network.get_mutations() << "," << network.get_evolutions() << "," << network.get_add_chance() << "," << network.get_learning_rate() << ",\n";
        for (auto l : network.get_layers()) {
            os << l << "\n";
        }

        return os;
    }

    Network Network::from_csv(const std::string& path) {
        auto raw = Util::File::csv(path);

        Network result(
            std::stoull(raw[0]),
            std::stoull(raw[1]),
            std::stoull(raw[2]),
            std::stoull(raw[3]),
            std::stod(raw[4]),
            std::stod(raw[5])
        );

        result.layers.resize(0);

        size_t size, dimensions, rows, columns;

        for (size_t i = 6; i < raw.size();) {
            double weight_chance = std::stod(raw[i++]);
            double bias_chance = std::stod(raw[i++]);
            double function_chance = std::stod(raw[i++]);

            dimensions = std::stoull(raw[i++]);
            rows = std::stoull(raw[i++]);
            columns = std::stoull(raw[i++]);

            Matrix<> weights(rows, columns);
            for (size_t x = 0; x < rows; x++) {
                for (size_t y = 0; y < columns; y++) {
                    weights[x][y] = std::stod(raw[i + x * columns + y]);
                }
            }

            i += rows * columns;

            dimensions = std::stoull(raw[i++]);
            rows = std::stoull(raw[i++]);

            Vector<> bias(rows);
            for (size_t x = 0; x < rows; x++) {
                bias[x] = std::stod(raw[i + x]);
            }

            i += rows;

            dimensions = std::stoull(raw[i++]);
            rows = std::stoull(raw[i++]);

            Vector<Vector<size_t>> functions(rows);
            for (size_t x = 0; x < rows; x++) {
                dimensions = std::stoull(raw[i++]);
                columns = std::stoull(raw[i++]);
                for (size_t y = 0; y < columns; y++) {
                    functions[x].push_back(std::stoull(raw[i + y]));
                }

                i += columns;
            }

            result.layers.emplace_back(weights, bias, functions, weight_chance, bias_chance, function_chance);
        }

        return result;
    }
}
