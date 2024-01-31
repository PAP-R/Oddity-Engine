#include <iostream>

#include <glm/glm.hpp>

#include <Util/Vector.h>
#include <Math/random.h>

#include <Neural Network/Network.h>

#include "Util/File.h"

double f(const double x){ return x != 0 ? 1 / x : 0; };
double f(const double x, const double y, const double z){ return x * y * z; };
double radius = 1;
glm::vec3 sphere_pos(0);
double sphere(const double x, const double y, const double z){ return glm::distance(glm::vec3(x, y, z), sphere_pos) - radius; };

int main() {
    size_t training_size = 11;
    size_t test_size = 21;

    double from = -10;
    double to = 10;

    OddityEngine::Vector<OddityEngine::Vector<>> training_input(training_size);
    OddityEngine::Vector<OddityEngine::Vector<>> training_output(training_size);

    for (int i = 0; i < training_size; i++) {
        double x = OddityEngine::Math::random(from, to);
        double y = OddityEngine::Math::random(from, to);
        double z = OddityEngine::Math::random(from, to);
        training_input[i] = {x, y, z};
        training_output[i] = {sphere(x, y, z)};
    }

    OddityEngine::Vector<OddityEngine::Vector<>> test_input(test_size);
    OddityEngine::Vector<OddityEngine::Vector<>> test_output(test_size);

    for (int i = 0; i < test_size; i++) {
        double x = OddityEngine::Math::random(from, to);
        double y = OddityEngine::Math::random(from, to);
        double z = OddityEngine::Math::random(from, to);
        test_input[i] = {x, y, z};
        test_output[i] = {sphere(x, y, z)};
    }

    size_t net_count = 10;

    OddityEngine::NeuralNetwork::Trainer trainer(5, 5, 0.01);

    OddityEngine::Vector nets(net_count, OddityEngine::NeuralNetwork::Network(3, 1));

    nets = trainer.train(nets, training_input, training_output, 1000);

    std::cout << "Networks:\n" << nets << "\n";

    auto test_net_csv = nets.to_csv();

    auto csv_to_net = OddityEngine::NeuralNetwork::Network(test_net_csv.auto_slice()[0]);

    OddityEngine::Vector<OddityEngine::NeuralNetwork::Network> csv_to_networks(test_net_csv.auto_slice());

    // auto test_net_from_csv = OddityEngine::NeuralNetwork::Network::from_csv(test_net_csv);
    //
    //
    // auto csv = nets.to_csv();
    //
    // std::cout << "Base:\n" << csv;
    //
    // std::cout << "\nFirst:\n" << csv.auto_slice();
    //
    // std::cout << "\nSecond:\n" << csv.auto_slice().auto_slice();
    //
    // std::cout << "\nThird:\n" << csv.auto_slice().auto_slice().auto_slice();
    //
    OddityEngine::Util::File::write("Networks/test.csv", nets.to_csv_string());

    nets = OddityEngine::Vector<OddityEngine::NeuralNetwork::Network>(OddityEngine::Util::File::csv("Networks/test.csv").to_long_double().auto_slice());

    std::cout << "CSV:\n" << test_net_csv << "\n" << "To Vec:\n" << csv_to_networks << "\n" << "Write and read:\n" << nets << "\n";

    // size_t load_count = 0;
    //
    // for (size_t i = 0; i < load_count; i++) {
    //     OddityEngine::Vector nets2(net_count, OddityEngine::NeuralNetwork::Network::from_csv("Networks/test.csv"));
    //
    //     nets2 = OddityEngine::NeuralNetwork::Network::train(nets2, training_input, training_output, 100);
    //
    //     std::cout << "Network " << i + 1 << ":\n" << nets2[0] << "\n";
    //
    //     OddityEngine::Util::File::write("Networks/test.csv", nets2[0]);
    // }

    // auto network = OddityEngine::NeuralNetwork::Network::from_csv("Networks/test.csv");
    //
    // std::cout << "Network " << load_count + 1 << ":\n" << network;

    // for (int i = 0; i < test_input.size(); i++) {
    //     std::cout << "\n" << test_input[i] << " -> " << nets[0].apply(test_input[i]) << " ≈ " << test_output[i] << "\n";
    // }
}
