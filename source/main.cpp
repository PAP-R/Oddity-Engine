#include <iostream>

#include <glm/glm.hpp>

#include <Util/Vector.h>
#include <Math/random.h>

#include <Neural Network/Network.h>

#include "Util/File.h"

float f(const float x){ return x != 0 ? 1 / x : 0; };
float f(const float x, const float y, const float z){ return x * y * z; };
float radius = 10;
glm::vec3 sphere_pos(0);

struct Ray {
    glm::vec3 pos;
    glm::vec3 dir;

    OddityEngine::Vector<float> to_vec() {
        return {pos.x, pos.y, pos.z, dir.x, dir.y, dir.z};
    }
};

Ray mk_ray(float from, float to) {
    Ray result = {{OddityEngine::Math::random(from, to), OddityEngine::Math::random(from, to), OddityEngine::Math::random(from, to)}, {OddityEngine::Math::random(from, to), OddityEngine::Math::random(from, to), OddityEngine::Math::random(from, to)}};

    result.dir = normalize(glm::vec3(0) - result.pos);

    return result;
}

OddityEngine::Vector<float> sphere(Ray ray) {
    float tolerance = 1E-3;

    OddityEngine::Vector<float> result(10);

    auto poso = glm::vec3(-ray.pos);

    float len = dot(poso, ray.dir);

    glm::vec3 c = len * ray.dir;

    float dstc = distance(poso, c);

    if (dstc <= radius) {
        float dst = sqrt(pow(radius, 2) - pow(dstc, 2));

        if (len > -dst + tolerance) {
            float sign = 1;

            if (len > dst + tolerance) {
                sign = -1;
            }

            result[0] = len + sign * dst;
            auto pos = ray.pos + ray.dir * result[0];

            auto normal = glm::normalize(pos);

            result[1] = normal.x;
            result[2] = normal.y;
            result[3] = normal.z;
        }
    }

    return result;
};


float test_function(const OddityEngine::NeuralNetwork::Network& net, const OddityEngine::Vector<OddityEngine::Vector<>>& input_list, const OddityEngine::Vector<OddityEngine::Vector<>>& output_list) {
    float error = 0;
    for (int i = 0; i < input_list.size(); i++) {
        if (auto value = net.apply(input_list[i]); value.size() != output_list[i].size() || value[0] * output_list[i][0] < 0) {
            error = std::numeric_limits<float>::infinity();
        }
        else {
            error += (value - output_list[i]).slice(1).abs().mean();
        }
    }

    return error / static_cast<float>(input_list.size());
}

OddityEngine::Vector<OddityEngine::Vector<float>> mk_test_input(size_t count) {
    OddityEngine::Vector<OddityEngine::Vector<>> training_input(count);

    float from = -2;
    float to = 2;

    for (int i = 0; i < count; i++) {
        auto ray = mk_ray(from, to);
        training_input[i] = ray.to_vec();
    }

    return training_input;
}

OddityEngine::Vector<OddityEngine::Vector<float>> mk_test_output(const OddityEngine::Vector<OddityEngine::Vector<float>>& input) {
    OddityEngine::Vector<OddityEngine::Vector<>> training_output(input.size());

    for (int i = 0; i < input.size(); i++) {
        training_output[i] = sphere({{input[i][0], input[i][1], input[i][2]}, {input[i][3], input[i][4], input[i][5]}});
    }

    return training_output;
}

int main() {
    size_t training_size = 100;
    size_t test_size = 10;

    OddityEngine::Vector<OddityEngine::Vector<>> training_input = mk_test_input(training_size);
    OddityEngine::Vector<OddityEngine::Vector<>> training_output = mk_test_output(training_input);

    OddityEngine::Vector<OddityEngine::Vector<>> test_input = mk_test_input(test_size);
    OddityEngine::Vector<OddityEngine::Vector<>> test_output = mk_test_output(test_input);

    OddityEngine::NeuralNetwork::Trainer trainer(5, 5, 0.01, 0.1, test_function);

    OddityEngine::Vector nets(10, OddityEngine::NeuralNetwork::Network(6, 10));

    nets = trainer.train(nets, training_input, training_output, 10);

    for (size_t i = 0; i < test_size; i++) {
        std::cout << test_input[i] << " : " << nets[0].apply(test_input[i]) << " / " << test_output[i] << "\n";
    }

    // std::cout << "Networks:\n" << nets << "\n";

    // auto test_net_csv = nets.to_csv();
    //
    // auto csv_to_net = OddityEngine::NeuralNetwork::Network(test_net_csv.auto_slice()[0]);

    // OddityEngine::Vector<OddityEngine::NeuralNetwork::Network> csv_to_networks(test_net_csv.auto_slice());

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
    // OddityEngine::Util::File::write("Networks/test.csv", nets.to_csv_string());

    // nets = OddityEngine::Vector<OddityEngine::NeuralNetwork::Network>(OddityEngine::Util::File::csv("Networks/test.csv").to_long_float().auto_slice());

    // std::cout << "CSV:\n" << test_net_csv << "\n" << "To Vec:\n" << csv_to_networks << "\n" << "Write and read:\n" << nets[0].to_csv() << "\n" << std::numeric_limits<float>::infinity() - std::numeric_limits<float>::infinity();

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
