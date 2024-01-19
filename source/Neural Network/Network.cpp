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
                auto size = evo.layers.size();
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

    inline Network::Network(size_t input_count, size_t output_count, size_t mutations, size_t evolutions, double add_chance, double learning_rate): input_count(input_count), output_count(output_count), mutations(mutations), evolutions(evolutions), add_chance(add_chance), learning_rate(learning_rate) {
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

    Network::operator Vector<Vector<>>() const {
        Vector<Vector<>> result;
    }

    std::ostream& operator<<(std::ostream& os, const Network& network) {
        os << network.get_input_count() << "," << network.get_output_count() << "," << network.get_mutations() << "," << network.get_evolutions() << "," << network.get_add_chance() << "," << network.get_learning_rate() << ",\n";
        for (auto l : network.get_layers()) {
            os << l << "\n";
        }

        return os;
    }

    Vector<double> Network::to_csv() const {
        Vector<double> result;

        this->to_csv(&result);

        return result;
    }

    void Network::to_csv(Vector<double>* result) const {
        result->push_back(1);
        result->push_back(2);
        Vector({input_count, output_count, mutations, evolutions, add_chance, learning_rate}).to_csv(result);
        layers.to_csv(result);
    }

    Network Network::from_csv(const std::string& path) {
        return from_csv(Util::File::csv(path).to_double());
    }

    Network Network::from_csv(const Vector<double>& csv) {
        auto net_csv = csv.auto_slice();

        size_t idx = 1;

        Network result(net_csv[0][idx++], net_csv[0][idx++], net_csv[0][idx++], net_csv[0][idx++], net_csv[0][idx++], net_csv[0][idx++]);

        auto layers_csv = net_csv[1].auto_slice();

        result.layers.resize(layers_csv.size());

        for (size_t i = 0; i < layers_csv.size(); i++) {
            result.layers[i] = Random_evolve::from_csv(layers_csv[i]);
        }

        return result;
    }

    Vector<Network> Network::from_csv_vector(const std::string& path) {
        auto raw = Util::File::csv(path).to_double();

        auto nets = raw.auto_slice();

        Vector<Network> result(nets.size());

        for (size_t i = 0; i < nets.size(); i++) {
            result[i] = from_csv(nets[i]);
        }

        return result;
    }
}
